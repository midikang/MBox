/*
 * organize.c - Windows folder organization command
 * 整理Windows文件夹 - 按文件类型将文件分类移动到子目录
 */

#include "../include/claw.h"
#include "../include/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Category definitions / 文件分类定义                                  */
/* ------------------------------------------------------------------ */

#define MAX_EXTS 32

typedef struct {
    const char *name;               /* Subdirectory name / 子目录名 */
    const char *exts[MAX_EXTS];    /* NULL-terminated extension list (lowercase, no dot) */
} FileCategory;

static const FileCategory CATEGORIES[] = {
    {
        "Documents",
        { "doc","docx","xls","xlsx","ppt","pptx","pdf","txt","md",
          "csv","rtf","odt","ods","odp","pages","numbers","keynote",
          "log","ini","cfg","conf","xml","json","yaml","yml", NULL }
    },
    {
        "Images",
        { "jpg","jpeg","png","gif","bmp","ico","tiff","tif","svg",
          "webp","raw","cr2","nef","arw","heic","heif","psd","ai",
          "eps","xcf", NULL }
    },
    {
        "Videos",
        { "mp4","avi","mkv","mov","wmv","flv","m4v","webm","rmvb",
          "rm","3gp","ts","mts","m2ts","vob","mpg","mpeg","divx", NULL }
    },
    {
        "Audio",
        { "mp3","wav","flac","aac","ogg","wma","m4a","opus","ape",
          "aiff","aif","mid","midi","amr","ac3","dts", NULL }
    },
    {
        "Archives",
        { "zip","rar","7z","tar","gz","bz2","xz","cab","iso","img",
          "dmg","tgz","tbz2","lz","lzma","zst", NULL }
    },
    {
        "Code",
        { "c","cpp","cc","cxx","h","hpp","py","js","ts","java","go",
          "rs","cs","php","rb","swift","kt","m","html","htm","css",
          "scss","less","sh","bat","cmd","ps1","lua","r","sql", NULL }
    },
    {
        "Executables",
        { "exe","msi","dll","sys","drv","ocx","scr","com","vbs",
          "jar","apk","deb","rpm", NULL }
    },
    {
        "Fonts",
        { "ttf","otf","woff","woff2","eot","fon","fnt", NULL }
    },
};

#define NUM_CATEGORIES ((int)(sizeof(CATEGORIES) / sizeof(CATEGORIES[0])))
#define CATEGORY_OTHERS "Others"

/* ------------------------------------------------------------------ */
/* Helpers / 工具函数                                                    */
/* ------------------------------------------------------------------ */

/* Return lowercase extension without leading dot, e.g. "TXT" -> "txt"
   写入 buf (at least 16 bytes)，返回 buf；找不到扩展名返回 NULL */
static const char *get_ext_lower(const char *filename, char *buf, int buf_size) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return NULL;   /* no extension or hidden file */

    int i;
    for (i = 0; i < buf_size - 1 && dot[i + 1]; i++) {
        char c = dot[i + 1];
        if (c >= 'A' && c <= 'Z') c = (char)(c + ('a' - 'A'));
        buf[i] = c;
    }
    buf[i] = '\0';
    return buf;
}

/* Find which category the extension belongs to; returns category name or NULL */
static const char *classify_ext(const char *ext_lower) {
    if (!ext_lower) return NULL;
    for (int ci = 0; ci < NUM_CATEGORIES; ci++) {
        for (int ei = 0; CATEGORIES[ci].exts[ei] != NULL; ei++) {
            if (strcmp(ext_lower, CATEGORIES[ci].exts[ei]) == 0)
                return CATEGORIES[ci].name;
        }
    }
    return NULL;
}

/* Build "dir\subdir" path.  Returns 0 on success. */
static int build_subdir_path(const char *dir, const char *subdir,
                              char *out, int out_size) {
    int n = _snprintf(out, out_size, "%s\\%s", dir, subdir);
    return (n > 0 && n < out_size) ? 0 : -1;
}

