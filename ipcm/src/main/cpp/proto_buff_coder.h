//
// Created by nebula on 2019-07-07.
//

#ifndef IPCM_PROTO_BUFF_CODER_H
#define IPCM_PROTO_BUFF_CODER_H

#include "ipc_buffer.h"
#include "source_data.h"
#include "sink_data.h"
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

#endif //IPCM_PROTO_BUFF_CODER_H
