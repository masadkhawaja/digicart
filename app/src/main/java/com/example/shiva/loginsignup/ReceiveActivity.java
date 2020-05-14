package com.example.shiva.loginsignup;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

public class ReceiveActivity extends AppCompatActivity {

    TextView receive;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_receive);
        receive = (TextView) findViewById(R.id.receive);

        Intent intent = getIntent();
        String message = intent.getStringExtra("msg");
        receive.setText(message);

    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        int id = android.os.Process.myPid();
        android.os.Process.killProcess(id);
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        finish();

    }
}
