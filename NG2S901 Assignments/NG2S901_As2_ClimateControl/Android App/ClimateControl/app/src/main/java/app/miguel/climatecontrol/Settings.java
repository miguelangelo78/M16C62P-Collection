package app.miguel.climatecontrol;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

public class Settings extends Activity {
    private Button backBut;
    private Button setBut;
    private Switch switchBut;
    private SeekBar seek;
    private TextView tempLabel;
    private TextView tempTypeLabel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.settingsactivity);

        backBut = (Button)findViewById(R.id.button5);
        backBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });

        Bundle b = getIntent().getExtras();
        int currTemp = b.getInt("temp");
        tempLabel = (TextView)findViewById(R.id.textView12);
        tempLabel.setText(currTemp + "ºC");

        seek = (SeekBar)findViewById(R.id.seekBar2);
        seek.setProgress(currTemp);
        seek.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tempLabel.setText(progress + "ºC");
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETTEMP, new char[]{(char) progress});
            }
        });

        setBut = (Button)findViewById(R.id.button7);
        setBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETTEMP, new char[]{(char) seek.getProgress()});
            }
        });

        tempTypeLabel = (TextView)findViewById(R.id.textView13);

        switchBut = (Switch)findViewById(R.id.switch2);
        boolean tempMethod = b.getInt("method")==1?true:false;
        switchBut.setChecked(tempMethod);
        if(tempMethod) tempTypeLabel.setText("Digital");
        else tempTypeLabel.setText("Analogue");

        switchBut.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) tempTypeLabel.setText("Digital");
                else tempTypeLabel.setText("Analogue");
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETTHERMMETHOD, new char[]{(char)(isChecked?1:0)});
            }
        });
    }
}
