//
// Created by nebula on 2019-07-07.
//

#include "proto_buff_coder.h"

using namespace std;

vector<EncodeItem> *construct_item(const unordered_map<string, IPCBuffer> &);

EncodeItem construct_item(const string &);

EncodeItem construct_item(const IPCBuffer &);

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
    vector<EncodeItem> *items = construct_item(dic);
    EncodeItem *item = &(*items)[0];
    if (item && item->compiled_size > 0) {
        m_output_buffer = new IPCBuffer(item->compiled_size);
        m_output_data = new SinkData(m_output_buffer->get_ptr(), m_output_buffer->length());
    }
    for (auto &encode_item : *items) {
        switch (encode_item.type) {
            case EncodeItemType_String: {
                m_output_data->write_string(*(encode_item.value.str_value));
                break;
            }
            case EncodeItemType_Data: {
                m_output_data->write_buff(*(encode_item.value.buffer_value));
                break;
            }
            case EncodeItemType_Container: {
                m_output_data->write_varint32(encode_item.value_size);
                break;
            }
            case EncodeItemType_None: {
                break;
            }
        }
    }
    return move(*m_output_buffer);
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

vector<EncodeItem> *construct_item(const unordered_map<string, IPCBuffer> &dic) {
    EncodeItem item;
    auto *items = new vector<EncodeItem>();
    items->push_back(item);
    EncodeItem *current = &items->back();
    current->type = EncodeItemType_Container;

    for (const auto &itr:dic) {
        const auto &key = itr.first;
        const auto &value = itr.second;
        if (key.length() <= 0) {
            continue;
        }
        EncodeItem key_item = construct_item(key);
        EncodeItem value_item = construct_item(value);
        current->value_size += key_item.compiled_size;
        current->value_size += value_item.compiled_size;
        items->push_back(key_item);
        items->push_back(value_item);
    }
    return items;
}

EncodeItem construct_item(const IPCBuffer &buffer) {
    EncodeItem item;
    item.type = EncodeItemType_Data;
    item.value.buffer_value = &buffer;
    item.value_size = static_cast<uint32_t>(buffer.length());
    item.compiled_size = static_cast<uint32_t>(cal_varint32_size(item.value_size) +
                                               item.value_size);
    return item;
}

EncodeItem construct_item(const string &string) {
    EncodeItem item;
    item.type = EncodeItemType_String;
    item.value.str_value = &string;
    item.value_size = static_cast<uint32_t>(string.size());
    item.compiled_size = static_cast<uint32_t>(cal_varint32_size(item.value_size) +
                                               item.value_size);
    return item;
}


