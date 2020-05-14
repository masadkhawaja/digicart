package com.example.shiva.loginsignup;

import android.annotation.TargetApi;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;
import android.widget.Toast;

public class MySmsReceiver extends BroadcastReceiver {
    private static final String TAG = MySmsReceiver.class.getSimpleName();
    public static final String pdu_type = "pdus";
    private Bundle bundle;
    private SmsMessage currentSMS;
    private String message;
    /**
     * Called when the BroadcastReceiver is receiving an Intent broadcast.
     *
     * @param context  The Context in which the receiver is running.
     * @param intent   The Intent received.
     */
    @TargetApi(Build.VERSION_CODES.M)
    @Override
    public void onReceive(Context context, Intent intent) {

        if (intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED")) {
            bundle = intent.getExtras();
            if (bundle != null) {
                Object[] pdu_Objects = (Object[]) bundle.get("pdus");
                if (pdu_Objects != null) {

                    for (Object aObject : pdu_Objects) {

                        currentSMS = getIncomingMessage(aObject, bundle);

                        String senderNo = currentSMS.getDisplayOriginatingAddress();

                        message = currentSMS.getDisplayMessageBody();
                        //Toast.makeText(OtpActivity.this, "senderNum: " + senderNo + " :\n message: " + message, Toast.LENGTH_LONG).show();

                        Intent intent1 = new Intent(context, ReceiveActivity.class);
                        intent1.putExtra("msg", message);
                        intent1.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        context.startActivity(intent1);

                    }
                    this.abortBroadcast();
                    // End of loop
                }
            }
        } // bundle null
    }


//        // Get the SMS message.
//        Bundle bundle = intent.getExtras();
//        SmsMessage[] msgs;
//        String strMessage = "";
//        String format = bundle.getString("format");
//        // Retrieve the SMS message received.
//        Object[] pdus = (Object[]) bundle.get(pdu_type);
//        if (pdus != null) {
//            // Check the Android version.
//            boolean isVersionM = (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M);
//            // Fill the msgs array.
//            msgs = new SmsMessage[pdus.length];
//            for (int i = 0; i < msgs.length; i++) {
//                // Check Android version and use appropriate createFromPdu.
//                if (isVersionM) {
//                    // If Android version M or newer:
//                    msgs[i] = SmsMessage.createFromPdu((byte[]) pdus[i], format);
//                } else {
//                    // If Android version L or older:
//                    msgs[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
//                }
//                // Build the message to show.
//                strMessage += "SMS from " + msgs[i].getOriginatingAddress();
//                strMessage += " :" + msgs[i].getMessageBody() + "\n";
//                // Log and display the SMS message.
//                Log.d(TAG, "onReceive: " + strMessage);
//                Toast.makeText(context, strMessage, Toast.LENGTH_LONG).show();
//
//                Intent intent1 = new Intent(context, ReceiveActivity.class);
//                intent1.putExtra("msg", strMessage);
////                intent1.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//                context.startActivity(intent1);

//            }
     //   }
private SmsMessage getIncomingMessage(Object aObject, Bundle bundle) {
    SmsMessage currentSMS;
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        String format = bundle.getString("format");
        currentSMS = SmsMessage.createFromPdu((byte[]) aObject, format);
    } else {
        currentSMS = SmsMessage.createFromPdu((byte[]) aObject);
    }
    return currentSMS;
}
}
