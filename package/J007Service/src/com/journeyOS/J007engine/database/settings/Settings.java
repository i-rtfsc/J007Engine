package com.journeyOS.J007engine.database.settings;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.Ignore;

@Entity(tableName = Config.TABLE_NAME, primaryKeys = {Config.KEY})
public class Settings {

    @NonNull
    @ColumnInfo(name = Config.KEY)
    public String key = "";

    @ColumnInfo(name = Config.VALUE)
    public String value;

    @ColumnInfo(name = Config.OBJECT)
    public String object;

    @Ignore
    public boolean getBoolean() {
        if (Boolean.class.getName().equals(object)) {
            return Boolean.parseBoolean(value);
        } else {
            throw new IllegalStateException("settings is " + object);
        }
    }

    @Ignore
    public int getInt() {
        if (Integer.class.getName().equals(object)) {
            return (Integer) Integer.parseInt(value);
        } else {
            throw new IllegalStateException("settings is " + object);
        }
    }

    @Ignore
    public String getString() {
        if (String.class.getName().equals(object)) {
            return (String) value;
        } else {
            throw new IllegalStateException("settings is " + object);
        }
    }

    @Ignore
    public float getFloat() {
        if (Float.class.getName().equals(object)) {
            return Float.parseFloat(value);
        } else {
            throw new IllegalStateException("settings is " + object);
        }
    }
}
