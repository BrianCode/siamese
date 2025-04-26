#include <arm_neon.h>
#include <stdint.h>

extern "C" void gf256_add_mem_a53_opt(void * GF256_RESTRICT vx,
                                      const void * GF256_RESTRICT vy, int bytes)
{
    uint8_t* x = reinterpret_cast<uint8_t*>(vx);
    const uint8_t* y = reinterpret_cast<const uint8_t*>(vy);

    while (bytes >= 128)
    {
        // Prefetch 2 iterations ahead to hide latency
        __builtin_prefetch(y + 256, 0, 3);
        __builtin_prefetch(x + 256, 1, 3);

        // Load 8x16B = 128B
        uint8x16_t x0 = vld1q_u8(x + 0);
        uint8x16_t y0 = vld1q_u8(y + 0);
        uint8x16_t x1 = vld1q_u8(x + 16);
        uint8x16_t y1 = vld1q_u8(y + 16);
        uint8x16_t x2 = vld1q_u8(x + 32);
        uint8x16_t y2 = vld1q_u8(y + 32);
        uint8x16_t x3 = vld1q_u8(x + 48);
        uint8x16_t y3 = vld1q_u8(y + 48);
        uint8x16_t x4 = vld1q_u8(x + 64);
        uint8x16_t y4 = vld1q_u8(y + 64);
        uint8x16_t x5 = vld1q_u8(x + 80);
        uint8x16_t y5 = vld1q_u8(y + 80);
        uint8x16_t x6 = vld1q_u8(x + 96);
        uint8x16_t y6 = vld1q_u8(y + 96);
        uint8x16_t x7 = vld1q_u8(x + 112);
        uint8x16_t y7 = vld1q_u8(y + 112);

        // XOR
        x0 = veorq_u8(x0, y0);
        x1 = veorq_u8(x1, y1);
        x2 = veorq_u8(x2, y2);
        x3 = veorq_u8(x3, y3);
        x4 = veorq_u8(x4, y4);
        x5 = veorq_u8(x5, y5);
        x6 = veorq_u8(x6, y6);
        x7 = veorq_u8(x7, y7);

        // Store
        vst1q_u8(x + 0,    x0);
        vst1q_u8(x + 16,   x1);
        vst1q_u8(x + 32,   x2);
        vst1q_u8(x + 48,   x3);
        vst1q_u8(x + 64,   x4);
        vst1q_u8(x + 80,   x5);
        vst1q_u8(x + 96,   x6);
        vst1q_u8(x + 112,  x7);

        x += 128;
        y += 128;
        bytes -= 128;
    }

    // You can follow this with 64B/16B/scalar tail logic
}

