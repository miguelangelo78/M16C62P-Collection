package app.miguel.climatecontrol.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

public class Bluetooth extends Thread {
    private static Bluetooth btooth;
    public static synchronized Bluetooth getBtooth() {
        return btooth;
    }
    public static synchronized void setBtooth(Bluetooth btooth) {
        Bluetooth.btooth = btooth;
    }

    private BluetoothSocket socket;
    private InputStream input;
    private OutputStream output;
    private BluetoothAdapter bAdapter;
    private BluetoothDevice device;
    private BToothEvt onConnect;
    private BToothEvt onDisconnect;
    private BToothEvtRx onRx;

    BluetoothDevice getDeviceByName(String bluetoothName, BluetoothAdapter adapter) {
        for(BluetoothDevice dev : adapter.getBondedDevices())
            if (dev.getName().trim().equals(bluetoothName))
                return dev;
        return null;
    }

    public Bluetooth(String deviceName) {
        bAdapter = BluetoothAdapter.getDefaultAdapter();
        device = getDeviceByName(deviceName, bAdapter);
    }

    public void setOnConnect(BToothEvt evt) {
        onConnect = evt;
    }

    public void setOnDisconnect(BToothEvt evt) {
        onDisconnect = evt;
    }

    public void setOnRx(BToothEvtRx evt) {
        onRx = evt;
    }

    public void send(byte[] data) {
        try {
            output.write(data);
        } catch(IOException e) { e.printStackTrace(); }
    }

    public int read(final byte[] buff) {
        int bytes;
        try {
            bytes = input.read(buff);
        } catch(IOException e) { e.printStackTrace(); return -1; }
        return bytes;
    }

    void manage() {
        final byte[] read_buffer = new byte[7];

        while(!Thread.interrupted())
            if(read(read_buffer) != -1)
                if(onRx != null)
                    onRx.call(read_buffer);
            else
                break;
    }

    public void run() {
        while(true) {
            try {
                socket = device.createRfcommSocketToServiceRecord(device.getUuids()[0].getUuid());
                bAdapter.cancelDiscovery();
                socket.connect();
                if (onConnect != null)
                    onConnect.call();
                input = socket.getInputStream();
                output = socket.getOutputStream();
                break;
            } catch (IOException e) {
                e.printStackTrace();
                try {
                    socket.close();
                } catch (IOException e2) {
                    e2.printStackTrace();
                }
                socket = null;
            }
        }
        manage();
    }

    public void disconnect() {
        try {
            socket.close();
            if(onDisconnect != null)
                onDisconnect.call();
        } catch(IOException e) { e.printStackTrace(); }
    }

    public void connect() {
        this.start();
    }
}