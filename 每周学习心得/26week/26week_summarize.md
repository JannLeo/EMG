#### 26th Week Summarize

- 上周总结：

  - 上周完成了multisim的EMG信号模拟，并且对仪表放大器进行了选型，根据老师的建议最后确定是AD8232。

- 这周工作计划

  - 这周打算选择出最适合的ADC部件,优先考虑AD7771，并且研究怎么使用AD8232.

- #### AD7771资料

  - 链接：[AD7771 数据表和产品信息 |模拟器件公司 --- AD7771 Datasheet and Product Info | Analog Devices](https://www.analog.com/en/products/ad7771.html)

  - AD7771虽然适合EMG的测量，并且是8通道仪器，但是由于他是24位ADC，精度有点高，因为表面肌电图sEMG的范围是在50μV-5mV之间，所以按照5mV来计算单位分辨率可以算出：

    - 对于AD7771作为24位ADC,由于SNR = 107dB，可以算出有效位数为：

      

      - $$
        ENOB =  \frac{SNR - 1.76}{6.02} = \frac{107-1.76}{6.02} = 16.96 ≈ 17 bit
        $$

      - 其中：

        - **1.76**：是一个常数，用于调整理想信号的dB值。
        - **6.02**：是将dB转换为位数的因子（20log10(2) ≈ 6.02）

      - 接下来根据实际有效位数计算分辨率

      - $$
        5mV / 2^{17} = 0.005 / 131072 ≈ 38.147 nV
        $$

    - 对于 16 位ADC：同理大概估算出有效位数为15位

      - $$
      ENOB =  \frac{SNR - 1.76}{6.02} = \frac{95.5-1.76}{6.02} = 15.55 ≈ 15 bit
        $$
  
        
      
      - $$
        5mV / 2^{15} = 0.005 / 32768 ≈ 152.588 nV
        $$
      
    - 根据以上可以看出，16位的ADC对于测量EMG来说已经足够精度，因为EMG信号最小幅度为50μV，远远大于152.588nV，并且如果使用24位的ADC的话，除了可能会导致成本上升等问题外，还可能会造成放大噪音的问题，这会给降噪等步骤带来许多难题。

| 类型               |                            AD7606                            |                            AD7771                            |
| ------------------ | :----------------------------------------------------------: | :----------------------------------------------------------: |
| **位数**           |                             16位                             |                             24位                             |
| **有效位数**       |                             15位                             |                             17位                             |
| **SNR**            |                            95.5dB                            |                            107dB                             |
| **分辨率**         |                          152.588nV                           |                           38.147nV                           |
| **采样率**         |                           200 kSPS                           |                           128 kSPS                           |
| **输入范围**       |                          2.3V - 5V                           |                         1.8V - 3.6V                          |
| **功耗**           |              低功耗模式100mW<br />待机模式 25mW              |   高分辨率模式每通道16.6 mW <br />低功耗模式每通道 5.25mW    |
| **接口类型**       |             SPI/QSPI™/MICROWIRE™/DSP compatible              |       Data output interface and SPI control interface        |
| **抗混叠滤波器**   |                     二阶抗混叠模拟滤波器                     |                              无                              |
| **多通道同步采样** |                             可以                             |                             可以                             |
| **精度**           |               典型精度为±4 LSB，最大为±16 LSB                |                典型精度为±2 LSB，最大为±8 LSB                |
| **线性度**         | 典型DNL为±0.5 LSB，最大为±1 LSB <br />典型INL为±1 LSB，最大为±2 LSB | 典型DNL为±0.5 LSB，最大为±1 LSB<br />典型INL为±1 LSB，最大为±2 LSB |
| **成本**           | $49.34<br />[AD7606C-16BSTZ ADI 公司\| AD7606C-16BSTZ集成电路 (IC) \|得捷电子 --- AD7606C-16BSTZ Analog Devices Inc. \| Integrated Circuits (ICs) \| DigiKey](https://www.digikey.com/en/products/detail/analog-devices-inc/AD7606C-16BSTZ/13918940) | $22.88<br />[AD7771BCPZ Analog Devices Inc. \| Integrated Circuits (ICs) \| DigiKey](https://www.digikey.com/en/products/detail/analog-devices-inc/AD7771BCPZ/7649198) |

- 通过上述比较，最后发现24位的AD7771甚至成本比16位的AD7606还低，并且AD7771相比之下有分辨率高、功耗低等优点，虽然没有抗混叠滤波器，但是我们仍然可以用matlab来进行后期噪音消除和滤波。

