## The 22th Week Summarize

#### Differential amplifier selection : AD8619

<img src="../15week/Muscle%20Sensor.assets/image-20240204184616113.png" alt="image-20240204184616113" style="zoom:33%;" />

- Due to the various requirements for EMG electromyographic signals, AD8619 was still chosen as the differential amplifier after comparison, for the following reasons:

  - **8通道放大器缺乏**：当前市面上并没有太多的8通道rail to rail放大器，所以只能选择4通道rail to rail 放大器。

    - <img src="21week_Summarize.assets/image-20240325165225396.png" alt="image-20240325165225396" style="zoom:50%;" />

  - **成熟产品的选择**：MyoWare2.0的差分放大器也是AD8619的，所以选择这个放大器肯定不会有太大差错。

    - <img src="../15week/Muscle%20Sensor.assets/image-20240204184405014.png" alt="image-20240204184405014" style="zoom: 25%;" />

  - **供电电压**：放大器工作电压在1.8V - 5V ，如应用手册所示。

    > - The parts are fully specified to operate from 1.8 V to 5 V single supply, or ±0.9 V and ±2.5 V dual supply. The combination of low noise, very low input bias currents, and low power consumption make the AD8613/AD8617/AD8619 especially useful in portable and loop-powered instrumentation

  - **增益带宽积**：确保所选放大器满足应用的频率响应需求。

  - **输入和输出特性**：输入偏置电流极低，如下图所示。

    - <img src="21week_Summarize.assets/image-20240325184907008.png" alt="image-20240325184907008" style="zoom:50%;" />

  - **温度范围**：25°C左右的室温环境下可以正常工作，误差较小。

    - <img src="21week_Summarize.assets/image-20240325185008705.png" alt="image-20240325185008705" style="zoom:50%;" />

- 如果需要测量8个或以上的肌肉群，那么我们可以多加一个AD8619放大器来测量肌电信号。

- 在了解了差分放大器之后，我在查资料过程中对工频信号有了一个初步的了解，工频信号在英国是60Hz，然而正常人体表面肌电信号赋值为0~1.5mV，主要能量频段集中在10~150Hz。它就会包含工频信号的干扰。此时差分放大器可以通过同时放大两个输入端之间的电压差来同时抑制两个输入端共同的信号（共模信号）。使用差分放大器可以在一定程度上减少这种类型的干扰，但是否能够完全消除这些误差取决于电路的设计是否科学，所以我们要开始学习设计电路。

- 首先需要开始学习设计最小MCU电路，由于之前没有对这一领域有涉猎，所以先对一个最经典的最小MCU电路进行学习：
  
  - PCB电路图原理学习笔记如下图所示：
    - ![PCB Design](21week_Summarize.assets/PCB%20Design.png)
  - 我根据已有的电路图，使用EDA软件仿制了一张一摸一样的图来提升自己的EDA软件使用熟练度，并且通过这个创建一个属于自己的元件库，如下图所示：
    - 这是我仿制的电路图：![SCH_Schematic1_1-P1_2024-03-25](21week_Summarize.assets/SCH_Schematic1_1-P1_2024-03-25.png)
    - 这是我自己创建的元件库：![image-20240325225423493](21week_Summarize.assets/image-20240325225423493.png)
    - EDA软件界面：![image-20240325225758659](21week_Summarize.assets/image-20240325225758659.png)
  - 我绘画的电路图通过了DRC检查，除了没有为元件添加封装的报错，其余正常。
    - ![image-20240325225902319](21week_Summarize.assets/image-20240325225902319.png)
  
- 下周计划：

  - 完善自己的元件库，为元件添加封装
  - 结合放大器与MCU最小系统，设计出具有ADC功能的MCU+WiFi模块+AD8619的电路图