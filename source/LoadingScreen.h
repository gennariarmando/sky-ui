#pragma once
#ifdef GTA3
#include "PluginBase.h"
#include "CSprite2d.h"
#include "Audio.h"

class CLoadingScreen {
public:
    struct LoadSc {
        std::string slot;
        CSprite2d sprite;

        LoadSc() {
            slot.clear();
            sprite = {};
        }
    };
    static constexpr bool SKIP_EAX_NVIDIA = true;

    static constexpr float FADE_DURATION = 1.0f;
    static constexpr float SPLASH_DURATION = 0.5f;
    static constexpr uint32_t NUM_SPLASHES = 21;
    static constexpr uint32_t NUM_CHUNKS = 80;
    static constexpr float SCREEN_CHANGE_TIME = 4.0f;
    static constexpr int32_t NUM_LOAD_SAMPLES = 2;

    static inline std::array<LoadSc, NUM_SPLASHES> m_aSplashes;

    static inline int32_t m_currDisplayedSplash;
    static inline int32_t m_numChunksLoaded;
    static inline int32_t m_chunkBarAppeared;

    static inline bool m_bActive;
    static inline bool m_bWantToPause;
    static inline bool m_bPaused;
    static inline bool m_bFading;
    static inline bool m_bLegalScreen;
    static inline bool m_bFadeInNextSplashFromBlack;
    static inline bool m_bFadeOutCurrSplashToBlack;
    static inline bool m_bReadyToDelete;

    static inline float m_StartFadeTime;
    static inline float m_ClockTimeOnPause;
    static inline float m_PauseTime;

    static inline float gfLoadingPercentage;
    static inline float m_TimeBarAppeared;

    static inline float m_timeSinceLastScreen;

    static inline uint8_t m_FadeAlpha;

    static inline int32_t m_loadSampleToPlay = -1;
    static inline float m_loadingVolumeMult = 1.0f;
    static inline bool m_loadingDone = false;
    static inline std::thread loadingThread = {};

    static inline std::unique_ptr<plugin::BassSampleManager> sampleManager;

public:
    static void Init(bool loaded);
    static void Shutdown();

    static void RenderSplash();
    static void LoadSplashes(bool starting, bool nvidia);
    static float GetClockTime(bool ignorePauseTime = true);
    static void Continue();
    static void RenderLoadingBar();
    static void StartFading();
    static void DisplayPCScreen();
    static void DoPCTitleFadeOut();
    static void DoPCTitleFadeIn();
    static void DoPCScreenChange(bool finish);
    static void NewChunkLoaded();
    static void DisplayPCScreenFix(double targetDuration, uint8_t* alpha, bool direction, float* attenuation);

    static bool IsActive() {
        return m_bActive;
    }

    static CSprite2d& GetCurrentDisplayedSplash() {
        return m_aSplashes[m_currDisplayedSplash].sprite;
    }

    static CSprite2d& GetGTALogo() {
        return m_aSplashes[NUM_SPLASHES - 1].sprite;
    }

};
#endif
