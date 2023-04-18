#include "plugin.h"
#include "CMenuManager.h"
#include "CPad.h"
#include "CFont.h"
#include "CText.h"
#include "CStats.h"
#include "CCamera.h"

#ifdef GTA3
#include "cDMAudio.h"
#elif GTAVC
#elif GTASA
#endif

#include "Utility.h"

using namespace plugin;

class SkyUI {
public:
#ifdef GTA3
    static inline ThiscallEvent <AddressList<0x48E721, H_CALL, 0x48C8A4, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x47AB12, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawStandardMenu;
#elif GTAVC

#elif GTASA
    static inline ThiscallEvent <AddressList<0x57BA58, H_CALL>, PRIORITY_AFTER, ArgPick2N<CMenuManager*, 0, uint8_t, 1>, void(CMenuManager*, uint8_t)> onDrawingStandardMenus;
    static inline ThiscallEvent <AddressList<0x57B457, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcessUserInput;
#endif

    inline enum {
        INPUT_TAB,
        INPUT_STANDARD,
    };

    inline enum {
        STATE_NONE = -1,
        STATE_OPENING,
        STATE_CLOSING,
        STATE_IDLE,
    };

    inline enum {
        TAB_STA,
        TAB_SAV,
        TAB_BRI,
        TAB_CON,
        TAB_AUD,
        TAB_DIS,
        TAB_LAN,
    };

    static inline uint8_t currentInput = INPUT_TAB;
    static inline int8_t previousTab = TAB_SAV;
    static inline int8_t currentTab = TAB_SAV;
    static inline uint32_t menuAlpha = 0;
    static inline float menuOffsetX = 0.0f;
    static inline int8_t menuState = STATE_NONE;
    static inline uint32_t previousTimeInMilliseconds = 0;
    static inline int32_t timeToWaitBeforeStateChange = 0;
    static inline std::array<const char*, 6> helpTexts;
    static inline bool menuActive = false;
    static inline bool saveMenuActive = false;

    struct tMenuTab {
        char str[8];
        float x;
        uint8_t targetPage;
    };

