from typing import Literal


class UINT8BitMask:
    __BIT_COUNT= 8
    def __init__(self):
        self.bits:int = 0x00000000

    def setBit(self, bit_index:int, state: int):
        if state:
            self.bits |= 1 << (UINT8BitMask.__BIT_COUNT - 1 - bit_index)
        else:
            self.bits &= ~(1 << (UINT8BitMask.__BIT_COUNT - bit_index))

        self._validate_state()
    def _validate_state(self):
        # ensure only first 8 bits are set
        self.bits &= 0xFF
    def __str__(self):
        return format(self.bits, "08b")

mask = UINT8BitMask()
mask.setBit(3,1)
print(mask)
mask.setBit(0,1)
print(mask)
