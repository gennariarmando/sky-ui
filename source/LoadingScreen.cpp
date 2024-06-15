#ifdef GTA3
#include "plugin.h"
#include "LoadingScreen.h"
#include "CTxdStore.h"
#include "CFileMgr.h"
#include "CTimer.h"
#include "Utility.h"
#include "Colors.h"
#include "CScene.h"
#include "CGeneral.h"
#include "CMenuManager.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

void CLoadingScreen::Init(bool loaded) {
    if (IsActive()) {
        return;
    }

    if (!loaded) {
        LoadSplashes(false, false);
    }

    m_currDisplayedSplash = -1;
    m_timeSinceLastScreen = GetClockTime();
    m_FadeAlpha = 0;
    m_bActive = true;
}

void CLoadingScreen::Shutdown() {
    if (!IsActive()) {
        return;
    }

    for (auto& splash : m_aSplashes) {
        splash.sprite.Delete();

        int32_t slot = CTxdStore::FindTxdSlot(splash.slot.c_str());
        if (slot != -1) {
            CTxdStore::RemoveTxdSlot(slot);
        }
    }

    m_bActive = false;
}

void CLoadingScreen::RenderSplash() {
    CSprite2d::InitPerFrame();
    CRect rect(-5.0f, -5.0f, SCREEN_WIDTH + 5.0f, SCREEN_HEIGHT + 5.0f);
    CRGBA color(255, 255, 255, 255);
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);

    if (m_bFading) {
        GetCurrentDisplayedSplash().Draw(rect, color);

        if (m_bFadeInNextSplashFromBlack || m_bFadeOutCurrSplashToBlack) {
            color.Set(0, 0, 0);
            color.a = (m_bFadeInNextSplashFromBlack) ? 255 - m_FadeAlpha : m_FadeAlpha;

            CSprite2d::DrawRect(rect, color);
        }
        else {
            color.a = 255 - m_FadeAlpha;

            m_aSplashes[m_currDisplayedSplash - 1].sprite.Draw(rect, color);
        }
    }
    else if (!m_bReadyToDelete) {
        GetCurrentDisplayedSplash().Draw(rect, color);
    }
}

void CLoadingScreen::LoadSplashes(bool starting, bool nvidia) {
    int32_t numSplashes = starting ? 1 : (NUM_SPLASHES);

    uint8_t screenIdx[NUM_SPLASHES];
    std::iota(std::begin(screenIdx), std::end(screenIdx), 0);

    LARGE_INTEGER pc;
    QueryPerformanceCounter(&pc);
    srand(pc.u.LowPart);

    if (!starting)
        std::shuffle(std::begin(screenIdx) + 1, std::end(screenIdx) - 1, std::mt19937{ std::random_device{}() });

    CFileMgr::SetDir("TXD\\");

    std::fill(m_aSplashes.begin(), m_aSplashes.end(), LoadSc());

    for (int32_t i = 0; i < numSplashes; i++) {
        std::string slotName;
        if (starting) {
            slotName = nvidia ? "nvidia" : "eax";
        }
        else if (i >= numSplashes - 1) {
            slotName = "gtalogo";
        }
        else {
            slotName = std::format("loadsc{}", screenIdx[i]);
        }

        std::string txdName = slotName;
        txdName += ".txd";

        int32_t slot = CTxdStore::AddTxdSlot(slotName.c_str());
        CTxdStore::LoadTxd(slot, txdName.c_str());
        CTxdStore::AddRef(slot);
        CTxdStore::PushCurrentTxd();
        CTxdStore::SetCurrentTxd(slot);

        m_aSplashes[i].slot = slotName;
        m_aSplashes[i].sprite.SetTexture((char*)slotName.c_str());

        CTxdStore::PopCurrentTxd();
    }
    CFileMgr::SetDir("");
}

float CLoadingScreen::GetClockTime(bool ignorePauseTime) {
    float time = (float)RsTimer() / 1000.0f;
    return ignorePauseTime ? time : time - m_PauseTime;
}

void CLoadingScreen::Continue() {
    if (!IsActive())
        return;

    m_bWantToPause = false;
    if (m_bPaused) {
        m_bPaused = false;
        m_PauseTime = GetClockTime() - m_ClockTimeOnPause + m_PauseTime;
    }
}

void CLoadingScreen::RenderLoadingBar() {
    if (m_TimeBarAppeared == 0.0f) {
        m_TimeBarAppeared = GetClockTime();
    }

    if (m_bLegalScreen || gfLoadingPercentage <= 0.0f || gfLoadingPercentage >= 100.0f)
        return;

    GetGTALogo().Draw(ScaleX(44.0f - 24.0f), SCREEN_HEIGHT - ScaleY(56.0f + 192.0f), ScaleX(226.0f), ScaleY(226.0f), CRGBA(255, 255, 255, 255));
    DrawProgressBar(ScaleX(44.0f), SCREEN_HEIGHT - ScaleY(56.0f), ScaleX(172.0f), ScaleY(10.0f), gfLoadingPercentage / 100.0f, CRGBA(HUD_COLOUR_RED_LC01, 255), CRGBA(HUD_COLOUR_GREYDARK, 255));
}

