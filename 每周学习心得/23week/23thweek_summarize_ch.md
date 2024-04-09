## The 23th Week Summarize

- This week's goal is to study the circuit design structure and design a circuit suitable for installing a differential amplifier.
- First we analyse the MCU chip which I used to design the circuit:
  - <img src="23thweek_summarize.assets/image-20240328191712593.png" alt="image-20240328191712593"  />

<center><b><font size ='2'>Figure 1 MCU Minimum System Circuit</font></b></center></font>

- 根据网络上的芯片产品信息，[GD32F103C8T6-Arm Cortex-M3-兆易创新半导体有限公司 --- GD32F103C8T6-Arm Cortex-M3-GigaDevice Semiconductor Inc.](https://www.gigadevice.com/product/mcu/arm-cortex-m3/gd32f103c8t6)该芯片属于GD32F1系列芯片，其具有以下特点：
  - 基于ARM® Cortex®-M3内核，这种核心专门针对低功耗、实时性和成本效益进行了优化，适用于各种应用，包括汽车、工业控制、消费类电子和医疗设备等。
  
  - 32位系统，支持最高运行频率为108MHz，提供高达3024KB的片上Flash存储器和高达96KB的SRAM。多种增强型 I/O 端口和片上外设连接到两条 APB 总线。
  
  - 提供多达 3 个 12 位 1MSPS ADC 和 10 个通用 16 位定时器以及 1 个 PWM 高级定时器，以及标准和高级通信接口。
  
    > - The GD32F103xx device incorporates the Arm® Cortex®-M3 32-bit processor core operating
    >   at 108 MHz frequency with Flash accesses zero wait states to obtain maximum efficiency. It
    >   provides up to 3 MB on-chip Flash memory and up to 96 KB SRAM memory. An extensive
    >   range of enhanced I/Os and peripherals connected to two APB buses. The devices offer up
    >   to three 12-bit ADCs, up to two 12-bit DACs, up to ten general 16-bit timers, two basic timers
    >   plus two PWM advanced timer, as well as standard and advanced communication interfaces:
    >   up to three SPIs, two I2Cs, three USARTs, two UARTs, two I2Ss, an USBD, a CAN and a
    >   SDIO.
- 如下图所示，根据datasheet里面的图片我们可以确定该芯片是GD32F103的C系列芯片。
  
  - ![image-20240328193302055](23thweek_summarize.assets/image-20240328193302055.png)

<center><b><font size ='2'>Figure 2 GD32F103Cx LQFP48 pinouts</font></b></center></font>

- 然后，我们通过分析芯片的Block Diagram可以知道它有三个12位的ADC接口，并且还是每秒百万样本的ADC。因为我们的需求是至少安装两个4通道差分放大器用于测量肌电信号，所以利用每个ADC具有多个通道，我们到时候可以使用多路选择器（MUX）来切换输入信号源，以实现对多个信号的采样。
  - ![image-20240328193538782](23thweek_summarize.assets/image-20240328193538782.png)

<center><b><font size ='2'>Figure 3 GD32F103Cx Block Diagram</font></b></center></font>

- 根据Figure 4的针脚定义可以看到，我们有总共9个ADC针脚可以使用，所以最多可以同时处理9个差分放大器传输的肌电信号，那么就可以接2个4通道的差分放大器，满足我们的设计要求。
  - ![image-20240328204225564](23thweek_summarize.assets/image-20240328204225564.png)

<center><b><font size ='2'>Figure 4 GD32F103Cx LQFP48 Pin Definitions</font></b></center></font>

- 在分析差分放大器如何接入电路之前，我们需要对整个MCU系统进行分析：
  - #### **3.3V稳压输出电路分析：**

    - 该电路主要用于讲5V输入电压转换成3.3V稳定输出电压。

    - AMS1117-3.3datasheet地址：[Microsoft Word - DS1117 (advanced-monolithic.com)](http://www.advanced-monolithic.com/pdf/ds1117.pdf)

    - 稳压器采用了AMS1117-3.3 这个稳压器选取对于我们的肌电信号能否成功读取有很大关系，因为如果稳压器输出电压值精度不高的话很难侦测到肌电信号，所以我们从电压精度以及电源噪声抑制能力分析该芯片是否适用于我们的电路。

    - **电源噪声抑制能力**：查表可得它的RMS Output Noise只有0.003%，那么可得：

      - $$
        RMS \  Noise = 0.003 *0.01 *3.3V = 0.000099V = 99uV
        $$

      - 由于EMG信号幅度再uV到mV级别，那么还是需要对电源噪声进行一个滤波才能实现比较好的效果。

    - **电压精度**：查表可得AMS1117的输出电压范围再3.201-3.399V范围内，那么也就是有±0.099V的偏差，电压变化分为大约是200mv。

      - 由于电压精度为200mv，那么我们还是需要减少电压的变化范围，为了节约成本，我们选择使用一个小容值的电容来降低高频噪声和大容器电容来稳定电源电压。

    - 接着分析电路：

      ![](23thweek_summarize.assets/image-20240329210136561.png)

      <center><b><font size ='2'>Figure 5 3.3V Regulated Output Circuit</font></b></center></font>

      - VIN_5V：5V电压输入端，输入了一个5V的电压进入AMS1117-3.3
      - AMS1117-3.3：稳压芯片，输出3.3V电压。
      - C18:大容值电容，用于稳定电源电压。
      - C17：小容值电容，用于降低高频噪声。
      - R5和LED1:用作充当电源指示灯。
      - VCC_3V3: 输出口，输出3.3V电压。

  - #### **USB输入模块电路分析：**

    - 该模块主要用于通过Micro输入口提供输入电压。

    - 自恢复保险丝SMD1210P050TF：[Datasheet - LCSC Electronics](https://www.lcsc.com/datasheet/lcsc_datasheet_2208291400_PTTC-Polytronics-Tech-SMD1210P050TF-30_C466600.pdf)

    - U-F-M5DD-Y-1:[Datasheet - LCSC Electronics](https://www.lcsc.com/datasheet/lcsc_datasheet_1811151533_Korean-Hroparts-Elec-U-F-M5DD-Y-1_C91467.pdf)

    - 电路分析：

      - U-F-M5DD-Y-1：Micro USB接口
      - SMD1210P050TF：用于保持电流处于0.05A-2.0A，此处用于0.5A的过流保护。
      - D+和D-：差分信号，用于传输数据，减少共模干扰。
      - C19、C13、C14： 用于滤波。

    - ![image-20240329211000104](23thweek_summarize.assets/image-20240329211000104.png)

      <center><b><font size ='2'>Figure 6 Micro Input Port Circuit</font></b></center></font>

  - #### **晶振部分电路分析：**

    - 左边的8MHz晶振电路：

      - C3 和C4：匹配电容，会稍微影响振荡频率和波形。
      - X2：用于产生8MHz的电信号频率
      - R3：**用于消除谐波和一些滤波**，使反相器在震荡初始时 处于线性工作区。

    - 右边的32.768kHz晶振电路：

      - C2和C1：起振电容
      - X3：用于产生32.768kHz的电信号频率
      - 为什么没有并联电阻，是因为单片机内部已经集成并联了一颗高阻值电阻。

    - ![image-20240330201111265](23thweek_summarize.assets/image-20240330201111265.png)

      <center><b><font size ='2'>Figure 6 External Crystal Oscillator Circuit</font></b></center></font>

  - #### **复位按键和唤醒按键电路分析：**

    - 复位按键：当单片机接收到低电平（NRST引脚）的时候会复位。

    - C20：用作缓冲，防止电压对单片机造成冲击，在接收到3.3V电压信号时电压不会立刻变化而是呈指数增长形式变到3.3V。

    - TSA063G43-250：[TSA063G43-250 BRIGHT | C294564 - LCSC Electronics](https://www.lcsc.com/product-detail/Tactile-Switches_BRIGHT-TSA063G43-250_C294564.html)就是一个普通的按钮，当按下的时候NRST输入端为低电平，此时会发生复位。

    - ![image-20240330202308942](23thweek_summarize.assets/image-20240330202308942.png)

      <center><b><font size ='2'>Figure 7 Reset Button Circuit</font></b></center></font>

    - 唤醒按键：当TSA063G43-250按键按下时，PA0/WAKEUP输出高电平，此时唤醒系统。

    - ![image-20240330202953411](23thweek_summarize.assets/image-20240330202953411.png)

  - #### **MCU主控部分电路分析**：

    - Pin 1 和 Pin 48：连接着4个电容分别是 C10、C11、C8 和 C9，VCC_3V3提供3.3V电压，然后电容用来滤波降低噪音，最后给MCU供电。

    - Pin 9：通过L1、C7和C6实现LC滤波给VDDA提供模拟电源电压。

    - Pin 23 和 Pin24： VSS_1接GND实现接地功能，VDD_1通过两级滤波与3.3V电压相连提供电压，Pin 36和 Pin35同理。

    - Pin 32 和 Pin 33 ：当想让这两个pin口上拉的时候就焊接R14和R15，当想要实现IO口功能的话就焊接R12和R10，当想要实现串口功能就焊接R13 和R11。 

    - ![image-20240330194657650](23thweek_summarize.assets/image-20240330194657650.png)

      <center><b><font size ='2'>Figure 7 MCU Minimum System Circuit</font></b></center></font>
      
    
    