// Handle multiples of 256 bytes
        while (bytes >= 256)
        {
            // Prefetch next iteration (256 bytes ahead)
            __builtin_prefetch((const void *)(y + 256), 0, 3); // read
            __builtin_prefetch((void *)(x + 256), 1, 3);       // write

            // Load 4x4 = 16 vectors = 256 bytes
            uint8x16x4_t x0 = vld1q_u8_x4(x); // x[0..63]
            uint8x16x4_t y0 = vld1q_u8_x4(y);
            uint8x16x4_t x1 = vld1q_u8_x4(x + 64); // x[64..127]
            uint8x16x4_t y1 = vld1q_u8_x4(y + 64);
            uint8x16x4_t x2 = vld1q_u8_x4(x + 128); // x[128..191]
            uint8x16x4_t y2 = vld1q_u8_x4(y + 128);
            uint8x16x4_t x3 = vld1q_u8_x4(x + 192); // x[192..255]
            uint8x16x4_t y3 = vld1q_u8_x4(y + 192);

            // XOR in-place
            for (int i = 0; i < 4; ++i)
            {
                x0.val[i] = veorq_u8(x0.val[i], y0.val[i]);
                x1.val[i] = veorq_u8(x1.val[i], y1.val[i]);
                x2.val[i] = veorq_u8(x2.val[i], y2.val[i]);
                x3.val[i] = veorq_u8(x3.val[i], y3.val[i]);
            }

            // Store results
            vst1q_u8_x4(x, x0);
            vst1q_u8_x4(x + 64, x1);
            vst1q_u8_x4(x + 128, x2);
            vst1q_u8_x4(x + 192, x3);

            x += 256;
            y += 256;
            bytes -= 256;
        }
