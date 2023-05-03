#ifdef GTA3
#define GTA3_MENU_MAP
//#define LCSFICATION
#endif

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
#include "cDMAudio.h"
#elif GTASA
#include "CAudioEngine.h"
#endif

#include "Utility.h"

#include "ModuleList.hpp"

using namespace plugin;

class __declspec(dllexport) SkyUI {
public:
#ifdef GTA3
    static inline ThiscallEvent <AddressList<0x48E721, H_CALL, 0x48C8A4, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x47AB12, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawStandardMenu;
#elif GTAVC
    static inline ThiscallEvent <AddressList<0x4A3299, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawRedefinePage;
    static inline ThiscallEvent <AddressList<0x4A4433, H_CALL, 0x4A5C88, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x4A325E, H_CALL, 0x4A32AD, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*, int)> onDrawStandardMenu;
#elif GTASA
    static inline ThiscallEvent <AddressList<0x57BA58, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*, uint8_t)> onDrawStandardMenu;
    static inline ThiscallEvent <AddressList<0x53BF44, H_CALL, 0x53E7A5, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
#endif

#define HUD_COLOUR_GREENLIGHT 25, 130, 70
#define HUD_COLOUR_ORANGE 144, 98, 16
#define HUD_COLOUR_ORANGELIGHT 210, 150, 25
#define HUD_COLOUR_WHITE 225, 225, 225
#define HUD_COLOUR_PINK 225, 225, 225
#define HUD_COLOUR_BLUELIGHT 86, 196, 255 
#define HUD_COLOUR_BLUEDARK 20, 94, 136
#define HUD_COLOUR_BLUEWHITE 164, 196, 232
#define HUD_COLOUR_AZURE2 49, 101, 148
#define HUD_COLOUR_GREY 150, 150, 150

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

#ifdef GTA3
#ifdef GTA3_MENU_MAP
    inline enum {
        MENUPAGE_MAP = MENUPAGE_NO_MEMORY_CARD,
    };
#endif
#endif

    inline enum {
#ifdef GTA3
#ifdef GTA3_MENU_MAP
        TAB_MAP,
#endif
        TAB_STA,
        TAB_SAV,
        TAB_BRI,
        TAB_CON,
        TAB_AUD,
        TAB_DIS,
        TAB_LAN,
#elif GTAVC
        TAB_MAP,
        TAB_BRI,
        TAB_SAV,
        TAB_STA,
        TAB_CON,
        TAB_AUD,
        TAB_DIS,
#elif GTASA
        TAB_SAV,
        TAB_BRI,
        TAB_MAP,
        TAB_STA,
        TAB_CON,
        TAB_AUD,
        TAB_GAL,
        TAB_DIS,
#endif
        NUM_TABS
    };

    inline enum {
#ifdef GTA3
        FE_SOUND_ESC = 149,
        FE_SOUND_ENTER = 150,
        FE_SOUND_SWITCH = 151,
        FE_SOUND_BACK = 153,
#elif GTAVC
        FE_SOUND_ESC = 169,
        FE_SOUND_ENTER = 196,
        FE_SOUND_SWITCH = 195,
        FE_SOUND_BACK = 197,
#elif GTASA
        FE_SOUND_ESC = 2,
        FE_SOUND_ENTER = 1,
        FE_SOUND_SWITCH = 3,
        FE_SOUND_BACK = 2,
#endif
    };

    static inline bool initialised = false;
    static inline uint8_t currentInput = INPUT_TAB;
    static inline uint8_t currentTab = TAB_SAV;
    static inline uint32_t menuAlpha = 0;
    static inline float menuOffsetX = 0.0f;
    static inline int8_t menuState = STATE_NONE;
    static inline uint32_t previousTimeInMilliseconds = 0;
    static inline int32_t timeToWaitBeforeStateChange = 0;
    static inline std::array<const char*, 10> helpTexts;
    static inline bool menuActive = false;
    static inline bool saveMenuActive = false;
#ifdef GTAVC
    static inline CMenuPoly selectedItemPoly = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static inline CMenuPoly tempItemPoly = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static inline bool updateItemPoly = true;

    static inline CMenuPoly currBackgroundPoly = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static inline CMenuPoly tempBackgroundPoly = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static inline bool updateBackPoly = true;

#endif
    static inline int32_t previousMenuEntry = -1;
    static inline int32_t previousMenuPage = -1;
    static inline int32_t previousLanguage = 0;

#if defined(GTA3) && defined(GTA3_MENU_MAP)
    static inline bool menuMap = false;
#endif

    struct tMenuTab {
        char str[8];
        float x, y;
        uint8_t targetPage;
    };

    static inline std::vector<tMenuTab> tabs = {
#ifdef GTA3
#ifdef GTA3_MENU_MAP
#define MAP_TAB_OFFSET_X + 66.0f
        { "FEG_MAP", 26.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_MAP },
#else
#define MAP_TAB_OFFSET_X + 0.0f
#endif
        { "FEB_STA", 92.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_STATS },
        { "FEB_SAV", 158.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_NEW_GAME },
        { "FEB_BRI", 224.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_BRIEFS },
        { "FEB_CON", 290.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_CONTROLLER_PC },
        { "FEB_AUD", 356.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEB_DIS", 422.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_DISPLAY_SETTINGS },
        { "FEB_LAN", 488.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_LANGUAGE_SETTINGS },
#elif GTAVC
        { "FEH_MAP", 180.0f, 408.0f, MENUPAGE_MAP },
        { "FEH_BRI", 256.0f, 408.0f, MENUPAGE_BRIEFS },
        { "FEH_LOA", 334.0f, 408.0f, MENUPAGE_NEW_GAME },
        { "FEH_STA", 420.0f, 408.0f, MENUPAGE_STATS },
        { "FEH_CON", 208.0f, 432.0f, MENUPAGE_CONTROLLER_PC },
        { "FEH_AUD", 312.0f, 432.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEH_DIS", 408.0f, 432.0f, MENUPAGE_DISPLAY_SETTINGS },
#elif GTASA
        { "FEH_LOA", 221.0f, 410.0f, MENUPAGE_NEW_GAME },
        { "FEH_BRI", 287.0f, 410.0f, MENUPAGE_BRIEFS },
        { "FEH_MAP", 350.0f, 410.0f, MENUPAGE_MAP },
        { "FEH_STA", 414.0f, 410.0f, MENUPAGE_STATS },
        { "FEH_CON", 198.0f, 432.0f, MENUPAGE_CONTROLLER_PC },
        { "FEH_AUD", 292.0f, 432.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEH_GAL", 372.0f, 432.0f, MENUPAGE_JOYPAD_SETTINGS },
        { "FEH_DIS", 458.0f, 432.0f, MENUPAGE_DISPLAY_SETTINGS },
#endif
    };

    static inline bool GetLeft() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewState.DPadLeft && !pad->OldState.DPadLeft) || (pad->NewKeyState.left && !pad->OldKeyState.left);
    }

    static inline bool GetRight() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewState.DPadRight && !pad->OldState.DPadRight) || (pad->NewKeyState.right && !pad->OldKeyState.right);
    }

    static inline bool GetUp() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewState.DPadUp && !pad->OldState.DPadUp) || (pad->NewKeyState.up && !pad->OldKeyState.up);
    }

    static inline bool GetDown() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewState.DPadDown && !pad->OldState.DPadDown) || (pad->NewKeyState.down && !pad->OldKeyState.down);
    }

    static inline bool GetEnter() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewState.ButtonCross && !pad->OldState.ButtonCross) || (pad->NewKeyState.extenter && !pad->OldKeyState.extenter);
    }

    static inline bool GetEscGamePadOnly() {
        CPad* pad = CPad::GetPad(0);
        return pad->NewState.Start && !pad->OldState.Start;
    }

    static inline bool GetEsc() {
        CPad* pad = CPad::GetPad(0);
        bool back = (pad->NewState.ButtonTriangle && !pad->OldState.ButtonTriangle);

#ifdef GTASA
        if (pad->Mode == 1) {
#else
        if (pad->Mode == 4) {
#endif
            back = (pad->NewState.ButtonCircle && !pad->OldState.ButtonCircle);
        }

        return back || (pad->NewKeyState.esc && !pad->OldKeyState.esc);
    }

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

        CPad* pad = CPad::GetPad(0);
        if (GetLeft() ||
            GetRight() ||
            GetUp() ||
            GetDown() ||
            GetEsc() ||
            GetEnter() ||
            pad->NewKeyState.esc ||
            pad->NewKeyState.insert ||
            pad->NewKeyState.del ||
            pad->NewKeyState.home ||
            pad->NewKeyState.end ||
            pad->NewKeyState.pgup ||
            pad->NewKeyState.pgdn ||
            pad->NewKeyState.up ||
            pad->NewKeyState.down ||
            pad->NewKeyState.left ||
            pad->NewKeyState.right ||
            pad->NewKeyState.scroll ||
            pad->NewKeyState.pause ||
            pad->NewKeyState.numlock ||
            pad->NewKeyState.div ||
            pad->NewKeyState.mul ||
            pad->NewKeyState.sub ||
            pad->NewKeyState.add ||
            pad->NewKeyState.enter ||
            pad->NewKeyState.decimal ||
            pad->NewKeyState.num1 ||
            pad->NewKeyState.num2 ||
            pad->NewKeyState.num3 ||
            pad->NewKeyState.num4 ||
            pad->NewKeyState.num5 ||
            pad->NewKeyState.num6 ||
            pad->NewKeyState.num7 ||
            pad->NewKeyState.num8 ||
            pad->NewKeyState.num9 ||
            pad->NewKeyState.num0 ||
            pad->NewKeyState.back ||
            pad->NewKeyState.tab ||
            pad->NewKeyState.capslock ||
            pad->NewKeyState.extenter ||
            pad->NewKeyState.lshift ||
            pad->NewKeyState.rshift ||
            pad->NewKeyState.shift ||
            pad->NewKeyState.lctrl ||
            pad->NewKeyState.rctrl ||
            pad->NewKeyState.lmenu ||
            pad->NewKeyState.rmenu ||
            pad->NewKeyState.lwin ||
            pad->NewKeyState.rwin ||
            pad->NewKeyState.apps) {
            _this->m_bShowMouse = false;
        }
    }

    static inline uint8_t GetTargetPage() {
        uint8_t targetPage = tabs.at(currentTab).targetPage;

        if (saveMenuActive && targetPage == MENUPAGE_NEW_GAME) {
#ifdef GTA3
            targetPage = MENUPAGE_SAVE;
#elif GTAVC
            targetPage = MENUPAGE_CHOOSE_SAVE_SLOT;
#endif
        }

        return targetPage;
    }

    static inline uint8_t CheckHover(CMenuManager* _this, float x1, float x2, float y1, float y2) {
        CPad* pad = CPad::GetPad(0);

        uint8_t result = 0;

        if (!_this->m_bShowMouse)
            return 0;

        if (_this->m_nMousePosX > x1 && _this->m_nMousePosX < x2 &&
            _this->m_nMousePosY > y1 && _this->m_nMousePosY < y2) {
            result = 1;

            if (pad->NewMouseControllerState.lmb && !pad->OldMouseControllerState.lmb)
                result = 2;

            if (!pad->NewMouseControllerState.lmb && pad->OldMouseControllerState.lmb)
                result = 3;
        }

        return result;
    }

    static inline uint8_t CheckHover(CMenuManager* _this, CRect rect) {
        return CheckHover(_this, rect.left, rect.right, rect.top, rect.bottom);
    }

    static inline void ClearInput() {
        CPad* pad = CPad::GetPad(0);
        pad->Clear(1
#ifdef GTASA
        , 1
#endif
        );
    }

    static inline void SwitchMenuPage(CMenuManager* _this, int32_t page, bool refresh) {
        _this->m_nPreviousMenuPage = _this->m_nCurrentMenuPage;
        _this->m_nCurrentMenuPage = page;
        _this->m_nCurrentMenuEntry = 0;

#ifdef GTAVC
        _this->m_nOptionHighlightTransitionBlend = 0;
        UpdateBackPoly(_this);
#endif

#if defined(GTA3) || defined(GTAVC)
        if (refresh) {
            _this->m_nMenuFadeAlpha = 0;
        }
#endif
    }

