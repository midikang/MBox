using System.Drawing;
using System.Drawing.Imaging;

namespace ImageSmall;

public partial class Form1 : Form
{
    private List<string> selectedFiles = new List<string>();
    
    public Form1()
    {
        InitializeComponent();
        lblQualityValue.Text = trackBarQuality.Value + "%";
    }

    private void btnSelectFiles_Click(object? sender, EventArgs e)
    {
        using (OpenFileDialog openFileDialog = new OpenFileDialog())
        {
            openFileDialog.Filter = "图片文件|*.jpg;*.jpeg;*.png;*.bmp;*.gif|所有文件|*.*";
            openFileDialog.Multiselect = true;
            openFileDialog.Title = "选择要压缩的图片";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (string file in openFileDialog.FileNames)
                {
                    if (!selectedFiles.Contains(file))
                    {
                        selectedFiles.Add(file);
                        listBoxFiles.Items.Add(Path.GetFileName(file));
                    }
                }
                
                lblStatus.Text = $"已选择 {selectedFiles.Count} 个文件";
            }
        }
    }

    private void btnSelectOutput_Click(object? sender, EventArgs e)
    {
        using (FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog())
        {
            folderBrowserDialog.Description = "选择输出目录";
            
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                txtOutputFolder.Text = folderBrowserDialog.SelectedPath;
            }
        }
    }

    private void btnClearList_Click(object? sender, EventArgs e)
    {
        selectedFiles.Clear();
        listBoxFiles.Items.Clear();
        lblStatus.Text = "已清空列表";
    }

    private void trackBarQuality_Scroll(object? sender, EventArgs e)
    {
        lblQualityValue.Text = trackBarQuality.Value + "%";
    }

    private async void btnCompress_Click(object? sender, EventArgs e)
    {
        if (selectedFiles.Count == 0)
        {
            MessageBox.Show("请先选择要压缩的图片！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            return;
        }

        string outputFolder = txtOutputFolder.Text.Trim();
        if (string.IsNullOrEmpty(outputFolder))
        {
            outputFolder = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "压缩后的图片");
        }

        if (!Directory.Exists(outputFolder))
        {
            Directory.CreateDirectory(outputFolder);
        }

        btnCompress.Enabled = false;
        btnSelectFiles.Enabled = false;
        btnClearList.Enabled = false;
        progressBar.Maximum = selectedFiles.Count;
        progressBar.Value = 0;

        int quality = trackBarQuality.Value;
        int successCount = 0;
        int failCount = 0;

        await Task.Run(() =>
        {
            for (int i = 0; i < selectedFiles.Count; i++)
            {
                string sourceFile = selectedFiles[i];
                
                this.Invoke((MethodInvoker)delegate
                {
                    lblStatus.Text = $"正在压缩: {Path.GetFileName(sourceFile)} ({i + 1}/{selectedFiles.Count})";
                });

                try
                {
                    string fileName = Path.GetFileNameWithoutExtension(sourceFile);
                    string extension = Path.GetExtension(sourceFile).ToLower();
                    string outputFile = Path.Combine(outputFolder, $"{fileName}_compressed{extension}");

                    CompressImage(sourceFile, outputFile, quality);
                    successCount++;
                }
                catch (Exception ex)
                {
                    failCount++;
                    this.Invoke((MethodInvoker)delegate
                    {
                        MessageBox.Show($"压缩文件 {Path.GetFileName(sourceFile)} 时出错: {ex.Message}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    });
                }

                this.Invoke((MethodInvoker)delegate
                {
                    progressBar.Value = i + 1;
                });
            }
        });

        btnCompress.Enabled = true;
        btnSelectFiles.Enabled = true;
        btnClearList.Enabled = true;

        lblStatus.Text = $"压缩完成！成功: {successCount}, 失败: {failCount}";
        MessageBox.Show($"压缩完成！\n成功: {successCount}\n失败: {failCount}\n输出目录: {outputFolder}", "完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
    }

    private void CompressImage(string sourceFile, string outputFile, int quality)
    {
        using (var image = Image.FromFile(sourceFile))
        {
            // Get image codec info for JPEG
            ImageCodecInfo? jpegCodec = GetEncoderInfo("image/jpeg");
            
            // Create encoder parameters
            using (EncoderParameters encoderParams = new EncoderParameters(1))
            {
                encoderParams.Param[0] = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, (long)quality);

                // Determine output format based on file extension
                string extension = Path.GetExtension(outputFile).ToLower();
                
                if (extension == ".jpg" || extension == ".jpeg")
                {
                    if (jpegCodec != null)
                    {
                        image.Save(outputFile, jpegCodec, encoderParams);
                    }
                    else
                    {
                        image.Save(outputFile, ImageFormat.Jpeg);
                    }
                }
                else if (extension == ".png")
                {
                    // PNG uses lossless compression - quality setting does not apply
                    image.Save(outputFile, ImageFormat.Png);
                }
                else if (extension == ".bmp")
                {
                    image.Save(outputFile, ImageFormat.Bmp);
                }
                else if (extension == ".gif")
                {
                    image.Save(outputFile, ImageFormat.Gif);
                }
                else
                {
                    // Default to JPEG
                    if (jpegCodec != null)
                    {
                        image.Save(outputFile, jpegCodec, encoderParams);
                    }
                    else
                    {
                        image.Save(outputFile, ImageFormat.Jpeg);
                    }
                }
            }
        }
    }

    private ImageCodecInfo? GetEncoderInfo(string mimeType)
    {
        ImageCodecInfo[] codecs = ImageCodecInfo.GetImageEncoders();
        
        foreach (ImageCodecInfo codec in codecs)
        {
            if (codec.MimeType == mimeType)
            {
                return codec;
            }
        }
        
        return null;
    }
}
