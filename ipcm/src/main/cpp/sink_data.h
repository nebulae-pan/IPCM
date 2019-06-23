//
// Created by nebula on 2019-06-17.
//

#ifndef IPCM_SINK_DATA_H
#define IPCM_SINK_DATA_H

#include "logger.h"
#include "source_data.h"
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

    void write_bool(bool data);

    void write_int32(int32_t data);

    void write_varint_32(int32_t);

    void write_varint_64(int64_t);
};

static std::string *int_to_bit(int64_t value, uint8_t size) {
    std::string *result = new std::string();
    int i = 0;
    while (i < size) {
        if ((value & 1) == 1) {
            result->insert(0, "1");
        } else {
            result->insert(0, "0");
        }
        i++;
        value >>= 1;
    }
    return result;
}

static char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
                       'F'};

static std::string *int8_to_hex_string(int8_t value) {
    std::string *result = new std::string();
    auto v = static_cast<uint8_t>(value);
    (*result) += hex[v >> 4];
    (*result) += hex[v & 0x0f];
    return result;
}

static const char *int8_array_to_string(uint8_t *array, int size) {
    std::string *res = new std::string();
    for (int i = 0; i < size; i++) {
        res->append(*int_to_bit(array[i], 8));
        *res += " ";
    }
    return res->c_str();
}

static const char *int8_array_to_hex_string(uint8_t *array, int size) {
    std::string *res = new std::string();
    for (int i = 0; i < size; i++) {
        res->append(*int8_to_hex_string(array[i]));
        *res += " ";
    }
    return res->c_str();
}

static void test() {
    uint8_t values[100] = {0};
//    char string[200];
    SinkData data(&values, 100);

    LOGE("%s", int8_array_to_string(values, 20));
    data.write_int32(1800410);

    LOGE("%s", int8_array_to_hex_string(values, 20));

    SourceData source(&values, 100);
    int32_t result = source.read_int32();
    LOGE("result:%d", result);
}

#endif //IPCM_SINK_DATA_H
