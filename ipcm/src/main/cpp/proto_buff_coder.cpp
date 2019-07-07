//
// Created by nebula on 2019-07-07.
//

#include "proto_buff_coder.h"

using namespace std;

ProtoBuffCoder::ProtoBuffCoder() {
    m_input_buffer = nullptr;
    m_input_data = nullptr;
    m_output_buffer = nullptr;
    m_output_data = nullptr;
}

ProtoBuffCoder::ProtoBuffCoder(IPCBuffer *input_buffer) {
    m_input_buffer = input_buffer;
    m_input_data = new SourceData(input_buffer->get_ptr(), input_buffer->length());

}

ProtoBuffCoder::~ProtoBuffCoder() {
    delete m_input_data;
    delete m_output_data;
    delete m_input_buffer;
    delete m_output_buffer;
}

IPCBuffer ProtoBuffCoder::get_buff_from(const std::unordered_map<std::string, IPCBuffer> &dic) {

    return IPCBuffer();
}

void ProtoBuffCoder::decode_buffer(std::unordered_map<std::string, IPCBuffer> &dic,
                                   size_t size) {
    if (size == 0) {
        auto length = m_input_data->read_int32();
    }
    while (!m_input_data->is_end()) {
        const auto &key = m_input_data->read_string();
        if (key.length() > 0) {
            auto value = m_input_data->read_data();
            if (value.length() > 0) {
                dic[key] = move(value);
            } else {
                dic.erase(key);
            }
        }
    }
}

void ProtoBuffCoder::decode_buffer_to_map(std::unordered_map<std::string, IPCBuffer> &dic,
                                          IPCBuffer &buffer, size_t size) {
    ProtoBuffCoder coder(&buffer);
    coder.decode_buffer(dic, size);
}

template<typename T>
IPCBuffer ProtoBuffCoder::encode_data_to_buff(const T &object) {
    ProtoBuffCoder coder;
    return coder.get_buff_from(object);
}


