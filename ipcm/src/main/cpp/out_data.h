//
// Created by nebula on 2019-06-17.
//

#ifndef IPCM_OUT_DATA_H
#define IPCM_OUT_DATA_H

#include "logger.h"
#include <cstdint>
#include <string>

class SinkData {
    uint8_t *m_ptr;
    size_t m_size;
    int32_t m_pos;
public:
    SinkData(void *ptr, size_t size);

    ~SinkData();

    void write_raw_byte(uint8_t data);

    void write_byte(bool data);

    void write_int32(int32_t data);

    void write_varint_32(int32_t);

    void write_varint_64(int64_t);
};

static std::string *Byte2Hex(uint8_t *bArray, int bArray_len) {
    std::string *strHex = new std::string();
    int nIndex = 0;
    for (int i = 0; i < bArray_len; i++) {
        int high = bArray[0] / 16, low = bArray[0] % 16;
        strHex[nIndex] = (high < 10) ? ('0' + high) : ('A' + high - 10);
        strHex[nIndex + 1] = (low < 10) ? ('0' + low) : ('A' + low - 10);
        nIndex += 2;
    }
    return strHex;
}

static void test() {
    uint8_t values[100] = {0};
//    char string[200];
    SinkData data(&values, 100);

//    std::string *string = Byte2Hex(values, 100);
    LOGE("%s", (char*)values);

    data.write_int32(12);

    LOGE("%s", (char*)values);
//    LOGE("1~20:%s", info.c_str());
//    string = Byte2Hex(reinterpret_cast<unsigned int *>(values), 100);
//    LOGE("%s", string->c_str());
}

#endif //IPCM_OUT_DATA_H