#ifdef GTAVC
    static inline void UpdateBackPoly(CMenuManager* _this) {
        if (!updateBackPoly)
            return;

        const float me = 16.0f;

        //CVector2D points[] = { 
        //    // Left Top         // Right Top
        //    { 68.0f, 74.0f }, { 566.0f, 22.0f },
        //    // Left Bottom      // Right Bottom
        //    { 50.0f, 380.0f }, { 596.0f, 366.0f }          
        //};        

        float x = DEFAULT_SCREEN_WIDTH / 2;
        float y = DEFAULT_SCREEN_HEIGHT / 2;
        float halfw = (DEFAULT_SCREEN_WIDTH - 64.0f) / 2;
        float halfh = (DEFAULT_SCREEN_HEIGHT - 64.0f) / 2;

        float topOffset = (24.0f);
        float bottomOffset = (58.0f);
        float leftRightOffset = (16.0f);

        tempBackgroundPoly.x1 = (x - halfw + leftRightOffset - ScaleX(plugin::Random(-me / 2, me / 2)));  tempBackgroundPoly.x2 = (x + halfw - leftRightOffset + ScaleX(plugin::Random(-me / 2, me / 2)));
        tempBackgroundPoly.y1 = (y - halfh + topOffset - ScaleY(plugin::Random(-me / 2, me / 2)));  tempBackgroundPoly.y2 = (y - halfh + topOffset - ScaleY(plugin::Random(-me / 2, me / 2)));

        tempBackgroundPoly.x3 = (x - halfw + leftRightOffset - ScaleX(plugin::Random(-me / 2, me / 2)));  tempBackgroundPoly.x4 = (596.0f);
        tempBackgroundPoly.y3 = (y + halfh - bottomOffset + ScaleY(plugin::Random(-me / 2, me / 2)));  tempBackgroundPoly.y4 = (366.0f);

        updateBackPoly = false;
    }

    static inline void UpdateItemPoly(CMenuManager* _this, float x, float y, float halfw, float halfh) {
        if (!updateItemPoly)
            return;

        const float me = 8.0f;

        tempItemPoly.x1 = (x - halfw - ScaleX(plugin::Random(-me / 2, me / 2)));  tempItemPoly.x2 = (x + halfw + ScaleX(plugin::Random(-me / 2, me / 2)));
        tempItemPoly.y1 = (y - halfh - ScaleY(plugin::Random(-me / 2, me / 2)));  tempItemPoly.y2 = (y - halfh - ScaleY(plugin::Random(-me / 2, me / 2)));

        tempItemPoly.x3 = (x - halfw - ScaleX(plugin::Random(-me / 2, me / 2)));  tempItemPoly.x4 = (x + halfw + ScaleX(plugin::Random(-me / 2, me / 2)));
        tempItemPoly.y3 = (y + halfh + ScaleY(plugin::Random(-me / 2, me / 2)));  tempItemPoly.y4 = (y + halfh + ScaleY(plugin::Random(-me / 2, me / 2)));

        updateItemPoly = false;
    }
