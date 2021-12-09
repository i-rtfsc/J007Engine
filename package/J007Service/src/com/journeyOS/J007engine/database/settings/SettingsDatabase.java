package com.journeyOS.J007engine.database.settings;


import androidx.room.Database;
import androidx.room.RoomDatabase;

@Database(entities = {Settings.class},
        version = Config.DB_VERSION,
        exportSchema = false)
public abstract class SettingsDatabase extends RoomDatabase {
    public abstract SettingsDao settingsDao();
}

