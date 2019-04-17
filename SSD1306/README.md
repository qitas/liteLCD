# OLED4PI
https://github.com/vadzimyatskevich/SSD1306

| OLED |    Name   |  Pin  |
|:----:|:---------:|:-----:|
|`VDD` |    3.3v   |  1    |
|`SDA` |   SDA.0   |  3    |
|`SCK` |   SCL.0   |  5    |
|`GND` |    0v     |  9    |

To run app:

1. `git clone https://github.com/Qitas/LCD4Pi`
1. `cd /<path to project>/SSD1306`
2. `make`
3. `sudo ./build/OrangePI_ssd1306`

To run app at boot time:

1. `sudo crontab -e`
2. add row: `@reboot /<path to project>/build/OrangePI_ssd1306`
3. reboot board