#endif

    static inline void SwitchTab(CMenuManager* _this, uint8_t tab) {
        if (!IsTabAvailable(_this, tab))
            return;

        EscTab(_this, false);

        currentInput = INPUT_TAB;
        currentTab = tab;
#if defined(GTA3) || defined(GTAVC)
        DMAudio.PlayFrontEndSound(FE_SOUND_SWITCH, 0);
#elif GTASA
        AudioEngine.ReportFrontendAudioEvent(FE_SOUND_SWITCH, 0.0f, 1.0f);
#endif
#ifdef GTAVC
        updateBackPoly = true;
        updateItemPoly = true;
#endif
    }

    static inline uint32_t GetAlpha(uint32_t a = 255) {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

#ifdef GTA3
        return min(menuAlpha, a);
#else
        return a;
#endif
    }

    static inline uint8_t GetCurrentInput() {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

        return currentInput;
    }

    static inline void EnterTab(CMenuManager* _this, int32_t i = -1, bool playSound = true) {
        if (saveMenuActive)
            return;

        if (i != -1)
            currentTab = i;
        else
            i = currentTab;

#ifdef GTA3
        if (i == TAB_STA || i == TAB_BRI)
            return;
#elif GTAVC
        if (i == TAB_BRI)
            return;
#endif

        SwitchMenuPage(_this, GetTargetPage(), false);
        _this->m_nCurrentMenuEntry = 0;
        _this->m_nHoverOption = 0;

        currentInput = INPUT_STANDARD;
        ClearInput();

        if (i == TAB_AUD) {
#if defined(GTA3) || defined(GTAVC)
            DMAudio.PlayFrontEndTrack(_this->m_nPrefsRadioStation, 1);
#elif GTASA
            AudioEngine.StartRadio(_this->m_nPrefsRadioStation, 0);
#endif
        }

        if (playSound) {
#if defined(GTA3) || defined(GTAVC)
            DMAudio.PlayFrontEndSound(FE_SOUND_ENTER, 0);
#elif GTASA
            AudioEngine.ReportFrontendAudioEvent(FE_SOUND_ENTER, 0.0f, 1.0f);
#endif
        }
#ifdef GTAVC
        updateItemPoly = true;
#endif
        previousMenuEntry = -1;
    }

    static inline void ChangeState(int8_t state) {
        menuState = state;
        timeToWaitBeforeStateChange = CTimer::m_snTimeInMillisecondsPauseMode
#ifdef GTA3
            + 500
#endif
            ;
    }

    static inline void EscTab(CMenuManager* _this, bool playSound = true) {
#ifdef GTASA
        if (AudioEngine.IsRadioRetuneInProgress())
            return;
#endif

        if (saveMenuActive) {
            ChangeState(STATE_CLOSING);
            return;
        }

        currentInput = INPUT_TAB;
        if (playSound)
            ClearInput();

        if (_this->m_nCurrentMenuPage == MENUPAGE_SOUND_SETTINGS) {
#if defined(GTA3) || defined(GTAVC)
            DMAudio.StopFrontEndTrack();
#elif GTASA
            AudioEngine.RetuneRadio(_this->m_nPrefsRadioStation);
            AudioEngine.StopRadio(0, 0);
#endif
        }

        if (playSound) {
#if defined(GTA3) || defined(GTAVC)
            DMAudio.PlayFrontEndSound(FE_SOUND_BACK, 0);
#elif GTASA
            AudioEngine.ReportFrontendAudioEvent(FE_SOUND_BACK, 0.0f, 1.0f);
#endif
        }
#ifdef GTAVC
        updateItemPoly = true;
#endif
        previousMenuEntry = -1;
    }

    static inline void EscMenuCompletely(CMenuManager* _this) {
        _this->RequestFrontEndShutDown();
        _this->SwitchMenuOnAndOff();
        ClearInput();
        Clear(_this);
#if defined(GTA3) || defined(GTAVC)
        DMAudio.PlayFrontEndSound(FE_SOUND_ESC, 0);
#endif
    }

    static inline void UpdateTab(CMenuManager* _this) {
#ifdef GTASA
        _this->m_bStandardInput = false;
#endif

        int firstTab = 0;
        int lastTab = tabs.size() - 1;
#ifdef GTAVC
        if (!_this->m_bGameNotLoaded) {
            if (currentTab >= TAB_MAP && currentTab <= TAB_STA) {
                firstTab = TAB_MAP;
                lastTab = TAB_STA;
            }
            else if (currentTab >= TAB_CON && currentTab <= TAB_DIS) {
                firstTab = TAB_CON;
                lastTab = TAB_DIS;
            }
        }
#elif GTASA
        if (!_this->m_bGameNotLoaded) {
            if (currentTab >= TAB_SAV && currentTab <= TAB_STA) {
                firstTab = TAB_SAV;
                lastTab = TAB_STA;
            }
            else if (currentTab >= TAB_CON && currentTab <= TAB_DIS) {
                firstTab = TAB_CON;
                lastTab = TAB_DIS;
            }
        }
#endif

        int32_t tab = currentTab;
        if (GetLeft()) {
            while (true) {
                if (tab == firstTab)
                    tab = lastTab;
                else
                    tab--;

                if (IsTabAvailable(_this, tab)) {
                    break;
                }
            }

            SwitchTab(_this, tab);
        }
        else if (GetRight()) {
            while (true) {
                if (tab == lastTab)
                    tab = firstTab;
                else
                    tab++;

                if (IsTabAvailable(_this, tab)) {
                    break;
                }
            }

            SwitchTab(_this, tab);
        }

#if defined(GTAVC) || defined(GTASA)
        if (!_this->m_bGameNotLoaded) {
            if (GetDown() || GetUp()) {
                switch (currentTab) {
                case 0:
                    tab = 4;
                    break;
                case 1:
                    tab = 5;
                    break;
                case 2:
                    tab = 6;
                    break;
                case 3:
                    tab = NUM_TABS - 1;
                    break;
                case 4:
                    tab = 0;
                    break;
                case 5:
                    tab = 1;
                    break;
                case 6:
                    tab = 2;
                    break;
#ifdef GTASA
                case 7:
                    tab = 3;
                    break;
#endif
                }

                SwitchTab(_this, tab);
            }
        }
#endif

        if (_this->m_nCurrentMenuPage != GetTargetPage())
            SwitchMenuPage(_this, GetTargetPage(), true);
        else {
            if (GetEnter()) {
                EnterTab(_this);
            }
            else if ((GetEsc() || GetEscGamePadOnly()) && !_this->m_bGameNotLoaded) {
                ChangeState(STATE_CLOSING);
            }
        }

        uint8_t hover = GetCheckHoverForStandardInput(_this);
        if (_this->m_bShowMouse) {
            if (hover == 3) {
                EnterTab(_this, currentTab, true);
            }
        }
    }

    static inline uint8_t GetCheckHoverForStandardInput(CMenuManager* _this) {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)
#if defined(GTA3) || defined(GTAVC)
        float x1 = ScaleXKeepCentered(32.0f);
        float x2 = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 32.0f);
        float y1 = ScaleY(32.0f);
        float y2 = ScaleY(DEFAULT_SCREEN_HEIGHT - 32.0f - 92.0f);
#elif GTASA
        float x1 = 0.0f;
        float x2 = SCREEN_WIDTH;
        float y1 = 0.0f;
        float y2 = ScaleY(DEFAULT_SCREEN_HEIGHT - 92.0f);
       
        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP && currentInput != INPUT_TAB)
            y2 = ScaleY(DEFAULT_SCREEN_HEIGHT);
#endif

        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS)
            return true;

        return _this->m_bShowMouse ? (CheckHover(_this, x1, x2, y1, y2)) : true;
    }

    static inline void UpdateStandard(CMenuManager* _this) {
#ifdef GTASA
        _this->m_bStandardInput = true;
#endif

        if (previousMenuPage != _this->m_nCurrentMenuPage) {
            //_this->m_nCurrentMenuEntry = 0;
            previousMenuPage = _this->m_nCurrentMenuPage;
        }

        if (_this->m_nCurrentMenuPage != GetTargetPage()) {

        }
        else {
            if (GetEsc()) {
                EscTab(_this);
            }
            else if (GetEnter() || (GetCheckHoverForStandardInput(_this) == 3 && !strcmp(aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_EntryName, "FEDS_TB"))) {
                if (_this->m_nCurrentMenuPage == MENUPAGE_STATS)
                    EscTab(_this);
            }
        }
    }

    static inline int32_t GetTimeToWaitBeforeStateChange() {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

        return timeToWaitBeforeStateChange;
    }

    static inline float GetMenuOffsetX() {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

#ifdef GTA3
        return menuOffsetX;
#else
        return 0.0f;
#endif
    }

    static inline void Process(CMenuManager* _this) {
        ClearHelpText();

#ifdef GTA3
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
#elif GTAVC
        if (currentInput == INPUT_STANDARD) {
            if (previousMenuEntry != _this->m_nCurrentMenuEntry) {
                updateItemPoly = true;
                previousMenuEntry = _this->m_nCurrentMenuEntry;
            }

            const CMenuScreen& s = aScreens[_this->m_nCurrentMenuPage];
            const CMenuScreen::CMenuEntry& e = s.m_aEntries[_this->m_nCurrentMenuEntry];

            const wchar_t* str = TheText.Get(e.m_EntryName);
            float strWidth = CFont::GetStringWidth(str, true) / 2;
            if (_this->m_nPrefsLanguage != 0)
                strWidth += ScaleX(16.0f);

            float x = ScaleXKeepCentered(e.m_nX);
            float y = ScaleY((e.m_nY * DEFAULT_SCREEN_HEIGHT / 448.0f) + 12.0f);

            if (e.m_nAlign == 1) {
                strWidth = ScaleX((DEFAULT_SCREEN_WIDTH / 2) - 82.0f);
                x = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH / 2);
            }
            else if (e.m_nAlign == 2) {
                x -= strWidth;
            }

            UpdateItemPoly(_this, x, y, ScaleX(8.0f) + strWidth, ScaleY(14.0f));
        }

        SetHelpText(0, "FEI_BTX");
        SetHelpText(1, "FEI_BTT");
        SetHelpText(2, "FEI_STA");
        SetHelpText(3, "FEI_BTD");

        SetHelpText(5, "FEI_SEL");
        SetHelpText(6, "FEI_BAC");
        SetHelpText(7, "FEI_RES");
        SetHelpText(8, "FEI_NAV");

        if (currentInput == INPUT_STANDARD) {
            switch (currentTab) {
            case TAB_STA:
                SetHelpText(3, "FEI_BTU");
                SetHelpText(5, "FEI_BAC");
                SetHelpText(8, "FEI_SCR");
                break;
            case TAB_MAP:
                SetHelpText(4, "FEI_R1B");
                SetHelpText(8, "FEI_SCR");
                SetHelpText(9, "FEI_ZOO");
                break;
            }
        }

        const float f = (CTimer::m_snTimeInMillisecondsPauseMode - previousTimeInMilliseconds) * 0.02f;
        const float smallNum = 0.001f;
        selectedItemPoly.x1 = std::lerp(selectedItemPoly.x1, tempItemPoly.x1, f);
        selectedItemPoly.y1 = std::lerp(selectedItemPoly.y1, tempItemPoly.y1, f);
        selectedItemPoly.x2 = std::lerp(selectedItemPoly.x2, tempItemPoly.x2, f);
        selectedItemPoly.y2 = std::lerp(selectedItemPoly.y2, tempItemPoly.y2, f);
        selectedItemPoly.x3 = std::lerp(selectedItemPoly.x3, tempItemPoly.x3, f);
        selectedItemPoly.y3 = std::lerp(selectedItemPoly.y3, tempItemPoly.y3, f);
        selectedItemPoly.x4 = std::lerp(selectedItemPoly.x4, tempItemPoly.x4, f);
        selectedItemPoly.y4 = std::lerp(selectedItemPoly.y4, tempItemPoly.y4, f);

        currBackgroundPoly.x1 = std::lerp(currBackgroundPoly.x1, tempBackgroundPoly.x1, f);
        currBackgroundPoly.y1 = std::lerp(currBackgroundPoly.y1, tempBackgroundPoly.y1, f);
        currBackgroundPoly.x2 = std::lerp(currBackgroundPoly.x2, tempBackgroundPoly.x2, f);
        currBackgroundPoly.y2 = std::lerp(currBackgroundPoly.y2, tempBackgroundPoly.y2, f);
        currBackgroundPoly.x3 = std::lerp(currBackgroundPoly.x3, tempBackgroundPoly.x3, f);
        currBackgroundPoly.y3 = std::lerp(currBackgroundPoly.y3, tempBackgroundPoly.y3, f);
        currBackgroundPoly.x4 = std::lerp(currBackgroundPoly.x4, tempBackgroundPoly.x4, f);
        currBackgroundPoly.y4 = std::lerp(currBackgroundPoly.y4, tempBackgroundPoly.y4, f);
