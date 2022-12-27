//
// Created by admin on 2022/12/26.
//

#include "core/advanced/GlyphOutline.h"
#include "core/logger.h"

void GlyphOutline::decompose(FT_GlyphSlot glyph) {
    FT_Outline_Funcs callbacks;
    callbacks.move_to = [](const FT_Vector *to,
                           void *user) {
        auto decomposer = static_cast<GlyphOutline* >(user);
        decomposer->tags.push_back(0);
        glm::vec2 point((to->x >> 6) * 1.0f, (to->y >> 6) * 1.0f);
        decomposer->current = point;
        return 0;
    };

    callbacks.line_to = [](const FT_Vector *to,
                           void *user) {
        auto decomposer = static_cast<GlyphOutline* >(user);
        decomposer->tags.push_back(1);
        glm::vec2 point((to->x >> 6) * 1.0f, (to->y >> 6) * 1.0f);
        decomposer->points.push_back(decomposer->current);
        // 两个假的控制点
        decomposer->points.emplace_back(-1.0f, -1.0f);
        decomposer->points.emplace_back(-1.0f, -1.0f);
        decomposer->current = point;
        decomposer->points.push_back(point);
        return 0;
    };

    callbacks.conic_to = [](const FT_Vector *control,
                            const FT_Vector *to,
                            void *user) {
        auto decomposer = static_cast<GlyphOutline* >(user);
        decomposer->tags.push_back(2);
        glm::vec2 point((to->x >> 6) * 1.0f, (to->y >> 6) * 1.0f);
        // 上一个点
        decomposer->points.push_back(decomposer->current);
        // 控制点
        decomposer->points.emplace_back((control->x >> 6) * 1.0f, (control->y >> 6) * 1.0f);
        decomposer->points.emplace_back(-1.0f, -1.0f);
        decomposer->current = point;
        decomposer->points.push_back(point);
        return 0;
    };

    callbacks.cubic_to = [](const FT_Vector *control1,
                            const FT_Vector *control2,
                            const FT_Vector *to,
                            void *user) {
        auto decomposer = static_cast<GlyphOutline* >(user);
        glm::vec2 point((to->x >> 6) * 1.0f, (to->y >> 6) * 1.0f);
        // 上一个点
        decomposer->points.push_back(decomposer->current);
        // 控制点
        decomposer->points.emplace_back((control1->x >> 6) * 1.0f, (control1->y >> 6) * 1.0f);
        decomposer->points.emplace_back((control2->x >> 6) * 1.0f, (control2->y >> 6) * 1.0f);
        decomposer->current = point;
        decomposer->points.push_back(point);
        return 0;
    };

    int error = FT_Outline_Decompose(&glyph->outline, &callbacks, this);
    if (error)
        LOGE("GlyphOutline", "Couldn't extract the outline: FT_Outline_Decompose() failed");
}
