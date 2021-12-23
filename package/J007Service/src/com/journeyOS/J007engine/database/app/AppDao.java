package com.journeyOS.J007engine.database.app;

import static androidx.room.OnConflictStrategy.REPLACE;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

import com.journeyOS.J007engine.database.DBConfigs;

import java.util.List;

@Dao
public interface AppDao {
    @Insert(onConflict = REPLACE)
    void insert(App app);

    @Insert(onConflict = REPLACE)
    void insert(List<App> apps);

    @Query("DELETE FROM " + DBConfigs.App.TABLE_NAME + " WHERE " + DBConfigs.App.PACKAGE_NAME + " LIKE :packageName")
    void delete(String packageName);

    @Delete
    void delete(App app);

    @Delete
    void delete(List<App> apps);

    @Query("SELECT * FROM " + DBConfigs.App.TABLE_NAME + " WHERE " + DBConfigs.App.PACKAGE_NAME + " LIKE :packageName")
    App getApp(String packageName);

    @Query("SELECT * FROM " + DBConfigs.App.TABLE_NAME)
    List<App> getApps();
}
