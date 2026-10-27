## Hardware

| Peripheral     | Hardware handling  | CPU involvement                 |
| -------------- | ------------------ | ------------------------------- |
| MCP4821 DAC    | PIO + DMA          | Fill next audio buffer          |
| MCP3008 ADC    | PIO + DMA          | Read latest control values      |
| LCD Hardware   | SPI + DMA          | Prepare changed display regions |
| Pitch CV       | Internal ADC + DMA | Process sampled voltage         |
| Gate input     | GPIO interrupt     | Handle gate transition          |
| Rotary encoder | PIO                | Read accumulated movement       |
| Buttons        | GPIO               | Poll at UI refresh rate         |

## Pico 2 GPIO Allocation

| GPIO        | Assignment          | Peripheral     |
| ----------- | ------------------- | -------------- |
| GP0         | Unused              | —              |
| GP1         | LCD CS              | SPI0           |
| GP2         | LCD SCK             | SPI0           |
| GP3         | LCD MOSI            | SPI0           |
| GP4         | LCD D/C             | LCD            |
| GP5         | LCD RESET           | LCD            |
| GP6         | LCD backlight PWM   | LCD            |
| GP7         | ADC CS              | MCP3008 PIO    |
| GP8         | ADC SCK             | MCP3008 PIO    |
| GP9         | ADC MOSI            | MCP3008 PIO    |
| GP10        | ADC MISO            | MCP3008 PIO    |
| GP11        | DAC CS              | MCP4821 PIO    |
| GP12        | DAC SCK             | MCP4821 PIO    |
| GP13        | DAC MOSI            | MCP4821 PIO    |
| GP14        | DAC LDAC (optional) | MCP4821        |
| GP15        | Gate input          | GPIO interrupt |
| GP16        | Encoder A           | PIO            |
| GP17        | Encoder B           | PIO            |
| GP18        | Encoder push        | GPIO           |
| GP19        | Back/Cancel         | GPIO           |
| GP20        | Spare               | —              |
| GP21        | Spare (MIDI IN)     | —              |
| GP22        | Spare               | —              |
| GP26 / ADC0 | Pitch CV            | Internal ADC   |
| GP27 / ADC1 | Spare analog        | —              |
| GP28 / ADC2 | Spare analog        | —              |

### Other Connections

| Pico 2 Pin                 | Assignment                   |
| -------------------------- | ---------------------------- |
| ADC_VREF (physical pin 35) | LM4040C30ILPR 3.0V reference |
| AGND (physical pin 33)     | Analog ground                |