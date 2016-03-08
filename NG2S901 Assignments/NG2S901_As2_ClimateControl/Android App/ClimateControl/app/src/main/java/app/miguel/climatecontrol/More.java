package app.miguel.climatecontrol;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

public class More extends Activity {
    private TextView rpmLabel;
    private TextView dirLabel;
    private SeekBar seek;
    private Switch switchBut;
    private Button backBut;
    private char motorDirection = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.moreactivity);

        backBut = (Button)findViewById(R.id.button6);
        backBut.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });

        dirLabel = (TextView)findViewById(R.id.textView10);
        rpmLabel = (TextView)findViewById(R.id.textView9);

        switchBut = (Switch)findViewById(R.id.switch1);
        switchBut.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                motorDirection = (char)(isChecked?1:0);
                dirLabel.setText(isChecked?"Right":"Left");
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETMOTOR, new char[]{motorDirection});
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETMOTOR, new char[]{(char) (seek.getProgress() & 0xFF), (char) ((seek.getProgress() & 0xFF00) >> 8), motorDirection});
            }
        });

        seek = (SeekBar)findViewById(R.id.seekBar);
        seek.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Climate.sendCmd(Climate.BTOOTH_CMD.CMD_SETMOTOR, new char[]{(char) (progress & 0xFF), (char) ((progress & 0xFF00) >> 8), motorDirection});
                rpmLabel.setText(progress + " RPM");
            }
        });
    }
}
