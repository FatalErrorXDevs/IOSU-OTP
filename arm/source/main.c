#include "types.h"
#include "utils.h"

#define FRAMEBUFFER_ADDRESS (0x14000000+0x38C0000)

extern void IOS_DCFlushAllCache(unsigned int addr);

void clearScreen(u32 color)
{
    u32* framebuffer = (u32*)FRAMEBUFFER_ADDRESS;
    int i;
    for(i = 0; i < 896 * 504; i++)
    {
        framebuffer[i] = color;
        asm volatile("MRC p15, 0, %0, c7, c14, 3" : : "r" (&framebuffer[i]));
    }
}

static const u32 repairData_IOS_GetUpTime64[] = {
    0xE59F3040, 0xE92D4010, 0xE593C000, 0xE24DD004,
    0xE59C3054, 0xE3A01008, 0xE3A02004, 0xE58D3000,
    0xE1A04000, 0xEBFFE9E6, 0xE3500000, 0x0A000001,
    0xE28DD004, 0xE8BD8010, 0xE1A00004, 0xE28DD004
};

void _main()
{
/*
    void (*ios_shutdown)(int) = (void (*)(int))0xFFFFDC48;
    void (*ios_dflush)(void *ptr, unsigned int len) = (void (*)(void *, unsigned int))0x081200F0;

    void * pIOS_GetUpTime64 = (void*)0x0812A314;
    m_memcpy(pIOS_GetUpTime64, repairData_IOS_GetUpTime64, sizeof(repairData_IOS_GetUpTime64));
    ios_dflush(pIOS_GetUpTime64, sizeof(repairData_IOS_GetUpTime64));
*/
    unsigned char test_buf[0x400];
    int (*read_otp_internal)(int index, void* out_buf, u32 size) = (int (*)(int, void*, u32)) 0x08120248;

    read_otp_internal(0, test_buf, sizeof(test_buf));

    while(1)
    {
        m_memcpy((void*)0x0012F000, test_buf, sizeof(test_buf));
        IOS_DCFlushAllCache(0x0012F000);
    }

    //ios_shutdown(1);

    /*
    // this is actually not executed anymore, move up if needed but it does not change the screen
    clearScreen(0xFF0000FF);

    unsigned int i = 0x3ffff;
    while(--i)
    {
        asm volatile ("nop");
    }
    */
}