    static inline std::vector<tMenuTab> tabs = {
        { "FEB_STA", 88.0f, MENUPAGE_STATS },
        { "FEB_SAV", 154.0f, MENUPAGE_NEW_GAME },
        { "FEB_BRI", 220.0f, MENUPAGE_BRIEFS },
        { "FEB_CON", 286.0f, MENUPAGE_CONTROLLER_PC },
        { "FEB_AUD", 352.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEB_DIS", 418.0f, MENUPAGE_GRAPHICS_SETTINGS },
        { "FEB_LAN", 484.0f, MENUPAGE_LANGUAGE_SETTINGS },
    };

    static inline void UpdateMouse(CMenuManager* _this) {
        if (!_this->m_bShowMouse
            && (_this->m_nMouseOldPosX != _this->m_nMousePosX || _this->m_nMouseOldPosY != _this->m_nMousePosY)) {
            _this->m_bShowMouse = true;
        }

        _this->m_nMouseOldPosX = _this->m_nMousePosX;
        _this->m_nMouseOldPosY = _this->m_nMousePosY;
        _this->m_nMousePosX = _this->m_nMouseTempPosX;
        _this->m_nMousePosY = _this->m_nMouseTempPosY;

        if (_this->m_nMousePosX < 0)
            _this->m_nMousePosX = 0;
        if (_this->m_nMousePosX > SCREEN_WIDTH)
            _this->m_nMousePosX = SCREEN_WIDTH;
        if (_this->m_nMousePosY < 0)
            _this->m_nMousePosY = 0;
        if (_this->m_nMousePosY > SCREEN_HEIGHT)
            _this->m_nMousePosY = SCREEN_HEIGHT;
    }

    static inline uint8_t GetTargetPage() {
        uint8_t targetPage = tabs.at(currentTab).targetPage;
        if (saveMenuActive && targetPage == MENUPAGE_NEW_GAME)
            targetPage = MENUPAGE_SAVE;

        return targetPage;
    }

    static inline uint8_t CheckHover(CMenuManager* _this, float x1, float x2, float y1, float y2) {
        CPad* pad = CPad::GetPad(0);

        uint8_t result = 0;

        if (!_this->m_bShowMouse)
            result = 0;

        if (_this->CheckHover(x1, x2, y1, y2)) {
            result = 1;

            if (pad->NewMouseControllerState.lmb && !pad->OldMouseControllerState.lmb)
                result = 2;
        }

        return result;
    }

    static inline uint8_t CheckHover(CMenuManager* _this, CRect rect) {
        return CheckHover(_this, rect.left, rect.right, rect.top, rect.bottom);
    }

    static inline void ClearInput() {
        CPad* pad = CPad::GetPad(0);
        pad->Clear(1);
        pad->ClearMouseHistory();
    }

    static inline void SwitchMenuPage(CMenuManager* _this, int8_t page, bool refresh) {
;       _this->m_nPreviousMenuPage = _this->m_nCurrentMenuPage;
        _this->m_nCurrentMenuPage = page;

        if (refresh)
            _this->m_nMenuFadeAlpha = 0;
    }

    static inline void SwitchTab(CMenuManager* _this, int8_t tab) {
        _this->m_nCurrentMenuEntry = 0;
        previousTab = currentTab;
        currentTab = tab;
        DMAudio.PlayFrontEndSound(151, 0);
    }

    static inline uint32_t GetAlpha(uint32_t a = 255) {
        return min(menuAlpha, a);
    }

    static inline void EnterTab(CMenuManager* _this, int32_t i = -1) {
        if (saveMenuActive)
            return;

        if (i != -1)
            currentTab = i;
        else
            i = currentTab;


        if (i == TAB_STA || i == TAB_BRI)
            return;

        SwitchMenuPage(_this, GetTargetPage(), false);

        currentInput = INPUT_STANDARD;
        ClearInput();

        if (i == TAB_AUD) {
            DMAudio.PlayFrontEndTrack(_this->m_nPrefsRadioStation, 1);
        }

        DMAudio.PlayFrontEndSound(150, 0);
    }

    static inline void ChangeState(int8_t state) {
        menuState = state;
        timeToWaitBeforeStateChange = CTimer::m_snTimeInMillisecondsPauseMode + 500;
    }

    static inline void EscTab(CMenuManager* _this) {
        if (saveMenuActive) {
            ChangeState(STATE_CLOSING);
            return;
        }

        currentInput = INPUT_TAB;
        ClearInput();
        DMAudio.StopFrontEndTrack();
        DMAudio.PlayFrontEndSound(153, 0);
    }

    static inline void EscMenuCompletely(CMenuManager* _this) {
        _this->RequestFrontEndShutdown();
        _this->SwitchMenuOnAndOff();
        ClearInput();
        Clear(_this);
        DMAudio.PlayFrontEndSound(149, 0);
    }

    static inline void UpdateTab(CMenuManager* _this) {
        CPad* pad = CPad::GetPad(0);
        const bool left = pad->NewKeyState.left && !pad->OldKeyState.left;
        const bool right = pad->NewKeyState.right && !pad->OldKeyState.right;
        const bool enter = pad->NewKeyState.extenter && !pad->OldKeyState.extenter;
        const bool esc = pad->NewKeyState.esc && !pad->OldKeyState.esc;

        if (left) {
            SwitchTab(_this, currentTab - 1);

            if (!IsTabAvailable(_this, currentTab))
                currentTab--;
        }
        else if (right) {
            SwitchTab(_this, currentTab + 1);

            if (!IsTabAvailable(_this, currentTab))
                currentTab++;
        }

        while (true) {
            if (currentTab < 0) {
                previousTab = 0;
                currentTab = tabs.size() - 1;
            }
            else if (currentTab > tabs.size() - 1) {
                previousTab = tabs.size() - 1;
                currentTab = 0;
            }

            if (IsTabAvailable(_this, currentTab))
                break;
            else
                currentTab++;
        }

        if (_this->m_nCurrentMenuPage != GetTargetPage())
            SwitchMenuPage(_this, GetTargetPage(), true);
        else {
            float x1 = ScaleXKeepCentered(32.0f);
            float x2 = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 32.0f);
            float y1 = ScaleY(32.0f);
            float y2 = ScaleY(DEFAULT_SCREEN_HEIGHT - 32.0f - 92.0f);

            if (enter || (CheckHover(_this, x1, x2, y1, y2) == 2)) {
                EnterTab(_this);
            }
            else if (esc && !_this->m_bGameNotLoaded) {
                ChangeState(STATE_CLOSING);
            }
        }
    }

