package app.miguel.climatecontrol;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.layout.Main2Activity;

import app.miguel.climatecontrol.bluetooth.BToothEvt;
import app.miguel.climatecontrol.bluetooth.Bluetooth;


public class MainActivity extends AppCompatActivity {
    private Bluetooth btooth;
    private Button connBut;

    void popup(final Context ctx, final String msg) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(ctx, msg, Toast.LENGTH_LONG).show();
            }
        });
    }

    void setText(final Button b, final String text) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                b.setText(text);
            }
        });
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        start();
    }

    public Context getContext() {
        return this;
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
       /* Disconnect from HC06: */
        btooth.disconnect();
        try {
            btooth.interrupt();
            btooth.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void start() {
        connBut = (Button) findViewById(R.id.button);
        connBut.setOnClickListener(new View.OnClickListener() {

            public void onClick(View v) {
                popup(v.getContext(), "Connecting to HC06 ...");
                setText(connBut, "Connecting...");
                connBut.setEnabled(false);

                btooth = new Bluetooth("HC-06");

                btooth.setOnConnect(new BToothEvt() {
                    public void call() {
                        popup(getContext(), "Connected!");

                        /* Launch 2nd activity: */
                        startActivityForResult(new Intent(MainActivity.this, Main2Activity.class), 1);
                    }
                });
                btooth.setOnDisconnect(new BToothEvt() {
                    public void call() {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                connBut.setEnabled(true);
                            }
                        });
                        popup(getContext(), "Disconnected!");
                        setText(connBut, "Connect to the Board");
                    }
                });

                /* Connect to HC06: */
                btooth.connect();

                Bluetooth.setBtooth(btooth);
            }
        });
    }
}
