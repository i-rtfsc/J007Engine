/*
 * Copyright (c) 2021 anqi.huang@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _FACTORS_H
#define _FACTORS_H

//factor type
#define SCENE_FACTOR_APP                (1 << 1)
#define SCENE_FACTOR_LCD                (1 << 2)
#define SCENE_FACTOR_BRIGHTNESS         (1 << 3)
#define SCENE_FACTOR_NET                (1 << 4)
#define SCENE_FACTOR_HEADSET            (1 << 5)
#define SCENE_FACTOR_BATTERY            (1 << 6)

//app type
//only limit system-background
#define APP_DEFAULT                     "default"

//4 big cores, others 4 little cores
#define APP_ALBUM                       "album"

//2 big cores only for top-app
#define APP_IM                          "im"
#define APP_MUSIC                       "music"

//op-app 8 cores, others 4 little cores
#define APP_GAME                        "game"
//top-app and (system-)foreground 8 cores
#define APP_BENCHMARK                   "benchmark"

//foreground more cores, SF/HWC to big cores
#define APP_VIDEO                       "video"
#define APP_LIVE                        "live"

//top-app 7 cores, not use cpu7
#define APP_NEWS                        "news"
#define APP_READER                      "reader"
#define APP_BROWSER                     "browser"
#define APP_WEIBO                       "weibo"
#define APP_SHOP                        "shop"

//top-app 4 cores
#define APP_LAUNCHER                    "launcher"
#define APP_NAVIGATION                  "navigation"


#endif //_FACTORS_H