#elif GTASA
        // Left
        SetHelpText(0, "FE_HLPH");
        SetHelpText(1, "FE_HLPE");

        // Right
        SetHelpText(4, "FE_HLPG");

        if (currentInput == INPUT_STANDARD) {
            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_MAP:
                if (_this->n_nMenuSystemPanelId == -99) {
                    SetHelpText(2, "FE_HLPB");
                    SetHelpText(3, "FE_HLPC");

                    SetHelpText(4, "FE_HLPO");
                    SetHelpText(5, "FE_HLPT");
                }
                else {
                    SetHelpText(4, "FE_HLPI");
                    SetHelpText(5, "FE_HLPT");
                }
                break;
            }
        }
#endif

        if (timeToWaitBeforeStateChange != -1 && timeToWaitBeforeStateChange < CTimer::m_snTimeInMillisecondsPauseMode) {
            switch (menuState) {
            case STATE_NONE:
                break;
            case STATE_OPENING: {
#ifdef GTA3
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
#else
                ChangeState(STATE_IDLE);
#endif
            } break;
            case STATE_CLOSING: {
#ifdef GTA3
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
#else
                ChangeState(STATE_NONE);
                EscMenuCompletely(_this);
#endif
            } break;
            case STATE_IDLE: {
                switch (currentInput) {
                case INPUT_TAB:
                    UpdateMouse(_this);
                    UpdateTab(_this);
                    break;
                case INPUT_STANDARD:
                    if (!GetCheckHoverForStandardInput(_this))
                        UpdateMouse(_this);
                    UpdateStandard(_this);
                    break;
                }
            } break;
            }
            timeToWaitBeforeStateChange = CTimer::m_snTimeInMillisecondsPauseMode;
        }

        if (_this->m_nPrefsLanguage != previousLanguage) {
#ifdef GTAVC
            updateItemPoly = true;
            updateBackPoly = true;
#endif
            previousLanguage = _this->m_nPrefsLanguage;
        }

        previousTimeInMilliseconds = CTimer::m_snTimeInMillisecondsPauseMode;
    }

    static inline CRect Translate(CRect& rect) {
        rect.left += ScaleX(GetMenuOffsetX());
        rect.right += ScaleX(GetMenuOffsetX());

        float off = 0.0005f;
        rect.left += off;
        rect.top += off;
        rect.right -= off;
        rect.bottom -= off;

        return rect;
    }

#ifdef GTAVC
    static inline void Translate(CMenuPoly& pos) {
        pos.x1 += ScaleX(GetMenuOffsetX());
        pos.x2 += ScaleX(GetMenuOffsetX());
        pos.x3 += ScaleX(GetMenuOffsetX());
        pos.x4 += ScaleX(GetMenuOffsetX()); 
    }

    static inline void Scale(CMenuPoly& pos, float value) {
        pos.x1 -= ScaleX(value);
        pos.x2 += ScaleX(value);
        pos.x3 -= ScaleX(value);
        pos.x4 += ScaleX(value);

        pos.y1 -= ScaleY(value);
        pos.y2 -= ScaleY(value);
        pos.y3 += ScaleY(value);
        pos.y4 += ScaleY(value);
    }
#endif

    static inline void DrawRadioIcon(CMenuManager* _this, int8_t id, float x, float y, float scale) {
#ifdef GTA3
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
#elif GTAVC
        _this->m_aMenuSprites[MENUSPRITE_WILDSTYLE + id].Draw(x, y, scale, scale, id == _this->m_nPrefsRadioStation ? CRGBA(255, 255, 255, GetAlpha()) : CRGBA(255, 255, 255, GetAlpha(55)));
#endif

        if (currentInput == INPUT_STANDARD && CheckHover(_this, x, x + scale, y, y + scale) == 2) {
            _this->m_nPrefsRadioStation = id;

#if defined(GTA3) || defined(GTAVC)
            DMAudio.PlayFrontEndTrack(_this->m_nPrefsRadioStation, 1);
#endif
        }
    }

    static inline void DrawSlider(CMenuManager*_this, float x, float y, float progress) {
        float _x = (x);
        float _y = (y);
        float _w = ScaleX(3.5f);
        float _h = ScaleY(1.0f);
        float off = ScaleY(2.0f);

        for (uint32_t i = 0; i < 16; i++) {
            float current = i / 16.0f + 1 / 32.0f;
            CSprite2d::DrawRect(CRect(_x + off, _y - _h + off, _x + _w + off, _y + off), CRGBA(0, 0, 0, GetAlpha()));

            CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), CRGBA(HUD_COLOUR_BLUEDARK, GetAlpha()));

            if (current < progress)
                CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), CRGBA(HUD_COLOUR_BLUELIGHT, GetAlpha()));

            if (CheckHover(_this, CRect(_x, _y - _h, _x + _w, _y)) == 2)
                _this->CheckSliderMovement(current < progress ? -1 : 1);

            _x += _w + _w;
            _h += ScaleY(1.0f);
        }
    }
    
    static inline void SetHelpText(uint8_t id, const char* str) {
        helpTexts[id] = str;
    }


    static inline void ClearHelpText() {
        for (auto& it : helpTexts)
            it = nullptr;
    }

#ifdef GTAVC
    static inline void DrawTrapezoidForHelpText() {
        CMenuPoly pos = {};
        CMenuPoly pos2 = {};

        pos.x1 = ScaleXKeepCentered(434.0f + GetMenuOffsetX());    pos.x2 = ScaleXKeepCentered(614.0f + GetMenuOffsetX());
        pos.y1 = ScaleY(332.0f);                pos.y2 = ScaleY(322.0f);

        pos.x3 = ScaleXKeepCentered(440.0f + GetMenuOffsetX());    pos.x4 = ScaleXKeepCentered(628.0f + GetMenuOffsetX());
        pos.y3 = ScaleY(394.0f);                pos.y4 = ScaleY(404.0f);

        Translate(pos);

        pos2 = pos;
        Scale(pos2, 4.0f);
        Draw2DPolygon(pos2.x1, pos2.y1, pos2.x2, pos2.y2, pos2.x3, pos2.y3, pos2.x4, pos2.y4, CRGBA(0, 0, 0, GetAlpha()));

        Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, CRGBA(HUD_COLOUR_GREENLIGHT, GetAlpha()));
    }
#endif

    static inline void DrawHeader(CMenuManager* _this) {
#ifdef GTA3
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOn();
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(0);
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetFontStyle(2);
        CFont::SetScale(ScaleX(0.8f), ScaleY(1.2f));
        CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 44.0f + GetMenuOffsetX()), ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f), UpperCase(TheText.Get(tabs.at(currentTab).str)));
