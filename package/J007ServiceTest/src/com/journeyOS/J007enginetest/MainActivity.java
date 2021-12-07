package com.journeyOS.J007enginetest;

import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.journeyOS.J007engine.J007EngineManager;

/**
 * @author solo
 * test activity
 */
public class MainActivity extends AppCompatActivity implements J007EngineManager.OnSceneChangedListener {
    private static final String TAG = "J007EngineTest";
    private LinearLayout mLayout;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        J007EngineManager.registerOnSceneChangedListener(J007EngineManager.SCENE_FACTOR_BRIGHTNESS, this);

        mLayout = new LinearLayout(this);
        mLayout.setOrientation(LinearLayout.VERTICAL);
        initView();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        J007EngineManager.unregisterOnSceneChangedListener(this);
    }

    private void initView() {
        Log.d(TAG, "init view");
        Button button = new Button(this);
        button.setText("Set Config");
        button.setOnClickListener(v -> {
            Log.d(TAG, "set config button click");
            J007EngineManager.setConfig(0, "aaa");
        });
        mLayout.addView(button);

        button = new Button(this);
        button.setText("Get Config");
        button.setOnClickListener(v -> {
            Log.d(TAG, "get config button click");
            J007EngineManager.getConfig(1);
        });
        mLayout.addView(button);


        ScrollView sv = new ScrollView(this);
        sv.addView(mLayout);
        setContentView(sv);
    }

    @Override
    public void onSceneChanged(long l, String s, String s1) {
        Log.d(TAG, "scene changed factors = [" + l + "], status = [" + s + "], packageName = [" + s1 + "]");
    }
}
