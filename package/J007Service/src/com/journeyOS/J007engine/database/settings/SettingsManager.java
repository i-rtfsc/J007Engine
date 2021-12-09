package com.journeyOS.J007engine.database.settings;

import android.content.Context;
import android.util.Singleton;

import androidx.annotation.NonNull;
import androidx.room.Room;
import androidx.room.RoomDatabase;

import com.journeyOS.J007engine.core.J007Core;

public class SettingsManager {

    public static void put(@NonNull String key, @NonNull Object defaultValue) {
        SettingsManagerImpl.getDefault().save(key, defaultValue);
    }

    public static String getString(@NonNull String key, @NonNull String defaultValue) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, defaultValue);
        return settings.getString();
    }

    public static String getString(@NonNull String key) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, "");
        return settings.getString();
    }

    public static int getInt(@NonNull String key, int defaultValue) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, defaultValue);
        return settings.getInt();
    }

    public static int getInt(@NonNull String key) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, -1);
        return settings.getInt();
    }

    public static boolean getBoolean(@NonNull String key, boolean defaultValue) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, defaultValue);
        return settings.getBoolean();
    }

    public static boolean getBoolean(@NonNull String key) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, false);
        return settings.getBoolean();
    }

    public static float getFloat(@NonNull String key, float defaultValue) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, defaultValue);
        return settings.getFloat();
    }

    public static float getFloat(@NonNull String key) {
        Settings settings = SettingsManagerImpl.getDefault().get(key, 0.0f);
        return settings.getFloat();
    }

    private static class SettingsManagerImpl {
        private static final String TAG = SettingsManagerImpl.class.getSimpleName();

        private final SettingsDao mSettingsDao;
        private final Object mLock = new Object();

        private static final Singleton<SettingsManagerImpl> gDefault = new Singleton<SettingsManagerImpl>() {
            @Override
            protected SettingsManagerImpl create() {
                return new SettingsManagerImpl();
            }
        };

        private SettingsManagerImpl() {
            Context context = J007Core.getCore().getContext();
            SettingsDatabase database = getRoomDatabaseBuilder(context, SettingsDatabase.class, Config.DB_NAME);
            mSettingsDao = database.settingsDao();
        }

        private <T extends RoomDatabase> T getRoomDatabaseBuilder(Context context, Class<T> dbCls, String dbName) {
            return Room.databaseBuilder(context, dbCls, dbName)
                    .allowMainThreadQueries()
                    .setJournalMode(RoomDatabase.JournalMode.AUTOMATIC)
                    .fallbackToDestructiveMigration().build();
        }

        public static SettingsManagerImpl getDefault() {
            return gDefault.get();
        }

        public void save(@NonNull String key, @NonNull Object value) {
            synchronized (mLock) {
                Settings settings = mSettingsDao.getSettings(key);
                if (settings == null) {
                    settings = new Settings();
                    settings.key = key;

                }
                Class<?> clazz = value.getClass();
                settings.object = clazz.getName();
                settings.value = value.toString();
                mSettingsDao.saveSettings(settings);
            }
        }

        public Settings get(@NonNull String key, @NonNull Object defaultValue) {
            synchronized (mLock) {
                Settings settings = mSettingsDao.getSettings(key);
                if (settings == null) {
                    Class<?> clazz = defaultValue.getClass();
                    final String object = clazz.getName();

                    settings = new Settings();
                    settings.key = key;
                    settings.value = defaultValue.toString();
                    settings.object = object;
                }

                return settings;
            }
        }
    }
}
