package com.example.shiva.loginsignup;

/**
 * Created by Juned on 3/13/2017.
 */

import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;

public class SQLiteHelper extends SQLiteOpenHelper {


    static String DATABASE_NAME="UserDataBase";

    public static final String TABLE_NAME="UserTable";

    public static final String Table_Column_ID="id";

    public static final String Table_Column_1_Name="name";

    public static final String Table_Column_2_Email="email";

    public static final String Table_Column_3_Password="password";

    public SQLiteHelper(Context context) {

        super(context, DATABASE_NAME, null, 1);

    }

    @Override
    public void onCreate(SQLiteDatabase database) {

        String CREATE_TABLE="CREATE TABLE IF NOT EXISTS "+TABLE_NAME+" ("+Table_Column_ID+" INTEGER PRIMARY KEY, "+Table_Column_1_Name+" VARCHAR, "+Table_Column_2_Email+" VARCHAR, "+Table_Column_3_Password+" VARCHAR)";
        database.execSQL(CREATE_TABLE);

    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS "+TABLE_NAME);
        onCreate(db);

    }

    public boolean update(String id, String number) {
        SQLiteDatabase db = this.getReadableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(Table_Column_1_Name,number);
        String[] args = new String[]{id};
        db.update(TABLE_NAME,contentValues,  "email=? ",args);






//        String up = "UPDATE "+TABLE_NAME+" SET name = "+"'"+number+"' "+ "WHERE email = "+"'"+email+"'";
//        db.update(TABLE_NAME,contentValues,"email=\'"+email+"\' and password= '"+password+"'",null);
//        String[] newemail = {String.valueOf(emailll)};
//        db.update(TABLE_NAME,contentValues,"email = ?",new String[] {email} );

//        db.update(TABLE_NAME,contentValues, Table_Column_2_Email+"=?",new String[] {emailll} );
//        db.execSQL("UPDATE "+TABLE_NAME+" SET name = "+"'"+number+"' "+ "WHERE email = "+"'"+email+"'");
//        db.rawQuery(up,null);
        return true;
    }

}