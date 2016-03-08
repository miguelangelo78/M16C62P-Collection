package com.layout;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import app.miguel.climatecontrol.Climate;
import app.miguel.climatecontrol.More;
import app.miguel.climatecontrol.R;
import app.miguel.climatecontrol.Settings;
import app.miguel.climatecontrol.bluetooth.BToothEvtRx;
import app.miguel.climatecontrol.bluetooth.Bluetooth;

public class Main2Activity extends AppCompatActivity {
    private Bluetooth btooth;
    private Button disconnBut;
    private Button moreBut;
    private Button settingsBut;
    private TextView tempLabel;
    private TextView desiredTempLabel;
    private TextView rpmLabel;
    private int currentTemp = 0;
    private int currentTempMethod = 0;

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        /* Send command to resume normal program */
        Climate.sendCmd(Climate.BTOOTH_CMD.CMD_BYPASSCLIMATE, new char[]{1});
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        tempLabel = (TextView)findViewById(R.id.textView5);
        rpmLabel = (TextView)findViewById(R.id.textView4);
        desiredTempLabel = (TextView)findViewById(R.id.textView7);

        btooth = Bluetooth.getBtooth();
        btooth.setOnRx(new BToothEvtRx() {
            public void call(final byte[] data) {
                runOnUiThread(new Runnable() {
                    public void run() {
                        currentTemp = data[2];
                        tempLabel.setText(data[1] + "ºC");
                        desiredTempLabel.setText(data[2] + "ºC");
                        rpmLabel.setText((data[3] | (data[4] << 8)) + "");
                        currentTempMethod = (int)data[5];
                    }
                });
            }
        });

        moreBut = (Button)findViewById(R.id.button4);
        moreBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                /* Send command to pause normal program */
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_BYPASSCLIMATE, new char[]{0});
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETMOTOR, new char[]{0,0,0});
                startActivityForResult(new Intent(Main2Activity.this, More.class), 1);
            }
        });

        settingsBut = (Button)findViewById(R.id.button3);
        settingsBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Bundle b = new Bundle();
                b.putInt("temp", currentTemp);
                b.putInt("method", currentTempMethod);
                Intent settingsInt = new Intent(Main2Activity.this, Settings.class);
                settingsInt.putExtras(b);
                startActivity(settingsInt);
            }
        });

        disconnBut = (Button)findViewById(R.id.button2);
        disconnBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });
    }
}
