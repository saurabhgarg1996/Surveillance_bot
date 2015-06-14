package com.camera.simplemjpeg;

import android.app.Application;

/**
 * Created by user on 04/06/2015.
 */
class global extends Application {

    private int mGlobalVarValue;

    public int getGlobalVarValue() {
        return mGlobalVarValue;
    }

    public void setGlobalVarValue(int i) {
        mGlobalVarValue = i;
    }
}