    static inline void UpdateStandard(CMenuManager* _this) {
        const CPad* pad = CPad::GetPad(0);
        const bool esc = pad->NewKeyState.esc && !pad->OldKeyState.esc;

        if (_this->m_nCurrentMenuPage != GetTargetPage()) {

        }
        else {
            if (esc) {
                EscTab(_this);
            }
        }
    }

    static inline void Process(CMenuManager* _this) {
        switch (currentTab) {
        default:
            SetHelpText(0, "FEDS_SE");
            SetHelpText(1, "FEDS_BA");
            SetHelpText(2, "FEDS_ST");

            if (currentInput == INPUT_TAB)
                SetHelpText(4, "FEDS_AM");
            else
                SetHelpText(4, "FEDSAS4");
            break;
        case TAB_STA:
            SetHelpText(0, "FEDS_ST");
            SetHelpText(1, "FEDS_AM");

            SetHelpText(3, "FEDSSC1");
            SetHelpText(4, "FEDSSC2");
            break;
        case TAB_BRI:
            SetHelpText(0, "FEDS_ST");
            SetHelpText(4, "FEDS_AM");
            break;
        }

        if (timeToWaitBeforeStateChange != -1 && timeToWaitBeforeStateChange < CTimer::m_snTimeInMillisecondsPauseMode) {
            switch (menuState) {
            case STATE_NONE:
                break;
            case STATE_OPENING: {
                if (menuOffsetX > 0.0f)
                    menuOffsetX -= (CTimer::m_snTimeInMillisecondsPauseMode - previousTimeInMilliseconds) * 1.0f;

                float alpha = (1.0f - (menuOffsetX / (DEFAULT_SCREEN_WIDTH)));
                alpha = max(alpha, 0.0f);
                menuAlpha = 255 * alpha;

                if (menuAlpha >= 255) {
                    menuAlpha = 255;
                    menuOffsetX = 0.0f;
                    ChangeState(STATE_IDLE);
                }
            } break;
            case STATE_CLOSING: {
                if (menuOffsetX < DEFAULT_SCREEN_WIDTH)
                    menuOffsetX += (CTimer::m_snTimeInMillisecondsPauseMode - previousTimeInMilliseconds) * 1.0f;

                float alpha = (1.0f - (menuOffsetX / (DEFAULT_SCREEN_WIDTH)));
                alpha = max(alpha, 0.0f);
                menuAlpha = 255 * alpha;

                if (menuAlpha <= 0) {
                    menuAlpha = 0;
                    menuOffsetX = DEFAULT_SCREEN_WIDTH;
                    ChangeState(STATE_NONE);
                    EscMenuCompletely(_this);
                }
            } break;
            case STATE_IDLE: {
                switch (currentInput) {
                case INPUT_TAB:
                    UpdateMouse(_this);
                    UpdateTab(_this);
                    break;
                case INPUT_STANDARD:
                    UpdateStandard(_this);
                    break;
                }
            } break;
            }
            timeToWaitBeforeStateChange = CTimer::m_snTimeInMillisecondsPauseMode;
        }

        previousTimeInMilliseconds = CTimer::m_snTimeInMillisecondsPauseMode;
    }

    static inline CRect Translate(CRect& rect) {
        rect.left += ScaleX(menuOffsetX);
        rect.right += ScaleX(menuOffsetX);

        float off = 0.0005f;
        rect.left += off;
        rect.top += off;
        rect.right -= off;
        rect.bottom -= off;

        return rect;
    }