#elif GTASA
        if (aScreens[_this->m_nCurrentMenuPage].m_ScreenName[0] != '\0') {
            CFont::SetProportional(true);
            CFont::SetBackground(false, false);
            CFont::SetOrientation(ALIGN_LEFT);
            CFont::SetWrapx(SCREEN_WIDTH);
            CFont::SetDropShadowPosition(2);
            CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
            CFont::SetColor(CRGBA(HUD_COLOUR_BLUEWHITE, GetAlpha()));
            CFont::SetFontStyle(FONT_GOTHIC);
            CFont::SetScale(ScaleX(1.7f), ScaleY(3.3f));
            CFont::PrintString(ScaleX(40.0f), ScaleY(30.0f), TheText.Get(aScreens[_this->m_nCurrentMenuPage].m_ScreenName));
        }
#endif
    }

    static inline void DrawHelpText(CMenuManager* _this) {
#ifdef GTAVC
        DrawTrapezoidForHelpText();
#endif
#if defined (GTA3) || defined(GTAVC)
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
#elif GTASA
        CFont::SetProportional(true);
        CFont::SetBackground(false, false);
        CFont::SetOrientation(ALIGN_LEFT);
#endif
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(0);

#ifdef GTA3
        CFont::SetScale(ScaleX(0.38f), ScaleY(0.64f));
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetFontStyle(0);

        float startx = ScaleXKeepCentered(50.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 94.0f);
#elif GTAVC
        CFont::SetScale(ScaleX(0.40f), ScaleY(0.78f));
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha(255)));
        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha(255)));
        CFont::SetFontStyle(1);

        float startx = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 96.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 148.0f);

        CFont::SetRightJustifyOn();
#elif GTASA
        CFont::SetDropShadowPosition(1);
        CFont::SetScale(ScaleX(0.35f), ScaleY(0.95f));
        CFont::SetScaleForCurrentlanguage(ScaleX(0.35f), ScaleY(0.95f));

        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha(255)));
        CFont::SetColor(CRGBA(HUD_COLOUR_GREY, GetAlpha(255)));
        CFont::SetFontStyle(FONT_MENU);

        float startx = ScaleX(40.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f);
        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP && currentInput == INPUT_STANDARD)
            starty += ScaleY(24.0f);
#endif
        float x = startx;
        float y = starty;
        float spacing = 16.0f;


#if GTAVC
        if (helpTexts[4])
            spacing = 12.0f;
#endif

        uint32_t i = 0;
        for (auto& it : helpTexts) {
            if (it)
                CFont::PrintString(x, y, TheText.Get(it));

            y += ScaleY(spacing);

#ifdef GTA3
            if (i == 2) {
                x += ScaleX(198.0f);
                y = starty;
            }
#elif GTAVC
            if (i == 4) {
                x += ScaleX(8.0f);
                y = starty;
                CFont::SetRightJustifyOff();
                CFont::SetCentreOff();
            }
#elif GTASA
            if (i == 1) {
                CFont::SetOrientation(ALIGN_LEFT);
                x = (SCREEN_WIDTH / 2) - ScaleX(40.0f);
                y = starty;
            }

            if (i == 3) {
                CFont::SetOrientation(ALIGN_RIGHT);
                x = SCREEN_WIDTH - ScaleX(40.0f);
                y = starty;
            }
#endif

            i++;
        }
    }

#ifdef GTAVC
    static inline void DrawTrapezoid() {
        CMenuPoly t = selectedItemPoly;

        Translate(t);
        Draw2DPolygon((t.x1), (t.y1),
            (t.x2), (t.y2),
            (t.x3), (t.y3),
            (t.x4), (t.y4), CRGBA(HUD_COLOUR_GREENLIGHT, GetAlpha()));
    }
#endif

    static inline void DrawBack(CMenuManager* _this) {
        //RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

        CSprite2d::DrawRect(CRect(-5.0f, -5.0f, SCREEN_WIDTH + 5.0f, SCREEN_HEIGHT + 5.0f), CRGBA(0, 0, 0, 255));

#ifdef GTA3
        CSprite2d* splash = plugin::CallAndReturn<CSprite2d*, 0x48D550>("splash1"); // LoadSplash
        splash->Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(50, 50, 50, 255));
#elif GTAVC
        _this->m_aMenuSprites[MENUSPRITE_BACKGROUND].Draw(CRect(ScaleXKeepCentered(0.0f), ScaleY(0.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH), ScaleY(DEFAULT_SCREEN_HEIGHT)), CRGBA(255, 255, 255, GetAlpha()));

        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP)
            _this->PrintMap();

        CMenuPoly pos = {};
        if (_this->m_nCurrentMenuPage == MENUPAGE_STATS) {
            pos.x1 = ScaleXKeepCentered(90.0f); pos.y1 = ScaleY(140.0f);  pos.x2 = ScaleXKeepCentered(542); pos.y2 = ScaleY(152.0f);
            pos.x3 = ScaleXKeepCentered(108.0f); pos.y3 = ScaleY(338.0f);  pos.x4 = ScaleXKeepCentered(534.0f); pos.y4 = ScaleY(322.0f);
            Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, CRGBA(HUD_COLOUR_AZURE2, GetAlpha(130)));
        }
        else if (_this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_SAVE_SLOT ||
            _this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_LOAD_SLOT ||
            _this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_DELETE_SLOT) {
            pos.x1 = ScaleXKeepCentered(76.0f); pos.y1 = ScaleY(88.0f);  pos.x2 = ScaleXKeepCentered(576.0f); pos.y2 = ScaleY(78.0f);
            pos.x3 = ScaleXKeepCentered(64.0f); pos.y3 = ScaleY(340.0f);  pos.x4 = ScaleXKeepCentered(556.0f); pos.y4 = ScaleY(354.0f);
            Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, CRGBA(HUD_COLOUR_AZURE2, GetAlpha(130)));
        }

        pos = {};

        CRGBA backCol = CRGBA(0, 0, 0, 255);
        CMenuPoly points = {};
        points = currBackgroundPoly;

        // Left
        pos.x1 = (0.0f); pos.y1 = (0.0f);  pos.x2 = ScaleXKeepCentered(points.x1); pos.y2 = (0.0f);
        pos.x3 = (0.0f); pos.y3 = SCREEN_HEIGHT;  pos.x4 = ScaleXKeepCentered(points.x3); pos.y4 = SCREEN_HEIGHT;
        Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, backCol);

        // Top
        pos.x1 = (0.0f); pos.y1 = (0.0f);  pos.x2 = SCREEN_WIDTH; pos.y2 = (0.0f);
        pos.x3 = (0.0f); pos.y3 = ScaleY(points.y1);  pos.x4 = SCREEN_WIDTH; pos.y4 = ScaleY(points.y2);
        Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, backCol);

        // Right
        pos.x1 = ScaleXKeepCentered(points.x2); pos.y1 = (0.0f);  pos.x2 = SCREEN_WIDTH; pos.y2 = (0.0f);
        pos.x3 = ScaleXKeepCentered(points.x4); pos.y3 = SCREEN_HEIGHT;  pos.x4 = SCREEN_WIDTH; pos.y4 = SCREEN_HEIGHT;
        Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, backCol);
        
        // Bottom
        pos.x1 = (0.0f); pos.y1 = ScaleY(points.y3);  pos.x2 = SCREEN_WIDTH; pos.y2 = ScaleY(points.y4);
        pos.x3 = (0.0f); pos.y3 = SCREEN_HEIGHT;  pos.x4 = SCREEN_WIDTH; pos.y4 = SCREEN_HEIGHT;
        Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, backCol);

        _this->m_aMenuSprites[MENUSPRITE_VCLOGO].Draw(ScaleXKeepCentered(48.0f), ScaleY(20.0f), ScaleX(128.0f), ScaleY(132.0f), CRGBA(255, 255, 255, GetAlpha()));
#endif
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

        CRect rect = {};
        CRGBA col = CRGBA(255, 255, 255, GetAlpha());

#ifdef GTA3
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
#endif

#ifdef GTA3
        // Radio icons
        if (_this->m_nCurrentMenuPage == MENUPAGE_SOUND_SETTINGS) {
            float start = ScaleXKeepCentered(144.0f + GetMenuOffsetX());
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
#endif

#ifdef GTAVC
        if (currentInput == INPUT_TAB || aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_EntryName[0] != '\0')
            DrawTrapezoid();
#endif

        // Sliders 
        for (int i = 0; i < NUM_ENTRIES; i++) {
#ifdef GTA3
            switch (aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nAction) {
            case MENUACTION_BRIGHTNESS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), _this->m_nPrefsBrightness / 512.0f);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f + 22.0f), (_this->m_fPrefsLOD - 0.8f) * 1.0f);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), _this->m_nPrefsMusicVolume / 128.0f);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f + 22.0f), _this->m_nPrefsSfxVolume / 128.0f);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), TheCamera.m_fMouseAccelHorzntal * 200.0f);
                break;
            }
