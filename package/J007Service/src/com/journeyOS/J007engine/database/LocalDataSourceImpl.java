package com.journeyOS.J007engine.database;

import android.content.Context;
import android.util.Singleton;

import com.journeyOS.J007engine.core.J007Core;
import com.journeyOS.J007engine.database.app.App;
import com.journeyOS.J007engine.database.app.AppDao;
import com.journeyOS.J007engine.database.settings.SettingsManager;
import com.journeyOS.J007engine.security.AESUtils;
import com.journeyOS.J007engine.task.TaskManager;
import com.journeyOS.J007engine.utils.FileUtils;
import com.journeyOS.J007engine.utils.JsonUtils;
import com.journeyOS.J007engine.utils.SmartLog;

import java.util.List;
import java.util.WeakHashMap;

public class LocalDataSourceImpl implements ILocalDataSource {
    private static final String TAG = LocalDataSourceImpl.class.getSimpleName();
    public static final String APP = "app.json";
    public static final String APP_ENCRYPT = "app_encrypt.json";

    private static final Singleton<LocalDataSourceImpl> gDefault = new Singleton<LocalDataSourceImpl>() {
        @Override
        protected LocalDataSourceImpl create() {
            return new LocalDataSourceImpl();
        }
    };

    private AppDao mAppDao;

    private WeakHashMap<String, App> mCacheApps = new WeakHashMap<>();

    private LocalDataSourceImpl() {
        J007Database database = DBHelper.getDefault().getRoomDatabaseBuilder(J007Core.getCore().getContext(), J007Database.class, DBConfigs.DB_NAME);
        mAppDao = database.appsDao();
        //init app if needed
        initApp();
    }

    public static LocalDataSourceImpl getDefault() {
        return gDefault.get();
    }

    @Override
    public void saveApp(App app) {
        if (app == null) {
            SmartLog.w(TAG, "save null object");
            return;
        }

        mAppDao.insert(app);
        if (mCacheApps.containsKey(app.packageName)) {
            mCacheApps.replace(app.packageName, app);
        } else {
            mCacheApps.put(app.packageName, app);
        }
    }

    @Override
    public void saveApps(List<App> apps) {
        if (apps == null) {
            SmartLog.w(TAG, "save null object");
            return;
        }

        mAppDao.insert(apps);

        for (App app : apps) {
            if (mCacheApps.containsKey(app.packageName)) {
                mCacheApps.replace(app.packageName, app);
            } else {
                mCacheApps.put(app.packageName, app);
            }
        }
    }

    @Override
    public App getApp(String packageName) {
        if (packageName == null) {
            SmartLog.w(TAG, "packageName was null");
            return null;
        }

        App app = mCacheApps.get(packageName);
        if (app == null) {
            mAppDao.getApp(packageName);
        }

        return app;
    }

    public void initApp() {
        TaskManager.getDefault().diskIO().execute(new Runnable() {
            @Override
            public void run() {
                boolean init = SettingsManager.getBoolean(DBConfigs.Settings.APP_INIT, DBConfigs.Settings.APP_INIT_DEFAULT);
                if (init) {
                    SmartLog.w(TAG, "app table has been init");
                    return;
                }
                Context context = J007Core.getCore().getContext();
//                String json = FileUtils.readFileFromAsset(context, APP);
//                if (json != null) {
//                    String decrypt = AESUtils.encrypt(json);
//                    File file = new File(context.getCacheDir(), APP_ENCRYPT);
//                    FileUtils.write2File(file, decrypt);
//                }
                String game = AESUtils.decrypt(FileUtils.readFileFromAsset(context, APP_ENCRYPT));
                if (game != null) {
                    JsonApps appInfo = JsonUtils.fromJson(game, JsonApps.class);
                    if (appInfo != null) {
                        List<App> apps = appInfo.apps;
                        mAppDao.insert(apps);
                        SettingsManager.put(DBConfigs.Settings.APP_INIT, !DBConfigs.Settings.APP_INIT_DEFAULT);
                    }
                }
            }
        });
    }

    public static class JsonApps {
        public int version;
        public List<App> apps;
    }

}