    static inline void DrawRadioIcon(CMenuManager* _this, int8_t id, float x, float y, float scale) {
        if (id == 9)
            _this->m_aMenuSprites[MENUSPRITE_MP3LOGO].Draw(x, y, scale, scale, id == _this->m_nPrefsRadioStation ? CRGBA(255, 255, 255, GetAlpha()) : CRGBA(255, 255, 255, GetAlpha(55)));
        else {
            uint8_t sprite = FE_RADIO1;
            switch (id) {
            case 0:
                sprite = FE_RADIO1;
                break;
            case 1:
                sprite = FE_RADIO2;
                break;
            case 2:
                sprite = FE_RADIO5;
                break;
            case 3:
                sprite = FE_RADIO7;
                break;
            case 4:
                sprite = FE_RADIO8;
                break;
            case 5:
                sprite = FE_RADIO3;
                break;
            case 6:
                sprite = FE_RADIO4;
                break;
            case 7:
                sprite = FE_RADIO6;
                break;
            case 8:
                sprite = FE_RADIO9;
                break;
            }
            _this->m_aFrontEndSprites[sprite].Draw(x, y, scale, scale, id == _this->m_nPrefsRadioStation ? CRGBA(255, 255, 255, GetAlpha()) : CRGBA(100, 100, 100, GetAlpha(100)));
        }

        if (CheckHover(_this, x, x + scale, y, y + scale) == 2)
            _this->m_nPrefsRadioStation = id;
    }

    static inline void DrawSlider(float x, float y, float progress) {
        float _x = (x);
        float _y = (y);
        float _w = ScaleX(3.5f);
        float _h = ScaleY(1.0f);

        for (uint32_t i = 0; i < 16; i++) {
            CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), CRGBA(20, 94, 136, GetAlpha()));

