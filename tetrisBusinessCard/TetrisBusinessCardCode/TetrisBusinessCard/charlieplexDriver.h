#ifndef DRIVER_H
#define DRIVER_H
#include <vector>

namespace driver
{
    class charlieplexDriver
    {
        public:
            void init();
            int writeFrame(std::vector<uint32_t> frame);
            uint32_t expandRow(uint32_t row);
    };
}

#endif