#else
            float y = (aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nY * DEFAULT_SCREEN_HEIGHT / 448.0f) + 16.0f;

            switch (aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nAction) {
            case MENUACTION_BRIGHTNESS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsBrightness / 384.0f);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), (_this->m_fPrefsLOD - 0.925f) / 0.875f);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsMusicVolume / 64.0f);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsSfxVolume / 64.0f);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), TheCamera.m_fMouseAccelHorzntal * 200.0f);
                break;

#ifdef GTAVC
            case MENUACTION_MP3VOLUMEBOOST:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsMP3BoostVolume / 64.0f);
                break;
#endif
            }
#endif
        }
    }

    static inline bool IsTabAvailable(CMenuManager* _this, int32_t tab) {
#if defined(GTA3) && defined(GTA3_MENU_MAP)
        if (menuMap) {
            if (tab == TAB_LAN)
                return false;
        }
        else if (tab == TAB_MAP)
            return false;
#endif

        if (_this->m_bGameNotLoaded && (tab == TAB_STA || tab == TAB_BRI
#if defined(GTAVC) || defined(GTASA) || (defined(GTA3) && defined(GTA3_MENU_MAP))
            || (tab == TAB_MAP)
#endif
#ifdef GTASA
            || (tab == TAB_GAL)
#endif
            )) {
            return false;
        }

        return true;
    }

    static inline void DrawFront(CMenuManager* _this) {
        if (timeToWaitBeforeStateChange == -1)
            return;
 
        //RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

        DrawHelpText(_this);

#ifdef GTASA
        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP && currentInput == INPUT_STANDARD)
            return;
#endif

#if defined(GTA3) || defined(GTASA)
        DrawHeader(_this);
#endif

        if (saveMenuActive)
            return;

#if defined(GTA3) || defined(GTAVC)
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
#else
        CFont::SetProportional(true);
        CFont::SetBackground(false, false);
#endif
        CFont::SetWrapx(SCREEN_WIDTH);

#ifdef GTA3
        CFont::SetDropShadowPosition(0);
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetFontStyle(0);
        CFont::SetScale(ScaleX(0.28f), ScaleY(0.54f));
#elif GTAVC
        CFont::SetDropShadowPosition(2);
        CFont::SetRightJustifyOff();
        CFont::SetCentreOn();
        CFont::SetFontStyle(2);
        CFont::SetScale(ScaleX(0.64f), ScaleY(1.0f));

        switch (_this->m_nPrefsLanguage) {
        case 1:
        case 2:
        case 3:
        case 4:
            CFont::SetScale(ScaleX(0.64f) * 0.8f, ScaleY(1.0f));
            break;
        }
#elif GTASA
        CFont::SetEdge(0);
        CFont::SetDropShadowPosition(2);
        CFont::SetOrientation(ALIGN_CENTER);
        CFont::SetFontStyle(FONT_PRICEDOWN);
        CFont::SetScale(ScaleX(0.5f), ScaleY(1.0f));
        CFont::SetScaleForCurrentlanguage(ScaleX(0.5f), ScaleY(1.0f));