            if (i / 16.0f + 1 / 32.0f < progress)
                CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), CRGBA(86, 196, 255, GetAlpha()));

            _x += _w + _w;
            _h += ScaleY(1.0f);
        }
    }
    
    static inline void SetHelpText(uint8_t id, const char* str) {
        helpTexts[id] = str;
    }

    static inline void DrawHelpText() {
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(0);
        CFont::SetFontStyle(0);
        CFont::SetScale(ScaleX(0.36f), ScaleY(0.6f));
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha()));

        float startx = ScaleXKeepCentered(50.0f + menuOffsetX);
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 94.0f);
        float x = startx;
        float y = starty;

        uint32_t i = 0;
        for (auto& it : helpTexts) {
            if (it)
                CFont::PrintString(x, y, TheText.Get(it));

            y += ScaleY(12.0f);

            if (i == 2) {
                x += ScaleX(198.0f);
                y = starty;
            }

            it = nullptr;
            i++;
        }
    }

    static inline void DrawBack(CMenuManager* _this) {
        CSprite2d::DrawRect(CRect(-5.0f, -5.0f, SCREEN_WIDTH + 5.0f, SCREEN_HEIGHT + 5.0f), CRGBA(0, 0, 0, 255));

        CSprite2d* splash = plugin::CallAndReturn<CSprite2d*, 0x48D550>("splash1"); // LoadSplash
        splash->Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(50, 50, 50, 255));

        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

        CRect rect = {};
        CRGBA col = CRGBA(255, 255, 255, GetAlpha());

        rect.left = ScaleXKeepCentered(0.0f);
        rect.top = ScaleY(0.0f);
        rect.right = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH * 0.5f);
        rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT * 0.5f);
        Translate(rect);

        _this->m_aFrontEndSprites[FE2_MAINPANEL_UL].Draw(rect, col);

        rect.left = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH * 0.5f);
        rect.top = ScaleY(0.0f);
        rect.right = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH);
        rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT * 0.5f);
        Translate(rect);

        _this->m_aFrontEndSprites[FE2_MAINPANEL_UR].Draw(rect, col);

        rect.left = ScaleXKeepCentered(0.0f);
        rect.top = ScaleY(DEFAULT_SCREEN_HEIGHT * 0.5f);
        rect.right = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH * 0.5f);
        rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT);
        Translate(rect);

        _this->m_aFrontEndSprites[FE2_MAINPANEL_DL].Draw(rect, col);

        rect.left = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH * 0.5f);
        rect.top = ScaleY(DEFAULT_SCREEN_HEIGHT * 0.5f);
        rect.right = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH);
        rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT);
        Translate(rect);

        _this->m_aFrontEndSprites[FE2_MAINPANEL_DR].Draw(rect, col);

        uint8_t sprite = FE_ICONBRIEF;
        switch (currentTab) {
        case TAB_STA:
        case TAB_SAV:
        case TAB_BRI:
            break;
        case TAB_CON:
            sprite = FE_ICONCONTROLS;
            break;
        case TAB_AUD:
            sprite = FE_ICONAUDIO;
            break;
        case TAB_DIS:
            sprite = FE_ICONDISPLAY;
            break;
        case TAB_LAN:
            sprite = FE_ICONLANGUAGE;
            break;
        };

        const float x1 = 48.0f;
        const float x2 = 48.0f;
        const float y1 = 58.0f;
        const float y2 = 108.0f;
        rect.left = ScaleXKeepCentered(x1);
        rect.top = ScaleY(y1);
        rect.right = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - x2);
        rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT - y2);
        Translate(rect);

        _this->m_aFrontEndSprites[sprite].Draw(rect, CRGBA(0, 0, 0, GetAlpha(190)));

        if (currentInput == INPUT_STANDARD) {
            rect.left = ScaleXKeepCentered(62.0f);
            rect.top = ScaleY(DEFAULT_SCREEN_HEIGHT - 42.5f);
            rect.right = ScaleXKeepCentered(62.0f + 518.0f);
            rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT - 38.0f + 13.5f);
            Translate(rect);

            CSprite2d::DrawRect(rect, CRGBA(41, 102, 103, GetAlpha())); // green

            rect.left = ScaleXKeepCentered(62.0f);
            rect.top = ScaleY(DEFAULT_SCREEN_HEIGHT - 42.5f);
            rect.right = ScaleXKeepCentered(62.0f + 518.0f);
            rect.bottom = ScaleY(DEFAULT_SCREEN_HEIGHT - 42.5f + 3.5f); // grey
            Translate(rect);

            CSprite2d::DrawRect(rect, CRGBA(38, 48, 56, GetAlpha()));
        }

        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

        // Radio icons
        if (_this->m_nCurrentMenuPage == MENUPAGE_SOUND_SETTINGS) {
            float start = ScaleXKeepCentered(144.0f + menuOffsetX);
            float x = start;
            float y = ScaleY(218.0f);
            float scale = ScaleY(70.0);
            float scaling = scale + ScaleX(2.0f);
            bool mp3 = DMAudio.IsMP3RadioChannelAvailable();

            for (uint32_t i = 0; i < 10; i++) {
                if (i == 9 && !mp3)
                    break;

                DrawRadioIcon(_this, i, x, y, scale);
                x += scaling;

                if (i == 4) {
                    x = start;
                    if (!mp3)
                        x += scaling / 2;
                    y += scaling;
                }
            }
        }

        // Sliders 
        for (int i = 0; i < 18; i++) {
            switch (aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_Action) {
            case MENUACTION_BRIGHTNESS:
                DrawSlider(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + menuOffsetX), ScaleY(92.0f), _this->m_nPrefsBrightness / 512.0f);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + menuOffsetX), ScaleY(92.0f + 22.0f), _this->m_fPrefsLOD - 0.8f * 1.0f);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + menuOffsetX), ScaleY(92.0f), _this->m_nPrefsMusicVolume / 128.0f);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + menuOffsetX), ScaleY(92.0f + 22.0f), _this->m_nPrefsSfxVolume / 128.0f);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + menuOffsetX), ScaleY(92.0f), TheCamera.m_fMouseAccelHorzntal * 200.0f);
                break;
            }
        }
    }

    static inline bool IsTabAvailable(CMenuManager* _this, int32_t tab) {
        if (_this->m_bGameNotLoaded && (tab == TAB_STA || tab == TAB_BRI)) {
            return false;
        }

        return true;
    }

    static inline void DrawFront(CMenuManager* _this) {
        if (timeToWaitBeforeStateChange == -1)
            return;
            
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

        DrawHelpText();

        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(0);
        CFont::SetFontStyle(0);
        CFont::SetScale(ScaleX(0.28f), ScaleY(0.54f));

        const float x = ScaleXKeepCentered(4.0f + menuOffsetX);
        const float y = ScaleY(DEFAULT_SCREEN_HEIGHT - 40.0f);

        int32_t i = 0;
        int32_t j = 0;
        CRGBA c = {};
        float it_x = 0.0f;
        for (auto& it : tabs) {
            if (!IsTabAvailable(_this, i)) {
                if (i == j && j != 0)
                    j--;

                i++;
                j++;
                continue;
            }
            it_x = tabs.at(j).x;

            RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

            CRect rect;
            rect.left = x + ScaleX(it_x - 4.0f);
            rect.top = y + ScaleY(-3.5f);
            rect.right = rect.left + ScaleX(130.0f);
            rect.bottom = rect.top + ScaleY(36.5f);

            c = CRGBA(0, 0, 0, GetAlpha());

            if (i == currentTab)
                _this->m_aFrontEndSprites[FE2_TABACTIVE].Draw(rect, CRGBA(255, 255, 255, GetAlpha()));

            if (currentInput != INPUT_STANDARD || i == currentTab) {
                rect.left = x + ScaleX(it_x - 4.0f);
                rect.top = y + ScaleY(-3.0f);
                rect.right = rect.left + ScaleX(64.0f);
                rect.bottom = rect.top + ScaleY(22.0f);
                uint8_t hover = CheckHover(_this, rect);

                if (hover) {
                    c = CRGBA(205, 205, 205, GetAlpha());

                    if (hover == 2) {
                        SwitchTab(_this, i);
                    }
                }

                CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
                CFont::SetColor(c);

                CFont::PrintString(x + ScaleX(it_x), y, TheText.Get(it.str));
            }
            i++;
            j++;
        }
    }

    static inline void Init(CMenuManager* _this) {
        for (int i = 0; i < MENUPAGE_58; i++) {
            for (int j = 0; j < 18; j++) {
                if (!strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FEDS_TB") ||
                    !strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FET_DEF")) {
                    aScreens[i].m_aEntries[j].m_Action = MENUACTION_NOTHING;
                    aScreens[i].m_aEntries[j].m_EntryName[0] = '\0';
                    aScreens[i].m_aEntries[j].m_nSaveSlot = 0;
                    aScreens[i].m_aEntries[j].m_nTargetMenu = MENUPAGE_NONE;
                }
            }
        }

        aScreens[MENUPAGE_EXIT].m_nPreviousPage[0] = MENUPAGE_NEW_GAME;
        aScreens[MENUPAGE_EXIT].m_nPreviousPage[1] = MENUPAGE_NEW_GAME;

        aScreens[MENUPAGE_EXIT].m_nParentEntry[0] = 3;
        aScreens[MENUPAGE_EXIT].m_nParentEntry[1] = 3;

        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_Action = MENUACTION_CHANGEMENU;
        strcpy(aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_EntryName, "FET_QG");
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nSaveSlot = 0;
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nTargetMenu = MENUPAGE_EXIT;

        //Clear(_this);
    }

    static inline void Clear(CMenuManager* _this, bool run = false) {
        if (saveMenuActive)
            currentInput = INPUT_STANDARD;
        else
            currentInput = INPUT_TAB;
        //currentTab = 0;
        menuOffsetX = 0.0f;
        menuAlpha = 0;
        previousTimeInMilliseconds = CTimer::m_snTimeInMillisecondsPauseMode;

        if (run) {
            ChangeState(STATE_OPENING);
            menuOffsetX = DEFAULT_SCREEN_WIDTH;
        }
        else
            ChangeState(STATE_NONE);

        if (_this->m_nCurrentMenuPage != GetTargetPage())
            SwitchMenuPage(_this, GetTargetPage(), false);
    }

    static inline void Shutdown(CMenuManager* _this) {

    }

    SkyUI() {
#ifdef GTA3
        // No green bar
        plugin::patch::Nop(0x47C597, 5);

        auto drawHeader = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetScale(ScaleX(0.8f), ScaleY(1.2f));
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 44.0f + menuOffsetX), ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f), TheText.Get(tabs.at(currentTab).str));
        };
        plugin::patch::RedirectCall(0x47AF76, LAMBDA(void, __cdecl, drawHeader, float, float, wchar_t*));

        auto drawAction = [](float, float, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + menuOffsetX), ScaleY(64.0f), str);
        };
        plugin::patch::RedirectCall(0x47B132, LAMBDA(void, __cdecl, drawAction, float, float, wchar_t*));

        auto drawBriefs = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(52.0f + menuOffsetX), y + ScaleY(38.0f), str);
        };
        plugin::patch::RedirectCall(0x484F3A, LAMBDA(void, __cdecl, drawBriefs, float, float, wchar_t*));

        auto drawLeftString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

            float center = 0.0f;

            if (CFont::Details.m_bCentre)
                center = 240.0f;

            CFont::PrintString(ScaleXKeepCentered(64.0f + center + menuOffsetX), y, str);
        };
        plugin::patch::RedirectCall(0x47C666, LAMBDA(void, __cdecl, drawLeftString, float, float, wchar_t*));

        auto drawRightString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX), y, str);
        };
        plugin::patch::RedirectCall(0x47C74C, LAMBDA(void, __cdecl, drawRightString, float, float, wchar_t*));
        plugin::patch::SetChar(0x47DA11 + 6, 255);

        // Stats
        auto drawCrimRa = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + menuOffsetX), ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x4825AD, LAMBDA(void, __cdecl, drawCrimRa, float, float, wchar_t*));

        auto drawRate = [](float, float, wchar_t* str) {
            char buff[32];
            sprintf(buff, "%d", CStats::FindCriminalRatingNumber());

            float x = CFont::GetStringWidth(buff, 1);
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX - 16.0f) - x, ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x482620, LAMBDA(void, __cdecl, drawRate, float, float, wchar_t*));

        auto drawRate2 = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX), ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x4826D0, LAMBDA(void, __cdecl, drawRate2, float, float, wchar_t*));

        auto drawStatLeft = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + menuOffsetX), y + ScaleY(30.0f), str);
        };
        plugin::patch::RedirectCall(0x4824E1, LAMBDA(void, __cdecl, drawStatLeft, float, float, wchar_t*));


        auto drawStatRight = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + menuOffsetX), y + ScaleY(30.0f), str);
        };
        plugin::patch::RedirectCall(0x482527, LAMBDA(void, __cdecl, drawStatRight, float, float, wchar_t*));

        plugin::patch::Nop(0x4827AE, 5); // No stats header

        // No help text
        plugin::patch::Nop(0x47F107, 7);

        // No sliders
        plugin::patch::RedirectJump(0x47CA11, (void*)0x47D9B9);

