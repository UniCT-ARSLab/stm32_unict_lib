Creating an STM32 Project with PlatformIO

1. Select the "PIO Home" tab
2. Click on "New Project"
3. Insert project name
4. As "Board" select "ST NUCLEO F401RE"
5. As "Framework" select CMSIS
6. After project creation copy the dir "stm32_unict_lib" into the "lib" directory
7. Add "ClockConfig()" as the first procedure call in the main() function
   (this sets the CPU clock at 84 MHz)
8. Have fun...


