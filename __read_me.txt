该文件是stm32_ezos的工程模板。

各目录说明如下：
1. bsp：版级支持包
     - 提供 RealView MDK 的工程模板
     - \_eclipse, 提供 Eclipse 的工程模板

2. bsp\开发板\applications：应用层
     - 包含了main文件, 中断处理文件, 系统配置文件, 顶层应用文件

3. components：组件层
     - 按大功能划分的软件组件。如fatfs文件系统、UI组件
     - 不建议直接调用最底层 libraries

4. documents：文档说明，
     - 提供了模板版本说明
     - 提供了模板分层说明
     - 提供 doxygen 注释风格的模板
     - 建议用户将工程的其它说明文档也放在此处
       如优先级规划, 存储结构手册等

5. ezos：操作系统
     - 提供协作式RTOS, ezos 系统

6. drivers：PCB板级驱动
     - 如按键、EEPROM、模拟I2C等等
     - 该层要能对上屏蔽掉最底层 libraries
       这样起到承上启下的作用,方便跨平台移植

7. drivers\libraries：MCU外设库
     - 此部分由芯片厂商提供，是标准库
     - 用户不得修改此层
