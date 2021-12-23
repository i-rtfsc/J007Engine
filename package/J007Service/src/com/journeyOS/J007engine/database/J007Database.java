package com.journeyOS.J007engine.database;

import androidx.room.Database;
import androidx.room.RoomDatabase;

import com.journeyOS.J007engine.database.app.App;
import com.journeyOS.J007engine.database.app.AppDao;

@Database(entities = {App.class},
        version = DBConfigs.DB_VERSION,
        exportSchema = false)
public abstract class J007Database extends RoomDatabase {
    public abstract AppDao appsDao();

}