void CLoadingScreen::StartFading() {
    m_bFading = true;
    m_FadeAlpha = 0;
    m_StartFadeTime = GetClockTime(false);
}

void CLoadingScreen::DisplayPCScreen() {
    if (RwCameraBeginUpdate(Scene.m_pCamera)) {
        DefinedState2d();
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)(TRUE));
        RenderSplash();
        if (m_currDisplayedSplash > 0 && (!m_bFading || m_currDisplayedSplash != 1)) {
            RenderLoadingBar();
        }
        RwCameraEndUpdate(Scene.m_pCamera);
        RsCameraShowRaster(Scene.m_pCamera);
    }
}

void CLoadingScreen::DoPCTitleFadeOut() {
    m_bFadeInNextSplashFromBlack = true;
    m_currDisplayedSplash = 0;
    m_bFading = true;

    m_FadeAlpha = 255;

    for (uint32_t i = SPLASH_DURATION; i; i--) {
        DisplayPCScreen();
    }

    for (uint32_t i = FADE_DURATION; i; i--) {
        m_FadeAlpha = (uint8_t)((i / (float)FADE_DURATION) * 250);
        DisplayPCScreen();
    }

    m_FadeAlpha = 0;
    m_bFadeInNextSplashFromBlack = true;
}

void CLoadingScreen::DoPCTitleFadeIn() {
    m_bFadeInNextSplashFromBlack = true;
    m_currDisplayedSplash = 0;
    m_bFading = true;

    for (uint32_t i = 0; i < FADE_DURATION; i++) {
        m_FadeAlpha = (uint8_t)((i / (float)FADE_DURATION) * 250);
        DisplayPCScreen();
    }
    m_FadeAlpha = 255;

    DisplayPCScreen();

    m_bFading = false;
}

void CLoadingScreen::DoPCScreenChange(bool finish) {
    if (!loadingThread.joinable() && m_loadingVolumeMult > 0.0f) {
        loadingThread = std::thread([&]() {
            if (RsGlobal.quit)
                return;

            if (m_loadSampleToPlay == -1) {
                int32_t res = std::uniform_int<int32_t>(0, NUM_LOAD_SAMPLES - 1)(std::mt19937{ std::random_device{}() });
                m_loadSampleToPlay = res;
            }

            while (m_loadingVolumeMult > 0.0f) {
                CLoadingScreen::sampleManager->AddSampleToQueue(1, (uint8_t)(80 * m_loadingVolumeMult), 0, m_loadSampleToPlay, true, {}, 8, true);
                CLoadingScreen::sampleManager->Process();
            }
        });
    }

    m_bFading = true;

    if (finish) {
        m_bFadeOutCurrSplashToBlack = true;
    }
    else {
        m_currDisplayedSplash = std::max((m_currDisplayedSplash + 1) % (std::size(m_aSplashes) - 1), (uint32_t)1);
    }

    m_FadeAlpha = 0;

    for (uint32_t i = SPLASH_DURATION; i > 0; i--) {
        DisplayPCScreen();
    }

    for (uint32_t i = 0; i < FADE_DURATION; i++) {
        float alpha = ((i / (float)FADE_DURATION) * 250.0f);
        m_FadeAlpha = (uint8_t)alpha;

        if (finish || m_bFadeOutCurrSplashToBlack) {
            float amplitude = m_bFadeOutCurrSplashToBlack ? (255.0f - alpha) / 255.0f : 1.0f;
            m_loadingVolumeMult = amplitude;
        }

        DisplayPCScreen();
    }
    m_FadeAlpha = 255;
    DisplayPCScreen();

    m_bFadeInNextSplashFromBlack = false;
    m_bFading = false;

    if (finish) {
        m_loadingVolumeMult = 0.0f;
        if (loadingThread.joinable()) {
            loadingThread.join();
        }
        Shutdown();
    }
}

void CLoadingScreen::NewChunkLoaded() {
    if (!IsActive())
        return;

    ++m_numChunksLoaded;
    if (m_chunkBarAppeared != -1) {
        gfLoadingPercentage = (float)(m_numChunksLoaded - m_chunkBarAppeared) / ((float)NUM_CHUNKS - (float)m_chunkBarAppeared) * 100.0f;
    }

    auto now = GetClockTime();
    auto delta = now - m_timeSinceLastScreen;

    if (m_numChunksLoaded == NUM_CHUNKS) {
        return DoPCScreenChange(true);
    }

    if (m_currDisplayedSplash && delta < SCREEN_CHANGE_TIME) {
        DisplayPCScreen();
    }
    else {
        DoPCScreenChange(false);
        m_timeSinceLastScreen = now;

        if (m_chunkBarAppeared == -1) {
            m_chunkBarAppeared = m_numChunksLoaded;
        }
    }
}
#endif
