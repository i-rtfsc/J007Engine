package com.journeyOS.J007engine.database;

import com.journeyOS.J007engine.database.app.App;

import java.util.List;

public interface ILocalDataSource {
    void saveApp(App app);

    void saveApps(List<App> apps);

    App getApp(String packageName);

}
