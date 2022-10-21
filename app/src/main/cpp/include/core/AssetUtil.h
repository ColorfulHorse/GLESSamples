//
// Created by admin on 2022/10/8.
//

#ifndef GLSAMPLE_ASSETUTIL_H
#define GLSAMPLE_ASSETUTIL_H

#include <android/asset_manager.h>
#include <android/bitmap.h>
#include "RenderContext.h"
#include "logger.h"
#include "stb_image.h"

class AssetUtil {
public:
    static char *loadTextAsset(const char *path) {
        auto asset = AAssetManager_open(RenderContext::instance()->assetManager, path,
                                        AASSET_MODE_UNKNOWN);
        if (!asset) {
            LOGE("AssetUtil", "loadTextAsset open error");
            return nullptr;
        }
        off_t len = AAsset_getLength(asset);
        char *buffer = new char[len + 1];
        uint32_t num = AAsset_read(asset, buffer, len);
        AAsset_close(asset);
        if (num != len) {
            LOGE("AssetUtil", "loadTextAsset read error");
            delete[] buffer;
            return nullptr;
        }
        buffer[len] = '\0';
        return buffer;
    }

    static unsigned char *loadAsset(const char *path, long *lenPtr) {
        auto asset = AAssetManager_open(RenderContext::instance()->assetManager, path,
                                        AASSET_MODE_UNKNOWN);
        if (!asset) {
            LOGE("AssetUtil", "loadAsset open error");
            return nullptr;
        }
        off_t len = AAsset_getLength(asset);
        *lenPtr = len;
        unsigned char *buffer = new unsigned char[len];
        uint32_t num = AAsset_read(asset, buffer, len);
        AAsset_close(asset);
        if (num != len) {
            LOGE("AssetUtil", "loadAsset read error");
            delete[] buffer;
            return nullptr;
        }
        return buffer;
    }
};


#endif //GLSAMPLE_ASSETUTIL_H
