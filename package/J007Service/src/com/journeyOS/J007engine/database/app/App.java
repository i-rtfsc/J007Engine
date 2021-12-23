package com.journeyOS.J007engine.database.app;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.Ignore;

import com.journeyOS.J007engine.database.DBConfigs;

@Entity(tableName = DBConfigs.App.TABLE_NAME, primaryKeys = {DBConfigs.App.PACKAGE_NAME})
public class App {

    @NonNull
    @ColumnInfo(name = DBConfigs.App.PACKAGE_NAME)
    public String packageName = "";

    @ColumnInfo(name = DBConfigs.App.TYPE)
    public String type;

    @ColumnInfo(name = DBConfigs.App.MODE)
    public int mode = -1;
    
    @ColumnInfo(name = DBConfigs.App.FPS)
    public int fps = -1;

    @ColumnInfo(name = DBConfigs.App.CPU)
    public int cpu = -1;

    @ColumnInfo(name = DBConfigs.App.MEMC)
    public int memc = -1;

    @ColumnInfo(name = DBConfigs.App.BL)
    public int bl = 0;

    @Ignore
    @Override
    public String toString() {
        return "Apps{" +
                "packageName='" + packageName + '\'' +
                ", type='" + type + '\'' +
                ", fps=" + fps +
                ", cpu=" + cpu +
                ", memc=" + memc +
                ", bl=" + bl +
                '}';
    }
}
