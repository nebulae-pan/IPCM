package com.nebula.module.ipcm;

import android.content.Context;
import android.util.Log;

/**
 * Created by nebula on 2019-06-05
 */
public class IPCM {
    private long mPointer;

    //    public static int MODE_
    public static void init(Context context) {
        init(context.getFilesDir().getAbsolutePath() + "/ipcm");
    }

    public static void init(String ipcmDir) {
        nativeInit(ipcmDir);
    }

    public static IPCM obtain() {
        long ptr = defaultIPCM(0);
        return new IPCM(ptr);
    }

    private IPCM(long ptr) {
        mPointer = ptr;
        Log.e("IPCM", ptr + ":");
    }

    public void put(String key, int value) {
        encodeInt(mPointer, key, value);
    }

    public void put(String key, String value) {

    }

    public void put(String key, long value) {

    }


    private static native void nativeInit(String rootDir);

    private static native long defaultIPCM(int mode);

    private native void encodeInt(long ptr, String key, int value);
}
