//
// Created by nebula on 2019-07-07.
//

#ifndef IPCM_PROTO_BUFF_CODER_H
#define IPCM_PROTO_BUFF_CODER_H

#include "ipc_buffer.h"
#include "source_data.h"
#include "sink_data.h"
#include "utils.h"
#include "vector"
#include <unordered_map>

class ProtoBuffCoder {
    IPCBuffer *m_input_buffer;
    SourceData *m_input_data;

    IPCBuffer *m_output_buffer;
    SinkData *m_output_data;


private:
    ProtoBuffCoder();

    ProtoBuffCoder(IPCBuffer *input_buffer);

    ~ProtoBuffCoder();

    IPCBuffer get_buff_from(const std::unordered_map<std::string, IPCBuffer> &dic);

    void decode_buffer(std::unordered_map<std::string, IPCBuffer> &dic,
                       size_t size);

public:

    static void decode_buffer_to_map(std::unordered_map<std::string, IPCBuffer> &dic,
                                     IPCBuffer &buffer, size_t size = 0);

    template<typename T>
    static IPCBuffer encode_data_to_buff(const T &object);
};

enum EncodeItemType {
    EncodeItemType_None,
    EncodeItemType_String,
    EncodeItemType_Data,
    EncodeItemType_Container,
};

struct EncodeItem {
    EncodeItemType type;
    uint32_t compiled_size;
    uint32_t value_size;
    union {
        const std::string *str_value;
        const IPCBuffer *buffer_value;
    } value;

    EncodeItem() : type(EncodeItemType_None), compiled_size(0), value_size(0) {
        memset(&value, 0, sizeof(value));
    }
};

#endif //IPCM_PROTO_BUFF_CODER_H