#elif GTASA
        // No half size map key.
plugin::patch::Nop(0x57516D, 4);

        // No help text
        plugin::patch::Nop(0x57E3AE, 5);
#endif

        plugin::Events::initRwEvent += []() {
            Init(&FrontEndMenuManager);
        };

        onProcess.before += [](CMenuManager* _this) {
            if (_this->m_bMenuActive) {
                if (!menuActive) {
                    Clear(_this, true);
                    menuActive = true;
                }
            }
            else if (_this->m_bSaveMenuActive) {
                if (!saveMenuActive) {
                    Clear(_this, true);
                    saveMenuActive = true;
                }
            }
            else {
                saveMenuActive = false;
                menuActive = false;
                Clear(_this, false);
            }

            if (menuActive || saveMenuActive) 
                Process(_this);
        };

        onDrawStandardMenu.before += [](CMenuManager* _this) {
            DrawBack(_this);
        };

        onDrawStandardMenu.after += [](CMenuManager* _this) {
            DrawFront(_this);
        };

        auto processButtonPresses = [](CMenuManager* _this, uint32_t) {
            if (currentInput == INPUT_STANDARD)
                _this->ProcessButtonPresses();
        };
        plugin::patch::RedirectCall(0x4852FC, LAMBDA(void, __fastcall, processButtonPresses, CMenuManager*, uint32_t));
    }

    ~SkyUI() {

    }
} skyUI;
