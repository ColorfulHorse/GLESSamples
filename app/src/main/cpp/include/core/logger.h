//
// Created by admin on 2022/10/8.
//

#ifndef GLSAMPLE_LOGGER_H
#define GLSAMPLE_LOGGER_H

#ifdef ANDROID

#include <android/log.h>

#define LOGE(tag, format,  ...)  __android_log_print(ANDROID_LOG_ERROR, tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...)  __android_log_print(ANDROID_LOG_INFO,  tag, format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf(LOG_TAG format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf(LOG_TAG format "\n", ##__VA_ARGS__)
#endif

#endif //GLSAMPLE_LOGGER_H
