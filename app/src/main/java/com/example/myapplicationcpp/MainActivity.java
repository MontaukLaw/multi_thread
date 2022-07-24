package com.example.myapplicationcpp;

import static java.lang.Thread.sleep;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.widget.TextView;

import com.example.myapplicationcpp.databinding.ActivityMainBinding;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.class.getSimpleName();
    TextView tv;

    static {
        System.loadLibrary("myapplicationcpp");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        tv = binding.sampleText;
        tv.setText(stringFromJNI());
        // TestLog testLog = new TestLog();

        // startSubThread();
        Log.d(TAG, "onCreate: ");

        subThread();
    }

    public native String stringFromJNI();

    public native void startSubThread();

    public void printTest() {
        Log.d(TAG, "printTest: ");
    }

    public void updateUI() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(stringFromJNI());
            }
        });
    }

    public void updateUIStr(String str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(str);
            }
        });
    }

    public void subThread() {
        new Runnable() {
            @Override
            public void run() {
                while (true) {
                    rev();

                    try {
                        sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
    }

    public void rev() {

        DatagramSocket receiveSocket = null;
        try {
            receiveSocket = new DatagramSocket(28000);
        } catch (Exception e) {
            e.printStackTrace();
        }
        boolean listenStatus = true;

        byte[] inBuf = new byte[1024];
        DatagramPacket inPacket = new DatagramPacket(inBuf, inBuf.length);
        try {
            assert receiveSocket != null;
            receiveSocket.receive(inPacket);
            byte[] data2 = new byte[1024];
            String reply = new String(data2, 0, inPacket.getLength());
            Log.d(TAG, "reply: " + reply);

            updateUIStr("new" + inPacket.getLength());

        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        // if (!inPacket.getAddress().equals("192.168.31.165")) {
        // throw new IOException("未知名的报文");
        // }

        // receiveInfo = inPacket.getData();
        // receiveHandler.sendEmptyMessage(1);
    }

}
