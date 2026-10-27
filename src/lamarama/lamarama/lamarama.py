import machine
import time
import math
import st7789 # type: ignore

spi = machine.SPI(
    0,
    baudrate=40_000_000,
    polarity=1,
    phase=0,
    sck=machine.Pin(18),
    mosi=machine.Pin(19)
)

print(spi)

display = st7789.ST7789(
    spi,
    240,
    320,
    dc=machine.Pin(16, machine.Pin.OUT),
    cs=machine.Pin(17, machine.Pin.OUT)
)

display.init()

# display.fill(st7789.BLACK)

backlight = machine.PWM(machine.Pin(20))
backlight.freq(1000)
backlight.duty_u16(int(100 * 65535))

r = 0
while(False):
    pct = abs(math.sin(r))
    backlight.duty_u16(int(pct * 65535))
    r += (math.pi / 100)
    if (r > math.pi):
        r = 0
    time.sleep(0.01)


# Load 16-colour RGB565 palette
with open("/assets/monk-palette.bin", "rb") as f:
    raw = f.read()

palette = {
    i: raw[i*2:i*2+2]
    for i in range(16)
}

WIDTH = 120
HEIGHT = 112

# Expand to RGB565
row = bytearray(WIDTH * 4)

for i in range(29):
    # Load packed 4bpp frame
    with open(f"/assets/monk-{i:02d}.bin", "rb") as f:
        for y in range(HEIGHT):
            packed = f.read(WIDTH // 2)

            j = 0
            for b in packed:
                for index in (b >> 4, b & 15):
                    colour = palette[index]
                    row[j:j+2] = colour
                    row[j+2:j+4] = colour
                    j += 4

            display.blit_buffer(row, 0, y * 2, WIDTH * 2, 1)
            display.blit_buffer(row, 0, y * 2 + 1, WIDTH * 2, 1)
