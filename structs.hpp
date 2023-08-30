#ifndef A2C_STRUCTS_HPP
#define A2C_STRUCTS_HPP

#include <cstdint>


namespace a2c
{
    struct LOCAL_PLAYER
    {
        char pad_0000[28]; //0x0000

        float AccelX; //0x001C
        float AccelY; //0x0020
        float AccelZ; //0x0024

        float PosX; //0x0028
        float PosY; //0x002C
        float PosZ; //0x0030

        float ViewYaw; //0x0034
        float ViewPitch; //0x0038
        float ViewRoll; //0x003C

        char pad_0040[172]; //0x0040

        int32_t Health; //0x00EC
    }; //Size: 0x0440
}


#endif //A2C_STRUCTS_HPP
