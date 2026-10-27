#!/usr/bin/env python3

import sys
import struct
import zlib
from pathlib import Path

from PIL import Image # type: ignore

EXPECTED_SIZE = 3719168
EXPECTED_CRC32 = 0x9B0E96C2

# offset, filename, width, height, bpp, transparency
RESOURCES = [
    (0x00E1B0, "background.png",        360,  510,  8, "opaque"),
    (0x03B310, "monk-strip.png",       1570, 1866,  8, "opaque"),
    (0x3079A8, "fader-right-sm.png",     10,   10, 24, "white"),
    (0x307B18, "fader-down-sm.png",      10,   10, 24, "white"),
    (0x307C88, "fader-down-large.png",   20,   17, 24, "white"),
    (0x3080B0, "knob-left.png",           50, 3000,  8, "topleft"),
    (0x32E640, "knob-right.png",          50, 3000,  8, "topleft"),
    (0x354BD0, "info.png",               253,  275, 24, "opaque"),
]


def dump_py(im: Image, path, colors):
    with open(path, "w") as f:
        f.write("monk_00 = bytes([\n")

        for y in range(im.height):
            f.write("    ")

            for x in range(im.width):
                r, g, b = im.getpixel((x, y))

                rgb565 = ((r >> 3) << 11) | \
                        ((g >> 2) << 5)  | \
                        (b >> 3)

                colors.append(rgb565)

                f.write(f"0x{rgb565 >> 8:02x},0x{rgb565 & 0xff:02x},")

            f.write("\n")

        f.write("])\n")


def extract_dib(data, offset, width, height, bpp):
    dib = memoryview(data)[offset:]

    header_size = struct.unpack_from("<I", dib, 0)[0]
    if header_size != 40:
        raise ValueError(
            f"Expected BITMAPINFOHEADER at 0x{offset:X}, got size {header_size}"
        )

    raw_height = struct.unpack_from("<i", dib, 8)[0]
    bottom_up = raw_height > 0

    # Number of palette entries (biClrUsed)
    if bpp <= 8:
        clr_used = struct.unpack_from("<I", dib, 32)[0]
        palette_entries = clr_used if clr_used else (1 << bpp)
    else:
        palette_entries = 0

    palette_offset = 40
    pixel_offset = palette_offset + palette_entries * 4

    # DIB scanlines are padded to 32-bit boundaries.
    row_bytes = ((width * bpp + 31) // 32) * 4

    image = Image.new("RGBA", (width, height))
    out = image.load()

    for y in range(height):
        src_y = height - 1 - y if bottom_up else y
        row = pixel_offset + src_y * row_bytes

        for x in range(width):

            if bpp == 24:
                p = row + x * 3
                b = dib[p]
                g = dib[p + 1]
                r = dib[p + 2]

            elif bpp == 8:
                index = dib[row + x]
                p = palette_offset + index * 4

                b = dib[p]
                g = dib[p + 1]
                r = dib[p + 2]

            else:
                raise ValueError(f"Unsupported bit depth: {bpp}")

            out[x, y] = (r, g, b, 255)

    return image


def apply_transparency(image, mode):
    if mode == "opaque":
        return

    pixels = image.load()
    width, height = image.size

    if mode == "white":
        key = (255, 255, 255)
    elif mode == "topleft":
        key = pixels[0, 0][:3]
    else:
        raise ValueError(f"Unknown transparency mode: {mode}")

    for y in range(height):
        for x in range(width):
            if pixels[x, y][:3] == key:
                pixels[x, y] = (0, 0, 0, 0)

def save_frame(frame, path):
    width, height = frame.size

    arr = []

    for y in range(height):
        for x in range(0, width, 2):
            a = frame.getpixel((x,y))
            b = frame.getpixel((x+1,y))
            arr.append(a << 4 | b)

    with open(path, "wb") as f:
        f.write(bytes(arr))

def split_monk_strip(image_path, output_dir):
    image = Image.open(image_path).convert("RGB")
    
    cols = 5
    rows = 6

    frame_width = image.width // cols      # 314
    frame_height = image.height // rows    # 311
    cropped_height = 290;
    resize_width = 120;
    resize_height = 112;

    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    n = 0

    processed_frames = []

    for y in range(rows):
        for x in range(cols):
            frame = image.crop((
                x * frame_width,
                y * frame_height,
                (x + 1) * frame_width,
                (y + 1) * frame_height,
            ))

            frame = frame.crop((0, 0, frame_width, cropped_height));

            frame = frame.resize((resize_width, resize_height), Image.Resampling.LANCZOS)

            frame.save(output_dir / f"monk-{n:02d}.png")

            processed_frames.append(frame)
            n += 1

    print(f"Extracted {n} Lama frames ({frame_width}x{frame_height})")

    strip = Image.new(
        "RGB",
        (120 * len(processed_frames), 112)
    )

    for i, frame in enumerate(processed_frames):
        strip.paste(frame, (i * 120, 0))

    strip.save(output_dir / f"monk-strip-raw.png")

    strip = strip.quantize(colors=16)

    strip.save(output_dir / f"monk-strip.png")

    palette = strip.getpalette()

    with open(output_dir / "monk-palette.bin", "wb") as f:
        for i in range(16):
            r, g, b = palette[i*3:i*3+3]

            rgb565 = ((r >> 3) << 11) | \
                     ((g >> 2) << 5)  | \
                     (b >> 3)

            f.write(rgb565.to_bytes(2, "big"))

    for i in range(len(processed_frames)):
        f = strip.crop((
            i * 120,
            0,
            (i + 1) * 120,
            112,
        ))
        
        save_frame(f, output_dir / f"monk-{i:02d}.bin")

def main():
    if len(sys.argv) not in (2, 3):
        print(f"Usage: {sys.argv[0]} DelayLama.dll [output-directory]")
        sys.exit(1)

    dll_path = Path(sys.argv[1])

    if len(sys.argv) == 3:
        output_dir = Path(sys.argv[2])
    else:
        output_dir = Path("classic")

    data = dll_path.read_bytes()

    if len(data) != EXPECTED_SIZE:
        raise SystemExit(
            f"Wrong DLL size: {len(data)} bytes "
            f"(expected {EXPECTED_SIZE})"
        )

    crc = zlib.crc32(data) & 0xFFFFFFFF

    if crc != EXPECTED_CRC32:
        raise SystemExit(
            f"Wrong DLL CRC32: {crc:08X} "
            f"(expected {EXPECTED_CRC32:08X})"
        )

    print(f"DLL OK: {len(data)} bytes, CRC32 {crc:08X}")

    output_dir.mkdir(parents=True, exist_ok=True)

    for offset, filename, width, height, bpp, transparency in RESOURCES:

        print(
            f"{filename}: "
            f"offset=0x{offset:X}, "
            f"{width}x{height}, {bpp} bpp"
        )

        image = extract_dib(
            data,
            offset,
            width,
            height,
            bpp,
        )

        apply_transparency(image, transparency)

        output_path = output_dir / filename
        image.save(output_path)

    print(f"\nExtracted {len(RESOURCES)} images to {output_dir}")

    split_monk_strip( output_dir / "monk-strip.png", output_dir / "monk-frames")

if __name__ == "__main__":
    main()