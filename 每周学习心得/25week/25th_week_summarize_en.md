### 25th Week Summarize

- **Work Objectives for the Week:**

  - Last week, I completed the basic drawing of the PCB design. This week, my plan is to first import the dataset into Multisim and then simulate the circuit of the PCB design on Multisim.

- In the downloaded dataset, there are multiple variables, where only the Emg variable represents the signal values of sEMG. However, it has undergone processing, and its sampling rate is 200Hz.

- ![image-20240418213203629](25th_week_summarize.assets/image-20240418213203629.png)

- As the sEMG data has been processed and is not suitable for Multisim's input format (time-voltage), I decided to abandon this dataset. Instead, I found a new dataset: Examples of Electromyograms and successfully implemented the simulated input signal in Multisim using the PWL Voltage component.

  - Link: [EMG Examples v1.0.0 --- Examples of Electromyograms v1.0.0 (physionet.org)](https://physionet.org/content/emgdb/1.0.0/)

  ![image-20240418230339730](25th_week_summarize.assets/image-20240418230339730.png)

- While researching how to implement an amplification circuit, I came across a literature review related to EMG: [Review on electromyography signal acquisition and processing - PMC (nih.gov)](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7755956/)

  - Through this paper, I learned that to design a low-cost EMG sensor, in addition to designing an amplification circuit, filters and rectifiers need to be implemented. As our plan involves transferring the raw signal to the computer for processing using Matlab, a primary task would be filtering the signal in Matlab alongside the PCB circuit design task.

    > - For portable EMG systems, MyoWare is the best sensor available in the market. For accurate results, MyoWare sensors are preferred over low-cost sensors. Filters and rectifiers need to be added before amplification if MyoWare is not used.

- Referring to the circuit diagram of MyoWare 1.0, to obtain Raw EMG output, all we need is to implement an instrumentation amplifier followed by an ADC for measurement and transmission. If connected to a Raspberry Pi, there might be no need for PCB design.

  - ![image-20240218195312806](25th_week_summarize.assets/image-20240218195312806.png)

- According to my PCB circuit design, I initially planned to use 4-channel AD8619 operational amplifiers to drive 4 AD8235 single-channel low-power instrumentation amplifiers. However, I later discovered that Matlab can also serve the function of a buffer similar to AD8619, so utilizing only AD8235 for amplification may suffice. Considering that AD8235 is a single-channel instrumentation amplifier, I am contemplating on selecting a more suitable multi-channel instrumentation amplifier.

  - AD8232: [AD8232 Datasheet and Product Info | Analog Devices](https://www.analog.com/en/products/ad8232.html). Through my study, I found that compared to AD8235, although both are biopotential amplifier chips, AD8232 has lower power consumption, higher gain, and differential output. I believe AD8232 could be a suitable choice, as in the future, this device could potentially be combined with energy-efficient portable battery-powered equipment to make EMG measurements more convenient.

- Following a conversation with my supervisor last week, knowing that there is no need for additional PCB design, my focus now is to delve deeper into understanding instrumentation amplifiers and ADC-related devices. By studying [Basic Linear Design - Analog Devices](https://www.analog.com/media/en/training-seminars/design-handbooks/Basic-Linear-Design/Chapter2.pdf), I learned that instrumentation amplifiers need to amplify signals at microvolt levels and suppress common-mode signal voltages at the input, requiring the amplifier to have very high common-mode rejection. However, besides this, during the development process, noise will inevitably be present, with the most common culprit being 50Hz/60Hz AC power-related noise (including harmonics).

- Regarding the Raspberry Pi, for the sake of portability, I have been considering using the Raspberry Pi as a PC-like device for processing models and displaying them on screen, as the Raspberry Pi can run a Linux system and Matlab on that system for data analysis. Additionally, I plan to use an STM32-based microcontroller development board to measure the raw signal and send it to the Raspberry Pi via a WiFi module, which is my idea.

- **Next Week's Plans:**

  - Conduct a more detailed comparison between AD8232 and AD8235, select a more suitable ADC device, and proceed with procurement.
  - Determine the type of data interface to be used for data acquisition and study the SPI protocol.
  - Decide whether the Raspberry Pi directly connects to the ADC and instrumentation amplifier for EMG measurement or if the Raspberry Pi receives the signal sent by STM32 via WiFi.

