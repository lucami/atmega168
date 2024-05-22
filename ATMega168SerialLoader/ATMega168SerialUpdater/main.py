import time

from serial import Serial


def openfile():
    return open("blink.bin", mode="rb")


def serial_open():
    s = Serial("/dev/cu.SLAB_USBtoUART")
    s.baudrate = 9600
    return s


def serial_comm(s):
    while 1:
        sentence = s.readline()
        if b"Boot" in sentence:
            print(f"Received bootloader string - {sentence}")
            return 1


if __name__ == '__main__':

    hex_file = []
    f = openfile()
    readed = f.read()


    s = serial_open()
    serial_comm(s)

    size = len(readed)  # 164 #0x00A4
    print(f"Filesize: {size}")

    size_byte = int.to_bytes(size, 2, "little")
    s.write(size_byte)

    print(f"Sending filesize to Bootloader {size} {size_byte}")
    file_size_echo = s.readline()
    print(f"Filesize Echo {file_size_echo} {int.from_bytes(file_size_echo[:-2], 'little')}")


    page_len_hex = s.readline()
    page_len=int.from_bytes(page_len_hex[:-2], 'little')
    print(f"Bootloader page length: {page_len}  {page_len_hex}")

    time.sleep(0.1)
    print("Sending file")
    i=0
    for b in readed:
        print(f"{i} Sending {int.to_bytes(b,1,'little')} ({(type(int.to_bytes(b,1,'little')))} len: {len(int.to_bytes(i,1,'little'))})  {s.write(int.to_bytes(b,1,'little'))} ", end=" ")
        #print(f"{i} Sending {i} ({(hex(i))})  {s.write(int.to_bytes(i,1,'little'))} ", end=" ")
        time.sleep(0.01)
        echo=s.read(3)

        print(f"Echo: {hex(int.from_bytes(echo[:-2],'little'))} {echo} {b==int.from_bytes(echo[:-2],'little')}")
        i+=1