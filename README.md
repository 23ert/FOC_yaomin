# FOC电机控制工程

## 工程简介

本工程是基于STM32G4系列微控制器开发的FOC（磁场定向控制）电机控制系统。该系统实现了三相无刷直流电机的精确控制，采用STM32CubeMX生成的HAL库驱动，结合自定义的板级驱动和应用层逻辑。

## 硬件平台

- **MCU**: STM32G4系列
- **开发环境**: STM32CubeIDE / Keil MDK
- **配置工具**: STM32CubeMX (STM32G4_CONFIG.ioc)

## 功能模块

### 外设模块
- **ADC**: 模数转换器，用于电流和电压采样
- **DAC**: 数模转换器，用于模拟信号输出
- **COMP**: 比较器，用于过零检测和比较功能
- **OPAMP**: 运算放大器，用于信号调理
- **TIM**: 定时器，用于PWM波形生成和编码器接口
- **USART**: 串口通信，用于调试和数据传输
- **DMA**: 直接存储器访问，用于高效数据传输
- **GPIO**: 通用输入输出，用于按键、LED等控制

### 应用功能
- **PWM控制**: 三相PWM输出控制
- **状态机**: 运行/停止状态管理
- **按键中断**: 外部中断响应
- **LED指示**: 状态显示
- **故障处理**: 故障检测和处理机制

## 工程结构

```
FOC/
├── Board/              # 板级驱动和应用层
│   ├── board.c/h      # 板级初始化和控制
│   ├── device.h       # 设备定义
│   ├── fault_handler.c/h  # 故障处理
│   └── sample_Init.c/h     # 采样初始化
├── Core/              # STM32 HAL核心代码
│   ├── Inc/           # 头文件
│   └── Src/           # 源文件
├── Drivers/           # STM32 HAL和CMSIS库
│   ├── CMSIS/         # CMSIS核心和DSP库
│   └── STM32G4xx_HAL_Driver/  # HAL驱动库
└── STM32G4_CONFIG.ioc # CubeMX配置文件
```

## 主要API接口

### PWM控制
- `ENPWMABC()`: 使能三相PWM输出
- `DISPWMABC()`: 禁用三相PWM输出
- `ENPWM_ADC()`: 使能PWM和ADC同步

### 状态控制
- `StateMachine_SetState()`: 设置系统状态

## 代码修改记录

| 日期       | 版本  | 修改内容 | 修改人 |
| ---------- | ----- | -------- | ------ |
| 2026/04/25 | V1.00 | 提交     |        |

## 注意事项

1. 本工程使用STM32 HAL库开发，遵循HAL库的编程规范
2. 修改代码时请注意保持HAL库生成的代码区域不被修改
3. PWM输出和ADC采样需要精确同步，修改定时器配置时需谨慎
4. 故障处理机制需要根据实际硬件进行调整

## 许可证

本工程遵循STMicroelectronics的BSD 3-Clause许可证