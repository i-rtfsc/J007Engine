package com.journeyOS.J007engine;


import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.journeyOS.J007engine.database.LocalDataSourceImpl;
import com.journeyOS.J007engine.database.settings.SettingsManager;

public class DebugActivity extends AppCompatActivity {
    private static final String TAG = "DebugActivity";
    private LinearLayout mLayout;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mLayout = new LinearLayout(this);
        mLayout.setOrientation(LinearLayout.VERTICAL);
        initView();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private void initView() {
        Log.d(TAG, "init view");
        TextView textView = new TextView(this);
        mLayout.addView(textView);

        Button button = new Button(this);
        button.setText("Save sp");
        button.setOnClickListener(v -> {
            Log.d(TAG, "set config button click");
            //SettingsManager.put("testKey", "testValue");

            LocalDataSourceImpl.getDefault().initApp();

            /**
             * /data/data/com.journeyOS.J007engine/
             * /data/user_de/0/com.journeyOS.J007engine/
             */
        });
        mLayout.addView(button);

        button = new Button(this);
        button.setText("Get sp");
        button.setOnClickListener(v -> {
            String value = SettingsManager.getString("testKey", "null");
            Log.d(TAG, "get config button click = " + value);
            textView.setText(value + " time = " + System.currentTimeMillis());
        });
        mLayout.addView(button);

        TextView version = new TextView(this);
        version.setText(BuildConfig.BUILD_VERSION_NAME);
        mLayout.addView(version);

        ScrollView sv = new ScrollView(this);
        sv.addView(mLayout);
        setContentView(sv);
    }

}