#endif

        int32_t i = 0;
        int32_t j = 0;
        CRGBA c = {};
        float it_x = 0.0f;
        float it_y = 0.0f;

        for (auto& it : tabs) {
#if defined(GTA3) || defined(GTAVC)
            const wchar_t* str = TheText.Get(tabs.at(i).str);
#elif GTASA
            const char* str = TheText.Get(tabs.at(i).str);
#endif
            const float strWidth = CFont::GetStringWidth(str, true
#ifdef GTASA
                , false
#endif
            );
#if defined(GTASA) || defined(GTAVC)
            int numItems = 3;
            float spacing = -32.0f;

            if (_this->m_nPrefsLanguage != 0) {
                spacing = -82.0f;
#ifdef GTASA
                switch (_this->m_nPrefsLanguage) {
                case 3:
                    if (i == TAB_STA)
                        spacing += 32.0f;
                    break;
                case 4:
                    if (i == TAB_STA)
                        spacing += 14.0f;
                    break;
                }
#endif
            }

            const float canvas = ((DEFAULT_SCREEN_WIDTH / 2) + spacing);
#endif

#if defined(GTA3) && defined(GTA3_MENU_MAP)
            if (menuMap) {
                if (i == TAB_MAP) {
                    wchar_t* strBuff = LowerCase((wchar_t*)str);
                    strBuff[0] = GetUpperCase(str[0]);
                    str = strBuff;
                }
            }
#endif

            if (!IsTabAvailable(_this, i)) {
                i++;
                if (j > 0 && j > i)
                    j--;
                continue;
            }

            it_x = ScaleXKeepCentered(tabs.at(j).x);
            it_y = ScaleY(tabs.at(j).y);

#if defined(GTASA) || defined(GTAVC)
            if (_this->m_bGameNotLoaded || _this->m_nPrefsLanguage != 0) {
                int32_t k = j;
                if (j > numItems)
                    j -= numItems + 1;
                it_x = SCREEN_WIDTH / 2;
                it_x -= ScaleX(canvas / 2);
                it_x += ScaleX((canvas / numItems) * j);
                j = k;
            }

#ifdef GTAVC
            if (!_this->m_bGameNotLoaded && _this->m_nPrefsLanguage != 0) {
                if (i > 3)
                    it_x += ScaleX(42.0f);
                else
                    it_x += ScaleX(-16.0f);
            }
#endif
#endif

#ifdef GTAVC 
            if (i == currentTab && currentInput == INPUT_TAB)
                UpdateItemPoly(_this, it_x, it_y + ScaleY(10.0f), ScaleX(4.0f) + strWidth / 2, ScaleY(18.0f));
#endif

            it_x += ScaleX(GetMenuOffsetX());

            //RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

            CRect rect;
            rect.left = it_x + ScaleX(-4.0f);
            rect.top = it_y + ScaleY(-3.5f);
            rect.right = rect.left + ScaleX(130.0f);
            rect.bottom = rect.top + ScaleY(36.5f);

#ifdef GTA3
            c = CRGBA(0, 0, 0, GetAlpha());
#elif GTAVC
            c = CRGBA(225, 225, 225, GetAlpha());
#elif GTASA
            c = CRGBA(HUD_COLOUR_ORANGE, GetAlpha());
#endif

#ifdef GTA3
            if (i == currentTab)
                _this->m_aFrontEndSprites[FE2_TABACTIVE].Draw(rect, CRGBA(255, 255, 255, GetAlpha()));
#endif

#ifdef GTA3
            if (currentInput != INPUT_STANDARD || i == currentTab)
#endif 
            {
#ifdef GTA3
                rect.left = it_x + ScaleX(-4.0f);
                rect.top = it_y + ScaleY(-3.0f);
                rect.right = rect.left + ScaleX(64.0f);
                rect.bottom = rect.top + ScaleY(22.0f);
#else
                rect.left = it_x - strWidth / 2;
                rect.top = it_y + ScaleY(-3.0f);
                rect.right = it_x + strWidth / 2;
                rect.bottom = rect.top + ScaleY(22.0f);
#endif
            
                //CSprite2d::DrawRect(rect, CRGBA(255, 255, 255, 255));
                uint8_t hover = CheckHover(_this, rect);

#ifdef GTASA
                if (currentTab == i)
                    c = CRGBA(HUD_COLOUR_WHITE, GetAlpha());
#endif

                if (hover && currentTab != i) {
#ifdef GTA3
                    c = CRGBA(205, 205, 205, GetAlpha());
#elif GTAVC
                    c = CRGBA(25, 130, 70, GetAlpha());
#elif GTASA
                    c = CRGBA(HUD_COLOUR_ORANGELIGHT, GetAlpha());
#endif

                    if (hover == 2) {
                        SwitchTab(_this, i);
                    }
                }

                CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
                CFont::SetColor(c);

                CFont::PrintString(it_x, it_y, str);
            }
            i++;
            j++;
        }
    }

    static inline void CheckForExternalScripts() {
#if defined(GTA3) && defined(GTA3_MENU_MAP)
        const HMODULE h = ModuleList().GetByPrefix(L"menumap");
        if (h) {
            menuMap = true;
        }
#endif
    }

    static inline void Init() {
        if (initialised)
            return;

        for (int i = 0; i < NUM_MENU_PAGES; i++) {

            for (int j = 0; j < NUM_ENTRIES; j++) {
#ifdef GTAVC
                if (aScreens[i].m_aEntries[j].m_nX != 0 && aScreens[i].m_aEntries[j].m_nAlign == 1) {
                    aScreens[i].m_aEntries[j].m_nX += 42;
                }

                if (i == MENUPAGE_STATS) {
                    aScreens[i].m_aEntries[j].m_nAction = MENUACTION_NOTHING;
                    aScreens[i].m_aEntries[j].m_nTargetMenu = MENUPAGE_NONE;
                    continue;
                }

#endif
                if (!strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FEDS_TB") ||
                    (!strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FET_DEF") && i != MENUPAGE_CONTROLLER_PC)) {
                    aScreens[i].m_aEntries[j].m_nAction = MENUACTION_NOTHING;
                    aScreens[i].m_aEntries[j].m_EntryName[0] = '\0';
                    aScreens[i].m_aEntries[j].m_nSaveSlot = 0;
                    aScreens[i].m_aEntries[j].m_nTargetMenu = MENUPAGE_NONE;
                }
            }
        }

#ifdef GTA3
        for (auto& it : tabs) {
            aScreens[it.targetPage].m_nPreviousPage[0] = it.targetPage;
            aScreens[it.targetPage].m_nPreviousPage[1] = it.targetPage;
        }

        aScreens[MENUPAGE_EXIT].m_nPreviousPage[0] = MENUPAGE_NEW_GAME;
        aScreens[MENUPAGE_EXIT].m_nPreviousPage[1] = MENUPAGE_NEW_GAME;

        aScreens[MENUPAGE_EXIT].m_nParentEntry[0] = 3;
        aScreens[MENUPAGE_EXIT].m_nParentEntry[1] = 3;
#else
        for (auto& it : tabs) {
            aScreens[it.targetPage].m_nPreviousPage = it.targetPage;
        }

        aScreens[MENUPAGE_EXIT].m_nPreviousPage = MENUPAGE_NEW_GAME;
        aScreens[MENUPAGE_EXIT].m_nParentEntry = 3;
#endif

#ifdef GTAVC
        aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[4].m_nAction = MENUACTION_RADIO;
        strcpy(aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[4].m_EntryName, "FEA_RSS");

        aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[5].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[5].m_EntryName[0] = '\0';
        aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[6].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_SOUND_SETTINGS].m_aEntries[6].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[8].m_nAlign = 1;
        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[8].m_nX = 0;
        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[8].m_nY = 0;
#endif

        strcpy(aScreens[MENUPAGE_NEW_GAME].m_ScreenName, "FEH_LOA");
        strcpy(aScreens[MENUPAGE_CONTROLLER_PC].m_ScreenName, "FEH_CON");

        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nAction = MENUACTION_CHANGEMENU;
        strcpy(aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_EntryName, "FET_QG");
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nSaveSlot = 0;
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nTargetMenu = MENUPAGE_EXIT;
#if defined(GTASA) || defined(GTAVC)
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nX = 0;
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nY = 0;

#ifdef GTAVC
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nAlign = 0;
#else
        aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_nAlign = 1;
#endif
#endif

#ifdef GTA3
        strcpy(aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_EntryName, "FET_LAN");
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAction = MENUACTION_CHANGEMENU;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nTargetMenu = MENUPAGE_LANGUAGE_SETTINGS;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage[0] = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage[1] = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry[0] = 8;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry[1] = 8;
#elif GTAVC
        strcpy(aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_EntryName, "FEH_LAN");
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nAction = MENUACTION_CHANGEMENU;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nTargetMenu = MENUPAGE_LANGUAGE_SETTINGS;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nX = 320;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nY = 310;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry = 9;
#elif GTASA
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry = 6;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[0].m_nAction = MENUACTION_LANG_ENG;
        strcpy(aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[0].m_EntryName, "FEL_ENG");
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[0].m_nX = 320;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[0].m_nY = 140;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[0].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_nAction = MENUACTION_LANG_FRE;
        strcpy(aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_EntryName, "FEL_FRE");
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_nX = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_nY = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_nAction = MENUACTION_LANG_GER;
        strcpy(aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_EntryName, "FEL_GER");
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_nX = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_nY = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_nAction = MENUACTION_LANG_ITA;
        strcpy(aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_EntryName, "FEL_ITA");
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_nX = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_nY = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_nAction = MENUACTION_LANG_SPA;
        strcpy(aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_EntryName, "FEL_SPA");
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_nX = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_nY = 0;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_nAlign = 0;

        aScreens[MENUPAGE_DISPLAY_ADVANCED].m_nParentEntry = 7;

        memcpy(&aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[7], &aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[6], sizeof(CMenuScreen::CMenuEntry));
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[7].m_nY += 20;

        strcpy(aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[6].m_EntryName, "FEO_LAN");
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[6].m_nAction = MENUACTION_CHANGEMENU;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[6].m_nTargetMenu = MENUPAGE_LANGUAGE_SETTINGS;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[6].m_nY -= 8;
#endif

        initialised = true;
        //Clear(_this);
    }

    static inline void InitAfterRw() {
        CheckForExternalScripts();

#if defined(GTA3) && defined(GTA3_MENU_MAP)
        if (!menuMap) {
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_EntryName[0] = '\0';
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAction = MENUACTION_NOTHING;
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nTargetMenu = MENUPAGE_NONE;
        }
#endif
    }

    static inline void Clear(CMenuManager* _this, bool run = false) {
        if (saveMenuActive)
            currentInput = INPUT_STANDARD;
        else
            currentInput = INPUT_TAB;

#if defined(GTAVC) || defined(GTASA)
        if (!_this->m_bGameNotLoaded && !saveMenuActive)
            currentTab = TAB_MAP;
#endif
        menuOffsetX = DEFAULT_SCREEN_WIDTH;
        menuAlpha = 0;
        previousTimeInMilliseconds = CTimer::m_snTimeInMillisecondsPauseMode;
        previousMenuEntry = -1;
#ifdef GTAVC
        updateItemPoly = true;
#endif

        if (run) {
            ChangeState(STATE_OPENING);
        }
        else
            ChangeState(STATE_NONE);

        if (_this->m_nCurrentMenuPage != GetTargetPage())
            SwitchMenuPage(_this, GetTargetPage(), true);

#ifdef GTASA
        _this->m_bStandardInput = false;
#endif
    }

    static inline void Shutdown(CMenuManager* _this) {

    }

    SkyUI() {
#ifdef GTA3
        // No green bar
        plugin::patch::Nop(0x47C597, 5);

        //auto drawHeader = [](float, float, wchar_t* str) {
        //
        //};
        //plugin::patch::RedirectCall(0x47AF76, LAMBDA(void, __cdecl, drawHeader, float, float, wchar_t*));
        plugin::patch::Nop(0x47AF76, 5);

        auto drawAction = [](float, float, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + GetMenuOffsetX()), ScaleY(64.0f), str);
        };
        plugin::patch::RedirectCall(0x47B132, LAMBDA(void, __cdecl, drawAction, float, float, wchar_t*));

        auto drawBriefs = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(52.0f + GetMenuOffsetX()), y + ScaleY(38.0f), str);
        };
        plugin::patch::RedirectCall(0x484F3A, LAMBDA(void, __cdecl, drawBriefs, float, float, wchar_t*));

        auto drawLeftString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

            float center = 0.0f;

            if (CFont::Details.m_bCentre)
                center = 240.0f;

            CFont::PrintString(ScaleXKeepCentered(64.0f + center + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x47C666, LAMBDA(void, __cdecl, drawLeftString, float, float, wchar_t*));

        auto drawRightString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x47C74C, LAMBDA(void, __cdecl, drawRightString, float, float, wchar_t*));
        plugin::patch::SetChar(0x47DA11 + 6, 255);

        // Stats
        auto drawCrimRa = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + GetMenuOffsetX()), ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x4825AD, LAMBDA(void, __cdecl, drawCrimRa, float, float, wchar_t*));

        auto drawRate = [](float, float, wchar_t* str) {
            char buff[32];
            sprintf(buff, "%d", CStats::FindCriminalRatingNumber());

            float x = CFont::GetStringWidth(buff, 1);
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX() - 16.0f) - x, ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x482620, LAMBDA(void, __cdecl, drawRate, float, float, wchar_t*));

        auto drawRate2 = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), ScaleY(52.0f), str);
        };
        plugin::patch::RedirectCall(0x4826D0, LAMBDA(void, __cdecl, drawRate2, float, float, wchar_t*));

        auto drawStatLeft = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(64.0f + GetMenuOffsetX()), y + ScaleY(30.0f), str);
        };
        plugin::patch::RedirectCall(0x4824E1, LAMBDA(void, __cdecl, drawStatLeft, float, float, wchar_t*));


        auto drawStatRight = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), y + ScaleY(30.0f), str);
        };
        plugin::patch::RedirectCall(0x482527, LAMBDA(void, __cdecl, drawStatRight, float, float, wchar_t*));

        plugin::patch::Nop(0x4827AE, 5); // No stats header

        // No help text
        plugin::patch::Nop(0x47F107, 7);

        // No sliders
        plugin::patch::RedirectJump(0x47CA11, (void*)0x47D9B9);
