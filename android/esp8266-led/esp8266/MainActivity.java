package code.com.esp8266;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends BaseActivity {
    private Button button1;
    private Button button2;
    private Button button3;
    public Socket socket;
    private EditText text1;
    private static final String HOST = "192.168.4.1";
    private PrintWriter printWriter;
    private BufferedReader br;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button1 = (Button) findViewById(R.id.button_1);
        button2 = (Button) findViewById(R.id.button_2);
        button3 = (Button) findViewById(R.id.button_3);
        text1 = (EditText) findViewById(R.id.text_1);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            socket = new Socket(HOST, 8080);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
            }
        });
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            PrintStream ps = new PrintStream(socket.getOutputStream());
                            ps.println("1");
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
            }
        });
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            PrintStream ps = new PrintStream(socket.getOutputStream());
                            ps.println("0");
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
            }
        });
    }
}
