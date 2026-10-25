# OSKernel2026 Starter — RISC-V64 教学启动基线

> **状态：教学原型 / 非官方评测适配版。** 本仓库仅演示 QEMU `virt` + OpenSBI 下的最小 S 模式内核入口，**不是完整操作系统、不能运行用户程序、没有官方评测成绩**。在获取中西部与华南区域赛的专用评测规范前，不能声称它满足比赛提交要求。

## 现有功能

- RISC-V64 汇编启动、清零 BSS、设置 16 KiB 启动栈。
- QEMU `virt` 平台 UART 串口输出，打印 Hart ID 和设备树地址。
- 16 页、每页 4 KiB 的 **内核私有静态页池**（分配、释放、耗尽与 double-free 检查）。这不是发现整机 RAM 的物理页分配器。
- 使用 SBI SRST 请求 QEMU 关机。
- Makefile、Dockerfile、主机端单元测试和 QEMU 串口冒烟测试。

## 推荐开发环境

Linux/macOS 均可使用 Docker，macOS 无需真实 RISC-V 开发板。Dockerfile 使用 Ubuntu 24.04、`riscv64-linux-gnu-gcc` 和 `qemu-system-riscv64`，是 **自定义演示环境**，不是赛方镜像。

```bash
docker build -t oskernel-starter:local .

# 构建 RISC-V64 ELF
docker run --rm -v "$PWD":/work -w /work oskernel-starter:local make all

# 执行 QEMU 串口冒烟验证（正常输出 BOOT_OK）
docker run --rm -v "$PWD":/work -w /work oskernel-starter:local make smoke

# 主机端内存页池测试
docker run --rm -v "$PWD":/work -w /work oskernel-starter:local make test
```

已经安装跨编译器与 QEMU 的 Linux 系统也可直接执行 `make all`、`make smoke`、`make test`。

运行成功时的自定义判定标记：

```text
[starter] RISC-V64 supervisor payload entered
[starter] hart = 0x0000000000000000  dtb = 0x...（实际地址以 QEMU 为准）
BOOT_OK: UART and page pool self-test passed
```

## 工程结构

```text
arch/riscv64/boot.S      RISC-V64 入口和启动栈
arch/riscv64/kernel.ld   内存链接布局（0x80200000）
kernel/main.c            启动流程及最小自测
kernel/uart.c            QEMU virt 串口设备
kernel/sbi.c             OpenSBI 关机调用
kernel/page_pool.c       内核私有静态页池
kernel/mem.c             独立运行环境下的基础内存函数
include/                 对外接口
tests/                   可在主机执行的单元测试
scripts/smoke.sh         QEMU 冒烟验证脚本
docs/                    开发路线、评测对接、AI 使用记录
Dockerfile               示例构建和运行环境
Makefile                 构建入口
```

## 当前明确不支持

- U-mode 用户进程、ELF 加载、系统调用、trap 上下文保存；
- Sv39 页表、物理内存发现、虚拟地址空间隔离；
- 时钟中断、抢占调度、多核同步；
- VFS、FAT32、块设备、管道、网络；
- LoongArch64 或其他架构；
- 官方自动评测协议与自动成绩上传。

## 下一步最优先的工作

1. 从官方资料核实本区域赛实际的目标 ISA、`make` 入口、产物文件名、Docker 镜像、测试程序来源和提交分支规范；切勿直接复制全国赛或往年赛规。
2. 在官方指定环境内完成第一次最小构建和在线测试，保存分数与日志。
3. 用评测清单反推缺失的内核能力，优先补异常/系统调用、进程/ELF、内存和文件接口。
4. 每次新功能加入 host-test / QEMU-test / online-test 三类回归，严禁仅凭启动横幅认定测试成功。

具体见 [实施路线](docs/实施路线_区域赛.md)、[官方评测对接清单](docs/评测对接清单.md)、[AI 使用记录](docs/AI使用记录.md)。

## GitLab 提交提醒

赛区要求在比赛 GitLab 开发和提交，**推拉使用 HTTPS，不支持 SSH**。注册流程需先在比赛平台设置密码，之后从平台入口登录 GitLab。项目名称可按赛事建议设为 `OSKernel2026-X`，项目描述填写队伍 ID、队名、学校名。

演示视频需上传百度网盘，并将分享链接填写在比赛仓库 README；当前原型尚无正式参赛视频。

## 代码来源和 AI 辅助

本初始原型是根据当前对话新写的教学代码，未直接复制第三方操作系统源码。构建工具本身属于第三方依赖。若后续引入 xv6、rCore、其他内核或库，请单独保留许可证、来源和修改声明。竞赛的人工复核、设计贡献及 AI 记录义务由团队承担；详见 [AI 使用记录](docs/AI使用记录.md)。
