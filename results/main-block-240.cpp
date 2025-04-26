// mkdir -p build && cd build
// rm -rf * && cmake .. && make -j5
// c++ main.cpp build/libsiamese.a  -lpthread -static
// time a.out
// valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./a.out
// kcachegrind &

#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>

#include "siamese.h"

int main() {
        siamese_init();
        
        // Configuration
        const int window_size = 6000;
        const int packet_size = 5 * 1024; // 5K packets
        const int fec_ratio = 6; // 1 in 6 = ~17%
        
        // Block-based processing parameters
        const int block_size = 240; // Process in blocks of 240 packets
        const int num_blocks = window_size / block_size; // Total 25 blocks
        const int fec_per_block = block_size / fec_ratio; // ~40 FEC packets per block
        
        SiameseOriginalPacket packet;
        SiameseRecoveryPacket recovery;
        packet.DataBytes = packet_size;

        // Pre-allocate packet buffers
        auto data = std::vector<std::vector<uint8_t>>();
        for (int i = 0; i < 256; i++) {
                data.push_back(std::vector<uint8_t>(packet_size, i));
        }
        
        // Start timing the entire process
        auto start = std::chrono::high_resolution_clock::now();
        
        int total_fec_generated = 0;
        
        // Process data in blocks for better cache locality
        for (int block = 0; block < num_blocks; block++) {
                // Create a new encoder for each block
                SiameseEncoder encoder = siamese_encoder_create();
                if (!encoder) {
                        std::cerr << "Error creating encoder for block " << block << std::endl;
                        return 1;
                }
                
                // Fill this block with packets
                for (int i = 0; i < block_size; i++) {
                        int packet_idx = block * block_size + i;
                        packet.PacketNum = i; // Each block starts with packet 0
                        packet.Data = data[packet_idx % 256].data();
                        siamese_encoder_add(encoder, &packet);
                }
                
                // Generate FEC for this block
                for (int j = 0; j < fec_per_block; j++) {
                        auto encode_result = siamese_encode(encoder, &recovery);
                        if (encode_result != Siamese_Success) {
                                std::cerr << "Error " << encode_result << " encoding FEC packet " 
                                         << j << " in block " << block << std::endl;
                                siamese_encoder_free(encoder);
                                return encode_result;
                        }
                        total_fec_generated++;
                        
                        // In a real application, you would transmit/store the recovery packet here
                }
                
                // Free this block's encoder
                siamese_encoder_free(encoder);
        }

        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;

        std::cout << "Block-based processing: " << num_blocks << " blocks of " << block_size << " pkts each" << std::endl;
        std::cout << "Total packets: " << window_size << " pkts" << std::endl;
        std::cout << "Generated: " << total_fec_generated << " FEC pkts (17%)" << std::endl;
        std::cout << "Time: " << elapsed.count() << " secs" << std::endl;
        std::cout << "Throughput: " << (total_fec_generated / elapsed.count()) << " FEC pkt/sec" << std::endl;
        std::cout << "Total data throughput: " << ((window_size * packet_size) / (elapsed.count() * 125000)) << " Mbit/s" << std::endl;

        return 0;
}