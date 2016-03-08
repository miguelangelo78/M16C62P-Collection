package app.miguel.climatecontrol;

import app.miguel.climatecontrol.bluetooth.Bluetooth;

public class Climate {
    private static Bluetooth btooth;

    public enum BTOOTH_CMD {
        CMD_BYPASSCLIMATE,
        CMD_SETMOTOR,
        CMD_SETTHERMMETHOD,
        CMD_SETTEMP
    }

    final static int CMD_FLAG = 255;

    public Climate() {}

    public static void sendCmd(BTOOTH_CMD cmd, char[] data) {
        btooth = Bluetooth.getBtooth();

        byte[] toSend = new byte[data.length + 3];

        toSend[0] = (byte)CMD_FLAG;
        toSend[1] = (byte)cmd.ordinal();
        toSend[toSend.length - 1] = '\n';
        for(int i=0;i < data.length;i++)
            toSend[i + 2] = (byte)data[i];

        btooth.send(toSend);
    }
}
