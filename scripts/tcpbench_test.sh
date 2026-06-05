#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TCPBENCH_DIR="${SCRIPT_DIR}/../benchmarks/tcpbench"

cd "$TCPBENCH_DIR" || { echo "目录不存在: $TCPBENCH_DIR" >&2; exit 1; }

# 使用 make -q：如果目标已是最新，返回 0；如果需要构建，返回 1；出错返回 2
if ! make -q tcpbench 2>/dev/null; then
    # echo "检测到源码变更或未构建，正在编译 tcpbench..."
    make tcpbench || { echo "构建失败！" >&2; exit 1; }
fi
    # echo "tcpbench 已是最新的，跳过构建。"
# fi

./tcpbench $@