/* Build "dir\filename" path. Returns 0 on success. */
static int build_file_path(const char *dir, const char *filename,
                            char *out, int out_size) {
    int n = _snprintf(out, out_size, "%s\\%s", dir, filename);
    return (n > 0 && n < out_size) ? 0 : -1;
}

/* ------------------------------------------------------------------ */
/* Core organize logic / 核心整理逻辑                                   */
/* ------------------------------------------------------------------ */

/*
 * Organize files in `dir`.
 * dry_run=1 : only print what would be done, do not move anything.
 * dry_run=0 : actually create subdirectories and move files.
 *
 * Returns 0 on success, non-zero on fatal error.
 */
int cmd_organize(const char *dir, int dry_run) {
    if (!dir) {
        fprintf(stderr, "Error: directory path required\n");
        return 1;
    }

    /* Verify the target directory exists */
    DWORD attr = GetFileAttributesA(dir);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        fprintf(stderr, "Error: '%s' is not a valid directory\n", dir);
        return 1;
    }

    char pattern[MAX_PATH];
    if (build_file_path(dir, "*", pattern, MAX_PATH) != 0) {
        fprintf(stderr, "Error: path too long\n");
        return 1;
    }

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA(pattern, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: cannot enumerate directory '%s'\n", dir);
        return 1;
    }

    int moved   = 0;
    int skipped = 0;
    int errors  = 0;

    if (dry_run) {
        printf("[Dry Run] Scanning: %s\n\n", dir);
    } else {
        printf("Organizing: %s\n\n", dir);
    }

    do {
        /* Skip . and .. and subdirectories */
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        const char *filename = ffd.cFileName;

        char ext_buf[32];
        const char *ext = get_ext_lower(filename, ext_buf, sizeof(ext_buf));
        const char *category = classify_ext(ext);  /* NULL means "Others" */
        const char *dest_subdir = category ? category : CATEGORY_OTHERS;

        /* Build destination subdirectory path */
        char dest_dir[MAX_PATH];
        if (build_subdir_path(dir, dest_subdir, dest_dir, MAX_PATH) != 0) {
            fprintf(stderr, "  SKIP (path too long): %s\n", filename);
            skipped++;
            continue;
        }

        /* Build source and destination file paths */
        char src_path[MAX_PATH];
        char dst_path[MAX_PATH];
        if (build_file_path(dir, filename, src_path, MAX_PATH) != 0 ||
            build_file_path(dest_dir, filename, dst_path, MAX_PATH) != 0) {
            fprintf(stderr, "  SKIP (path too long): %s\n", filename);
            skipped++;
            continue;
        }

        if (dry_run) {
            printf("  [Would move] %-40s -> %s\\\n", filename, dest_subdir);
            moved++;
            continue;
        }

        /* Create destination subdirectory if needed */
        DWORD dattr = GetFileAttributesA(dest_dir);
        if (dattr == INVALID_FILE_ATTRIBUTES) {
            if (!CreateDirectoryA(dest_dir, NULL)) {
                fprintf(stderr, "  ERROR creating dir '%s': %lu\n",
                        dest_dir, GetLastError());
                errors++;
                continue;
            }
        }

        /* Move (rename) the file */
        if (MoveFileExA(src_path, dst_path,
                        MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)) {
            printf("  Moved: %-40s -> %s\\\n", filename, dest_subdir);
            moved++;
        } else {
            DWORD err = GetLastError();
            if (err == ERROR_ALREADY_EXISTS || err == ERROR_FILE_EXISTS) {
                fprintf(stderr, "  SKIP (already exists): %s\n", filename);
                skipped++;
            } else {
                fprintf(stderr, "  ERROR moving '%s': %lu\n", filename, err);
                errors++;
            }
        }

    } while (FindNextFileA(hFind, &ffd));

    FindClose(hFind);

    printf("\n");
    if (dry_run) {
        printf("Dry run complete. %d file(s) would be moved.\n", moved);
    } else {
        printf("Done. Moved: %d, Skipped: %d, Errors: %d\n",
               moved, skipped, errors);
    }

    return (errors > 0) ? 1 : 0;
}
