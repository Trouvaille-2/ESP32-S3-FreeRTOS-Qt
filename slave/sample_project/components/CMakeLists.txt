idf_component_register(
    SRCS
        "modbus/modbus.c"
        "device/device.c"
        "uart/myuart.c"
        "pid/pid.c"
        "motor/motor.c"
        "app/app.c"
    INCLUDE_DIRS
        "."
        "modbus"
        "device"
        "uart"
        "pid"
        "motor"
        "app"
    REQUIRES driver
)

# 编译优化选项（可选）
component_compile_options(
    -O3
    -Wno-format
    -Wno-format-security
)