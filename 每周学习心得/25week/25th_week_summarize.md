#### 25th Week Summarize

- 本周工作目标：

  - 上周我完成了PCB设计图的基本绘制，这周我打算首先完成数据集在Multisim的导入，然后将PCB设计图的电路再Multisim上面模拟实现。
  
- 在所下载的数据集中，有多个变量，其中的Emg变量才是sEMG的信号值，但是它是经过了处理的，其采样速率为200Hz。
  
- ![image-20240418213203629](25th_week_summarize.assets/image-20240418213203629.png)
  
- 由于sEMG的数据有被处理，并且不适合Multisim的输入格式（时间 电压），所以放弃了这个数据集，重新找到了一个新的数据集：Examples of Electromyograms，并且通过PWL Voltage组件成功实现了在Multisim的模拟输入信号。

  - 链接地址：[肌电图示例 v1.0.0 --- Examples of Electromyograms v1.0.0 (physionet.org)](https://physionet.org/content/emgdb/1.0.0/)

  ![image-20240418230339730](25th_week_summarize.assets/image-20240418230339730.png)

- 在研究如何实现放大电路时，我发现了一个有关EMG的文献综述：[肌电信号采集与处理综述——PMC --- Review on electromyography signal acquisition and processing - PMC (nih.gov)](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7755956/)

  - 通过阅读这篇论文，我知道了要设计出来低成本EMG传感器，我们除了需要设计放大电路，还需要设计滤波器和整流器，但是我们的计划是通过原始信号传输给电脑然后使用Matlab进行处理，那么主要任务可能就是如何使用matlab对信号进行滤波，与设计PCB电路的任务并列。
  
    > - For portable EMG systems, MyoWare is the best sensor available in the market. For accurate results, MyoWare sensors are preferred over low-cost sensors. Filters and rectifiers need to be added before amplification if MyoWare is not used.
  
- 根据MyoWare1.0的电路图，我们想要得到Raw EMG output，只需要实现instrumentation amplifier就可以了

  - ![image-20240218195312806](25th_week_summarize.assets/image-20240218195312806.png)

- 按照我的PCB电路设计，我计划用4通道的AD8619运放来驱动4个AD8235单通道低功耗仪器放大器，但是后来发现matlab也可以实现像AD8619的缓冲器的功能，所以只需要AD8235来实现放大即可，但是AD8235是单通道的仪表放大器，所以考虑重新选择适合的多通道仪表放大器。

- 

