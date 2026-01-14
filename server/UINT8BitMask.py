class UINT8BitMask:
    __BIT_COUNT = 8
    __MASK = 0xFF

    def __init__(self):
        self.bits: int = 0

    def set_bit(self, bit_index: int, state: bool) -> None:
        if not 0 <= bit_index < self.__BIT_COUNT:
            raise IndexError("bit_index must be in range 0..7")

        shift = self.__BIT_COUNT - 1 - bit_index
        mask = 1 << shift

        if state:
            self.bits |= mask
        else:
            self.bits &= ~mask

        self.bits &= self.__MASK

    def get_bit(self, bit_index: int) -> int:
        if not 0 <= bit_index < self.__BIT_COUNT:
            raise IndexError("bit_index must be in range 0..7")

        shift = self.__BIT_COUNT - 1 - bit_index
        return (self.bits >> shift) & 1

    def clear(self) -> None:
        self.bits = 0

    def __int__(self) -> int:
        return self.bits

    def __str__(self) -> str:
        return format(self.bits, "08b")
