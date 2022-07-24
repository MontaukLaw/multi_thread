package com.example.myapplicationcpp;

import static java.lang.Thread.sleep;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.myapplicationcpp.databinding.ActivityMainBinding;

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

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        tv = binding.sampleText;
        tv.setText(stringFromJNI());
        // TestLog testLog = new TestLog();

        startSubThread();
        Log.d(TAG, "onCreate: ");
    }

    public native String stringFromJNI();

    public native void startSubThread();

    public void printTest() {
        Log.d(TAG, "printTest: ");
    }

    public void updateUI(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(stringFromJNI());
            }
        });
    }

}
