//
// Created by nebula on 2019-06-14.
//

#ifndef IPCM_UTILS_H
#define IPCM_UTILS_H

const int IPCM_MODE_NORMAL = 1;
const int IPCM_MODE_MULTI_PROCESS = 2;

inline size_t cal_varint32_size(int32_t value){
    if ((value & (0xffffffff << 7)) == 0) {
        return 1;
    } else if ((value & (0xffffffff << 14)) == 0) {
        return 2;
    } else if ((value & (0xffffffff << 21)) == 0) {
        return 3;
    } else if ((value & (0xffffffff << 28)) == 0) {
        return 4;
    }
    return 5;
}

#endif //IPCM_UTILS_H
