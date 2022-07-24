package com.example.myapplicationcpp;

import static java.lang.Thread.sleep;

import android.util.Log;
import android.widget.TextView;

public class TestLog {

    private final static String TAG = TestLog.class.getSimpleName();

    static {
        System.loadLibrary("logtest");
    }

    public TestLog() {
        // Log.d(TAG, "From test " + stringFromTest());
        new Thread(new Runnable() {
            @Override
            public void run() {
                // Log.d(TAG, "From test " + stringFromTest());
                Log.d(TAG, "From test " );

                try {
                    sleep(1000);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();

        // createNewThread();
    }

    public native void createNewThread();

    public native String stringFromTest();

}
