package com.nebula.module.ipcm;

import android.content.Context;
import android.util.Log;

/**
 * Created by nebula on 2019-06-05
 */
public class IPCM {
    public static void init(Context context){
        init(context.getFilesDir().getAbsolutePath() + "/ipcm");
    }

    public static void init(String ipcmDir){
        nativeInit(ipcmDir);
    }

    public static native void nativeInit(String rootDir);
}