#elif GTAVC

        // No help text
        plugin::patch::Nop(0x48F9AF, 5);
        
        // No green bar
        plugin::patch::Nop(0x49F4D6, 5);
        plugin::patch::Nop(0x49FB7E, 5);
        
        //plugin::patch::Nop(0x4A269F, 8);
        //plugin::patch::Nop(0x4985BD, 8);
        //plugin::patch::Nop(0x4985C5, 8);
        
        // No background
        //plugin::patch::Nop(0x4A21E0, 5);
        plugin::patch::Nop(0x4A2DB9, 5);
        plugin::patch::Nop(0x4A2EB3, 5);
        plugin::patch::Nop(0x4A2FC2, 5);
        plugin::patch::Nop(0x4A2831, 5);
        plugin::patch::Nop(0x4A292B, 5);
        plugin::patch::Nop(0x4A2A34, 5);
        plugin::patch::Nop(0x4A30D1, 5);
        plugin::patch::Nop(0x4A271E, 17);
        plugin::patch::Nop(0x4A2CA6, 17);
        
        // No map
        plugin::patch::SetUChar(0x4A2CBE, 0xEB);
        plugin::patch::SetUChar(0x4A2736, 0xEB);
        
        // No logo
        plugin::patch::Set<BYTE>(0x4A330B + 1, 0x85);
        plugin::patch::Set<BYTE>(0x4A3302 + 6, -1);
        
        auto drawHeader = [](float, float, wchar_t* str) {
            CFont::SetScale(ScaleX(1.0f), ScaleY(2.2f));
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 30.0f + GetMenuOffsetX()), ScaleY(32.0f), str);
        };
        plugin::patch::RedirectCall(0x49E3D9, (void(__cdecl*)(float, float, wchar_t*))drawHeader);
        
        auto drawHeaderShadow = [](float, float, wchar_t* str) {
            CFont::SetScale(ScaleX(1.0f), ScaleY(2.2f));
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 36.0f + GetMenuOffsetX()), ScaleY(36.0f), str);
        };
        plugin::patch::RedirectCall(0x49E30E, (void(__cdecl*)(float, float, wchar_t*))drawHeaderShadow);
        
        auto drawAction = [](float, float, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::PrintString(ScaleXKeepCentered(92.0f + GetMenuOffsetX()), ScaleY(92.0f), str);
        };
        plugin::patch::RedirectCall(0x49E656, (void(__cdecl*)(float, float, wchar_t*))drawAction);
        
        auto drawLeftString = [](float x, float y, wchar_t* str) {
            float _x = x;
            if (SCREEN_WIDTH != DEFAULT_SCREEN_WIDTH) {
                _x = x / (0.0015625 * SCREEN_WIDTH);
            }
        
            CFont::PrintString(ScaleXKeepCentered(_x), y, str);
        };
        plugin::patch::RedirectCall(0x49FCAD, (void(__cdecl*)(float, float, wchar_t*))drawLeftString);
        
        auto drawRightString = [](float, float y, wchar_t* str) {
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 84.0f + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x49FE4E, (void(__cdecl*)(float, float, wchar_t*))drawRightString);
        
        // No blue background
        plugin::patch::Nop(0x49BB31, 5);
        plugin::patch::Nop(0x49E1B5, 5);
        
        auto drawStatLeftString = [](float, float y, wchar_t* str) {
            CFont::SetScale(ScaleX(0.43f), ScaleY(0.75f));
            CFont::PrintString(ScaleXKeepCentered(110.0f + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x49C0F3, (void(__cdecl*)(float, float, wchar_t*))drawStatLeftString);
        
        auto drawStatRightString = [](float, float y, wchar_t* str) {
            CFont::SetScale(ScaleX(0.43f), ScaleY(0.75f));
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 110.0f + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x49C135, (void(__cdecl*)(float, float, wchar_t*))drawStatRightString);
        
        // No sliders
        plugin::patch::RedirectJump(0x4A00B8, (void*)0x4A00F1);

        // No radio icons
        //plugin::patch::Nop(0x49DFF6, 7);

        const float yoff = 0.85f;
        plugin::patch::SetFloat(0x68D718, 0.63616073 * yoff);
        plugin::patch::SetFloat(0x68D704, 0.78125 * yoff);
        plugin::patch::SetFloat(0x68D724, 0.13392857 * yoff);
        plugin::patch::SetFloat(0x68D720, 0.64285713 * yoff);
        plugin::patch::SetFloat(0x68D79C, 0.73660713 * yoff);
        plugin::patch::SetFloat(0x68D7AC, 0.7098214 * yoff);
        plugin::patch::SetFloat(0x68D7B4, 0.6785714 * yoff);

        //plugin::patch::SetFloat(0x68D6AC, 0.71651787 * yoff);
        static float f = 0.71651787 * yoff;
        plugin::patch::SetPointer(0x49D9A2 + 2, &f);
        plugin::patch::SetPointer(0x49DA22 + 2, &f);
        plugin::patch::SetPointer(0x49DCA0 + 2, &f);
        plugin::patch::SetPointer(0x49DD20 + 2, &f);

        plugin::patch::SetFloat(0x68D78C, 0.68526787 * yoff);
        plugin::patch::SetFloat(0x68D77C, 0.7433036 * yoff);
        plugin::patch::SetFloat(0x68D4E8, 0.044642858 * yoff);
        plugin::patch::SetFloat(0x68D744, 0.62053573 * yoff);

#elif GTASA
        // No half size map key.
        plugin::patch::Nop(0x57516D, 4);
        plugin::patch::Nop(0x576307, 4);

        // No help text
        plugin::patch::Nop(0x57E3AE, 5);

        // No header
        injector::MakeNOP(0x57F737, 5);
        injector::MakeNOP(0x579698, 5);
#endif

        Init();

        plugin::Events::initRwEvent += []() {
            InitAfterRw();
        };

        onProcess.before += [](CMenuManager* _this) {
#ifdef GTASA
            _this->SwitchMenuOnAndOff();
#endif
            if (_this->m_bMenuActive || _this->m_bSaveMenuActive) {
                if (!menuActive) {
                    if (!saveMenuActive && _this->m_bSaveMenuActive) {
                        SwitchTab(_this, TAB_SAV);
                        saveMenuActive = true;
                    }

                    Clear(_this, true);

                    menuActive = true;
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


#if defined(GTA3) || defined(GTAVC)
        onDrawStandardMenu.before += [](CMenuManager* _this) {
            DrawBack(_this);
        };
#endif

        onDrawStandardMenu.after += [](CMenuManager* _this) {
            DrawFront(_this);
        };

#ifdef GTA3
        auto processButtonPresses = [](CMenuManager* _this, uint32_t) {
            if (currentInput == INPUT_STANDARD) {
                if (GetCheckHoverForStandardInput(_this)) {
                    _this->ProcessButtonPresses();
                }
            }

            // Items selection
            plugin::patch::SetUChar(0x47AE0C + 4, currentInput == INPUT_STANDARD ? 1 : 0);
            plugin::patch::SetUChar(0x47B50A + 4, currentInput == INPUT_STANDARD ? 0 : 1);
        };
#else
        auto userInput = [](CMenuManager* _this, uint32_t) {
            if (!menuActive)
                SwitchMenuPage(_this, tabs.at(currentTab).targetPage, true);

            if (currentInput == INPUT_STANDARD) {
                if (GetCheckHoverForStandardInput(_this)) {
                    _this->UserInput();
                }
            }
        };
#endif

#ifdef GTA3
        plugin::patch::RedirectCall(0x4852FC, LAMBDA(void, __fastcall, processButtonPresses, CMenuManager*, uint32_t));
#elif GTAVC
        plugin::patch::RedirectCall(0x49A03C, LAMBDA(void, __fastcall, userInput, CMenuManager*, uint32_t));

        onDrawRedefinePage.before += [](CMenuManager* _this) {
            CSprite2d::DrawRect(CRect(-5.0f, -5.0f, SCREEN_WIDTH + 5.0f, SCREEN_HEIGHT + 5.0f), CRGBA(0, 0, 0, 255));
        };
#elif GTASA
        plugin::patch::RedirectCall(0x57B457, LAMBDA(void, __fastcall, userInput, CMenuManager*, uint32_t));
        plugin::patch::PutRetn(0x57B477);
#endif
 }

    ~SkyUI() {

    }
} skyUI;
