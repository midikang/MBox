/*
 * 程序入口 / Program Entry Point
 * 简单转发到CLI主函数 / Simply forwards to CLI main function
 */

#include "../include/claw.h"

int main(int argc, char **argv) {
    return cli_main(argc, argv);
}
