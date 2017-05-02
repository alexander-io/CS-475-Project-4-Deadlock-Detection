from bitstring import BitArray

# W 0x00000000
# R 0x00000400
# W 0x00000800
# R 0x00000C00
# W 0x00001000

lst = ['0x00000000','0x00000400','0x00000800','0x00000C00','0x00001000','0x00001400','0x00001800','0x00001C00','0x00002000','0x00002400','0x00002800','0x00002C00','0x00003000','0x00003400','0x00003800','0x00003C00']

def hexToBitArray(lst):
    for x in lst:
        b = BitArray(hex=x)
        print(x, ':', b.bin)

hexToBitArray(lst)
