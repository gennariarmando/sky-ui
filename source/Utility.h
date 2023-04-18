#pragma once
#include "CTimer.h"
#include "extensions/Screen.h"
#include "CSprite2d.h"
#include "CDraw.h"

#define DEFAULT_SCREEN_WIDTH 640.0f
#define DEFAULT_SCREEN_HEIGHT 480.0f
#define DEFAULT_SCREEN_ASPECT_RATIO (DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT)

static float GetAspectRatio() {
#ifdef GTA3
    float& fScreenAspectRatio = *(float*)0x5F53C0;
#elif GTAVC
    float& fScreenAspectRatio = *(float*)0x94DD38;
#elif GTASA
    float& fScreenAspectRatio = CDraw::ms_fAspectRatio;
#endif
    return fScreenAspectRatio;
}

#define SCREEN_ASPECT_RATIO GetAspectRatio() // (SCREEN_WIDTH / SCREEN_HEIGHT)

static float ScaleX(float x) {
    float f = ((x) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

static float ScaleXKeepCentered(float x) {
    float f = ((SCREEN_WIDTH == DEFAULT_SCREEN_WIDTH) ? (x) : (SCREEN_WIDTH - ScaleX(DEFAULT_SCREEN_WIDTH)) / 2 + ScaleX((x)));
    return f;
}

static float ScaleY(float y) {
    float f = ((y) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

static float ScaleW(float w) {
    float f = ((w) * (float)SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH) * DEFAULT_SCREEN_ASPECT_RATIO / SCREEN_ASPECT_RATIO;
    return f;
}

static float ScaleH(float h) {
    float f = ((h) * (float)SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT);
    return f;
}

static void Draw2DPolygon(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    CSprite2d::SetVertices(x1, y1, x2, y2, x3, y3, x4, y4, color, color, color, color);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)(color.a != 255));
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
}

static void RotateVertices(CVector2D* rect, float x, float y, float angle) {
    float xold, yold;
    //angle /= 57.2957795;
    float _cos = cosf(angle);
    float _sin = sinf(angle);
    for (unsigned int i = 0; i < 4; i++) {
        xold = rect[i].x;
        yold = rect[i].y;
        rect[i].x = x + (xold - x) * _cos + (yold - y) * _sin;
        rect[i].y = y - (xold - x) * _sin + (yold - y) * _cos;
    }
}

static void DrawSpriteWithRotation(CSprite2d* sprite, float x, float y, float w, float h, float angle, CRGBA const& col) {
    CVector2D posn[4];
    posn[1].x = x - (w * 0.5f); posn[1].y = y - (h * 0.5f); posn[0].x = x + (w * 0.5f); posn[0].y = y - (h * 0.5f);
    posn[3].x = x - (w * 0.5f); posn[3].y = y + (h * 0.5f);	posn[2].x = x + (w * 0.5f); posn[2].y = y + (h * 0.5f);

    RotateVertices(posn, x, y, angle);

    if (sprite)
        sprite->Draw(
            posn[3].x, posn[3].y, posn[2].x, posn[2].y,
            posn[1].x, posn[1].y, posn[0].x, posn[0].y, CRGBA(col));
    else
        CSprite2d::DrawRect(CRect(x - (w * 0.5f), y - (h * 0.5f), x + (w * 0.5f), y + (h * 0.5f)), col);
}

static void DrawTriangle(float x, float y, float scale, float angle, CRGBA const& col) {
    CVector2D posn[4];
    float w = scale;
    float h = scale;

    posn[1].x = x - (w * 0.5f); posn[1].y = y - (h * 0.5f); posn[0].x = x + (w * 0.5f); posn[0].y = y - (h * 0.5f);
    posn[3].x = x; posn[3].y = y + (h * 0.5f);	posn[2].x = x; posn[2].y = y + (h * 0.5f);

    RotateVertices(posn, x, y, angle);
    Draw2DPolygon(posn[0].x, posn[0].y, posn[1].x, posn[1].y, posn[2].x, posn[2].y, posn[3].x, posn[3].y, CRGBA(col));
}
