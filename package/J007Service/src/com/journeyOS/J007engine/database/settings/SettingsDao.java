package com.journeyOS.J007engine.database.settings;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;

import static androidx.room.OnConflictStrategy.REPLACE;

@Dao
public interface SettingsDao {
    @Insert(onConflict = REPLACE)
    void saveSettings(Settings setting);

    @Query("DELETE FROM " + Config.TABLE_NAME + " WHERE " + Config.KEY + " LIKE :key")
    void deleteSettings(String key);

    @Query("SELECT * FROM " + Config.TABLE_NAME + " WHERE " + Config.KEY + " LIKE :key")
    Settings getSettings(String key);
}
