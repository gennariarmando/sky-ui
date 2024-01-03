#ifdef GTA3
#define GTA3_MENU_MAP
//#define LCSFICATION // LC01 only
#define FRAME_LIMITER
#endif

#include "plugin.h"
#include "CMenuManager.h"
#include "CPad.h"
#include "CFont.h"
#include "CText.h"
#include "CStats.h"
#include "CCamera.h"
#include "CGame.h"
#include "CTxdStore.h"

#include "TextLoader.h"
#include "Timer.h"

#ifdef GTA3
#include "CAnimManager.h"
#include "cDMAudio.h"
#include "CPlayerSkin.h"
#include "CScene.h"
#include "CWorld.h"
#include "dxsdk/d3d8.h"
#include "CCivilianPed.h"
#include "CPopulation.h"

#include "eAnimations.h"
#include "CAnimBlendAssociation.h"
#include "RpAnimBlend.h"
#elif GTAVC
#include "cDMAudio.h"
#include "CPlayerSkin.h"
#include "CScene.h"
#include "dxsdk/d3d8.h"
#elif GTASA
#include "CAudioEngine.h"
#include <filesystem>
#include "CFileMgr.h"
#include "CHud.h"
#include "CMessages.h"
#endif

#include "Utility.h"

#include "ModuleList.hpp"

#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

#include "GInputAPI.h"

#include "../injector/assembly.hpp"


using namespace plugin;

class __declspec(dllexport) SkyUI {
public:
#ifdef GTA3
    static inline ThiscallEvent <AddressList<0x48E721, H_CALL, 0x48C8A4, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x47AB12, H_CALL, 0x47AB07, H_CALL, 0x47AAFE, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawStandardMenu;
    static inline CdeclEvent <AddressList<0x582E3C, H_CALL>, PRIORITY_AFTER, ArgPickNone, void(int32_t, int32_t)> onProcessGameState7;
    static inline ThiscallEvent <AddressList<0x485183, H_CALL, 0x5816A5, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onLoadAllMenuTextures;
    static inline ThiscallEvent <AddressList<0x47A1F3, H_CALL, 0x485682, H_CALL, 0x48ABA0, H_CALL, 0x48C71D, H_CALL,
        0x58163D, H_CALL, 0x582F62, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onUnloadMenuTextures;

#elif GTAVC
    static inline ThiscallEvent <AddressList<0x4A3299, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawRedefinePage;
    static inline ThiscallEvent <AddressList<0x4A4433, H_CALL, 0x4A5C88, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x4A325E, H_CALL, 0x4A32AD, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*, int)> onDrawStandardMenu;
#elif GTASA
    static inline ThiscallEvent <AddressList<0x57BA58, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*, uint8_t)> onDrawStandardMenu;
    static inline ThiscallEvent <AddressList<0x53BF44, H_CALL, 0x53E7A5, H_CALL>, PRIORITY_AFTER, ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline ThiscallEvent <AddressList<0x57B66F, H_CALL>, PRIORITY_BEFORE, ArgPick4N<CMenuManager*, 0, int8_t, 1, bool*, 2, bool, 3>, void(CMenuManager*, int8_t arrows, bool* back, bool enter)> onProcessMenuOptions;
#endif

    static inline Timer timer;

#define HUD_COLOUR_RED_LC01 219, 36, 38
#define HUD_COLOUR_RED 205, 5, 5
#define HUD_COLOUR_GREENLIGHT 25, 130, 70
#define HUD_COLOUR_ORANGE 144, 98, 16
#define HUD_COLOUR_ORANGELIGHT 210, 150, 25
#define HUD_COLOUR_WHITE 225, 225, 225
#define HUD_COLOUR_PINK 225, 225, 225
#define HUD_COLOUR_BLUELIGHT 86, 196, 255 
#define HUD_COLOUR_BLUEDARK 20, 94, 136
#define HUD_COLOUR_BLUEWHITE 164, 196, 232
#define HUD_COLOUR_AZURE 49, 101, 148
#define HUD_COLOUR_AZUREDARK 80, 100, 123
#define HUD_COLOUR_GREY 150, 150, 150
#define HUD_COLOUR_GREYDARK 96, 96, 96
#define HUD_COLOUR_BLACK 0, 0, 0
#define HUD_COLOUR_GREYLIGHT 205, 205, 205
#define HUD_COLOUR_LCS_GREY 14, 16, 21
#define HUD_COLOUR_LCS_MENU 111, 165, 208
#define HUD_COLOUR_YELLOW_LIGHT 255, 235, 150

#if defined(GTA3) && defined(LCSFICATION)
#define LCS_MENU_SCALE_X (0.5f)
#define LCS_MENU_SCALE_Y (1.0f)
#define MENU_OFFSET_Y (12.0f)
#else
#define MENU_OFFSET_Y (0.0f)
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

#ifdef GTA3
#ifdef GTA3_MENU_MAP
    inline enum {
        MENUPAGE_MAP = MENUPAGE_NO_MEMORY_CARD,
    };
#endif
#endif

#ifdef GTASA
    inline enum {
        MENUPAGE_GALLERY = MENUPAGE_MAIN_MENU,
        MENUPAGE_GALLERY_DELETE_PHOTO = MENUPAGE_JOYPAD_SETTINGS,
        MENUPAGE_MODLOADER = 44,
    };

    inline enum {
        MENUACTION_DELETEGALLERYPHOTO = 101,

    };

#endif

    inline enum {
#ifdef GTA3
#ifdef LCSFICATION
        TAB_MAP,
        TAB_BRI,
        TAB_SAV,
        TAB_STA,
        TAB_CON,
        TAB_AUD,
        TAB_DIS,
        TAB_PS,
#else
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
#endif
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
    static inline std::array<std::string, 10> helpTexts = {};
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
    static inline int32_t previousLanguage = -1;

#if defined(GTA3) && defined(GTA3_MENU_MAP)
    static inline bool menuMap = false;
#endif

    static inline IGInputPad* gInputPad = nullptr;

#if defined(GTA3) && defined(LCSFICATION)
    static inline CSprite2d backgroundSprite = {};
    static inline CSprite2d skinSelSprite = {};

    static inline bool playerSkinInitialised = false;
#endif

    static inline bool modLoader = false;

    inline enum {
        PCBTN_LEFT,
        PCBTN_RIGHT,
        PCBTN_UP,
        PCBTN_DOWN,
        NUM_PC_BTNS
    };
    static inline std::array<CSprite2d, NUM_PC_BTNS> pcbtnsSprites = {};
    static inline const char* pcbtnsSpritesNames[NUM_PC_BTNS] = {
        "left",
        "right",
        "up",
        "down",
    };

    static inline bool HasPadInHands() {
        return gInputPad ? gInputPad->HasPadInHands() : false;
    }

#ifdef GTASA
    inline enum {
        MAX_GALLERY_PICS = 128,
    };

    struct tGalleryPhoto {
        int32_t id;
        RwTexture* texture;
    };

    static inline bool scanGalleryPhotos = true;
    static inline bool createGalleryPhotos = false;
    static inline int32_t numGalleryPhotos = 0;
    static inline std::array<tGalleryPhoto, MAX_GALLERY_PICS> galleryPhotos = {};
    static inline int32_t currentGalleryPhoto = 0;
    static inline int32_t galleryDeleteTimer = 0;

    static inline CSprite2d skipHighSprite = {};
#endif

    static inline TextLoader textLoader = {};

    struct tMenuTab {
        char str[8];
        float x, y;
        uint8_t targetPage;
    };

    static inline std::vector<tMenuTab> tabs = {
#ifdef GTA3
#ifdef LCSFICATION
        { "FEG_MAP", 221.0f, 408.0f, MENUPAGE_MAP },
        { "FEB_BRI", 287.0f, 408.0f, MENUPAGE_BRIEFS },
        { "FEB_SAV", 350.0f, 408.0f, MENUPAGE_NEW_GAME },
        { "FEB_STA", 414.0f, 408.0f, MENUPAGE_STATS },
        { "FEB_CON", 198.0f, 432.0f, MENUPAGE_CONTROLLER_PC },
        { "FEB_AUD", 292.0f, 432.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEB_DIS", 372.0f, 432.0f, MENUPAGE_DISPLAY_SETTINGS },
        { "FEB_PS",  458.0f, 432.0f, MENUPAGE_SKIN_SELECT },
#else
#ifdef GTA3_MENU_MAP
#define MAP_TAB_OFFSET_X + 66.0f
        { "FEG_MAP", 26.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_MAP },
#else
#define MAP_TAB_OFFSET_X + 0.0f
#endif
        { "FEB_STA", 92.0f  MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_STATS },
        { "FEB_SAV", 158.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_NEW_GAME },
        { "FEB_BRI", 224.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_BRIEFS },
        { "FEB_CON", 290.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_CONTROLLER_PC },
        { "FEB_AUD", 356.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEB_DIS", 422.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_DISPLAY_SETTINGS },
        { "FEB_LAN", 488.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_LANGUAGE_SETTINGS },
#endif
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
        { "FEH_GAL", 372.0f, 432.0f, MENUPAGE_GALLERY },
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
#elif GTASA
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
        pad->Clear(0
#ifdef GTASA
        , 1
#endif
        );
    }

    static inline void SwitchMenuPage(CMenuManager* _this, int32_t page, bool refresh) {
        _this->m_nPreviousMenuPage = _this->m_nCurrentMenuPage;
        _this->m_nCurrentMenuPage = page;
        _this->m_nCurrentMenuEntry = 0;

#if defined(GTA3) && defined(LCSFICATION)
        currPlayerSkinPage = 0;
        currPlayerSkin = 0;
#endif

#ifdef GTAVC
        _this->m_nOptionHighlightTransitionBlend = 0;
        UpdateBackPoly(_this);
#endif

#if defined(GTA3) && defined(LCSFICATION)
        _this->m_nMenuFadeAlpha = 255;
#endif

#if (defined(GTA3) && !defined(LCSFICATION)) || defined(GTAVC)
        if (refresh) {
            _this->m_nMenuFadeAlpha = 0;
        }
#endif

#if defined(GTA3) && defined(LCSFICATION)
        if (page == MENUPAGE_SKIN_SELECT && !playerSkinInitialised) {
            CPlayerSkin::BeginFrontendSkinEdit();
            _this->m_bSkinsFound = false;
            playerRotation = -25.0f;
            playerRotationLerp = -25.0f;
            playerZoom = 1.0f;
            playerZoomLerp = 1.0f;
            playerSkinInitialised = true;
        }

        if (page != MENUPAGE_SKIN_SELECT && playerSkinInitialised) {
            CPlayerSkin::EndFrontendSkinEdit();
            playerSkinInitialised = false;
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

#if defined(GTA3) && !defined(LCSFICATION)
        return min(menuAlpha, a);
#else
        return a;
#endif
    }

    static inline bool GetGTA3LCS() {
#pragma comment(linker, "/EXPORT:" __FUNCTION__"=" __FUNCDNAME__)

#if defined(GTA3) && defined(LCSFICATION)
        return true;
#else
        return false;
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
#elif GTASA
        if (i == TAB_GAL) {
            if (numGalleryPhotos <= 0)
                return;
            else {
                scanGalleryPhotos = true;
                createGalleryPhotos = true;
            }
        }
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
#if defined(GTA3) && !defined(LCSFICATION)
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

#ifdef GTASA
        if (currentTab == TAB_GAL) {
            GalleryShutdown();
        }
#endif

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
#if defined(GTAVC) || (defined(GTA3) && defined(LCSFICATION))
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

#if defined(GTAVC) || defined(GTASA) || (defined(GTA3) && defined(LCSFICATION))
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
#if (defined(GTA3) && !defined(LCSFICATION)) || defined(GTAVC)
        float x1 = ScaleXKeepCentered(32.0f);
        float x2 = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 32.0f);
        float y1 = ScaleY(32.0f);
        float y2 = ScaleY(DEFAULT_SCREEN_HEIGHT - 32.0f - 92.0f);
#elif defined(GTASA) || (defined(GTA3) && defined(LCSFICATION))
        float x1 = 0.0f;
        float x2 = SCREEN_WIDTH;
        float y1 = 0.0f;
        float y2 = ScaleY(DEFAULT_SCREEN_HEIGHT - 92.0f);
       
        if ((_this->m_nCurrentMenuPage == MENUPAGE_MAP || _this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_SAVE_SLOT) && currentInput != INPUT_TAB)
            y2 = ScaleY(DEFAULT_SCREEN_HEIGHT);
#endif

        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS
            )
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

#if defined(GTA3) && !defined(LCSFICATION)
        return menuOffsetX;
#else
        return 0.0f;
#endif
    }

    static inline std::string HStr(std::string str) {
        //return HasPadInHands() ? str : str + "PC";
        return str;
    }

    static inline void Process(CMenuManager* _this) {
        ClearHelpText();

#ifdef GTA3
        switch (currentTab) {
        default:
            SetHelpText(0, HStr("FEDS_SE"));

            if (currentInput == INPUT_TAB && _this->m_bGameNotLoaded) {

            }
            else {
                SetHelpText(1, HStr("FEDS_BA"));

#ifndef LCSFICATION
                if (HasPadInHands() && !_this->m_bGameNotLoaded)
                    SetHelpText(2, HStr("FEDS_ST"));
#endif
            }

            if (currentInput == INPUT_TAB)
                SetHelpText(4, HStr("FEDS_AM"));
            else
                SetHelpText(4, HStr("FEDSAS4"));
            break;
        case TAB_STA:
            SetHelpText(0, HStr("FEDS_ST"));
            SetHelpText(1, HStr("FEDS_AM"));

            SetHelpText(3, HStr("FEDSSC1"));
            SetHelpText(4, HStr("FEDSSC2"));
            break;
        case TAB_BRI:
            SetHelpText(0, HStr("FEDS_ST"));
            SetHelpText(4, HStr("FEDS_AM"));
            break;
#ifdef LCSFICATION
        case TAB_MAP:
            if (currentInput == INPUT_STANDARD) {
                SetHelpText(0, HStr("FEDS_BA"));
                SetHelpText(2, HStr("FEDS_LE"));
            }
            else {
                SetHelpText(0, HStr("FEDS_SE"));
                SetHelpText(1, HStr("FEDS_BA"));
            }
            break;
#endif
        }
#elif GTAVC
        if (currentInput == INPUT_STANDARD) {
            if (previousMenuEntry != _this->m_nCurrentMenuEntry) {
                updateItemPoly = true;
                previousMenuEntry = _this->m_nCurrentMenuEntry;
            }

            const CMenuScreen& s = aScreens[_this->m_nCurrentMenuPage];
            const CMenuScreen::CMenuEntry& e = s.m_aEntries[_this->m_nCurrentMenuEntry];

            const wchar_t* str = textLoader.Get(e.m_EntryName);
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
        SetHelpText(0, HStr("FE_HLPH"));

        if (currentInput == INPUT_TAB && _this->m_bGameNotLoaded) {
        }
        else
            SetHelpText(1, HStr("FE_HLPE"));

        // Right
        SetHelpText(4, "FE_HLPG");

        switch (_this->m_nCurrentMenuPage) {
        case MENUPAGE_MAP:
            if (currentInput == INPUT_STANDARD) {
                if (_this->n_nMenuSystemPanelId == -99) {
                    SetHelpText(0, HStr("FE_HLPA"));
                    SetHelpText(2, HStr("FE_HLPB"));
                    SetHelpText(3, HStr("FE_HLPC"));

                    SetHelpText(4, "FE_HLPO");
                    SetHelpText(5, HStr("FE_HLPT"));
                }
                else {
                    SetHelpText(4, "FE_HLPI");
                    SetHelpText(5, HStr("FE_HLPT"));
                }
            }
            break;
        case MENUPAGE_GALLERY:
            if (currentInput == INPUT_STANDARD) {
                SetHelpText(0, HStr("FE_HLPM"));
                SetHelpText(4, "FE_HLPF");

                if (numGalleryPhotos <= 1)
                    SetHelpText(4, std::string());

                ProcessGallery(_this);
            }
            else {
                if (numGalleryPhotos <= 0)
                    SetHelpText(0, std::string());

            }
            break;
        }

#endif
    
        if (timeToWaitBeforeStateChange != -1 && timeToWaitBeforeStateChange < CTimer::m_snTimeInMillisecondsPauseMode) {
            switch (menuState) {
            case STATE_NONE:
                break;
            case STATE_OPENING: {
#if defined(GTA3) && !defined(LCSFICATION)
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
#if defined(GTA3) && !defined(LCSFICATION)
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

    static inline void DrawSlider(CMenuManager*_this, float x, float y, float progress, int type = 0) {
#if defined(GTA3) && defined(LCSFICATION)
        float _x = (x - ScaleX(-24.0f));
        float _y = (y);
        float _w = ScaleX(8.0f);
        float _h = ScaleY(11.0f);
        float spacing = ScaleX(4.0f);
#else
        float _x = (x);
        float _y = (y);
        float _w = ScaleX(3.5f);
        float _h = ScaleY(1.0f);
        float spacing = _w;
#endif
        float off = ScaleY(2.0f);

        for (uint32_t i = 0; i < 16; i++) {
            float current = i / 16.0f + 1 / 32.0f;
            CSprite2d::DrawRect(CRect(_x + off, _y - _h + off, _x + _w + off, _y + off), CRGBA(0, 0, 0, GetAlpha()));

            CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), CRGBA(
#if defined(GTA3) && defined(LCSFICATION)
                HUD_COLOUR_LCS_GREY,
#else
                HUD_COLOUR_BLUEDARK,
#endif
                    GetAlpha()));

            const bool selected = (aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_nAction == type) && currentInput == INPUT_STANDARD;
            if (current < progress)
                CSprite2d::DrawRect(CRect(_x, _y - _h, _x + _w, _y), 
#if defined(GTA3) && defined(LCSFICATION)
                   selected ? CRGBA(HUD_COLOUR_WHITE, GetAlpha()) : CRGBA(HUD_COLOUR_LCS_MENU, GetAlpha())
#else
                    CRGBA(HUD_COLOUR_BLUELIGHT, GetAlpha())
#endif
                   );

            if (CheckHover(_this, CRect(_x, _y - _h, _x + _w, _y)) == 2)
                _this->CheckSliderMovement(current < progress ? -1 : 1);

            _x += _w + spacing;

#if defined(GTA3) && defined(LCSFICATION)
#else
            _h += ScaleY(1.0f);
#endif
        }
    }
    
    static inline void SetHelpText(uint8_t id, std::string const& str) {
        helpTexts[id] = str;
    }

    static inline void ClearHelpText() {
        for (auto& it : helpTexts) {
            it.clear();
        }
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
#ifdef LCSFICATION
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(2);
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetColor(CRGBA(HUD_COLOUR_RED_LC01, GetAlpha()));
        CFont::SetFontStyle(2);
        CFont::SetScale(ScaleX(1.0f), ScaleY(1.8f));
        CFont::PrintString(ScaleX(26.0f + GetMenuOffsetX()), ScaleY(18.0f), UpperCase(textLoader.Get(tabs.at(currentTab).str)));
#else
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
        CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 44.0f + GetMenuOffsetX()), ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f), UpperCase(textLoader.Get(tabs.at(currentTab).str)));
#endif
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
            CFont::PrintString(ScaleX(40.0f), ScaleY(30.0f), textLoader.Get(aScreens[_this->m_nCurrentMenuPage].m_ScreenName));
        }
#endif
    }

    static inline void DrawHelpText(CMenuManager* _this) {
        float spacing = 16.0f;

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

#if defined(GTA3) && !defined(LCSFICATION) 
        CFont::SetScale(ScaleX(0.38f), ScaleY(0.64f));
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));

        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetFontStyle(0);

        float startx = ScaleXKeepCentered(50.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 94.0f);
#elif defined(GTA3) && defined(LCSFICATION) 
        CFont::SetDropShadowPosition(1);
        CFont::SetScale(ScaleX(0.35f), ScaleY(0.95f));

        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha(255)));
        CFont::SetColor(CRGBA(HUD_COLOUR_WHITE, GetAlpha(255)));
        CFont::SetFontStyle(0);

        float startx = ScaleX(26.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 70.0f);
        spacing += 8.0f;
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

        if ((_this->m_nCurrentMenuPage == MENUPAGE_MAP || _this->m_nCurrentMenuPage == MENUPAGE_GALLERY) &&
            currentInput == INPUT_STANDARD)
            starty += ScaleY(18.0f);
#endif
        float x = startx;
        float y = starty;

#if GTAVC
        if (!helpTexts[4].empty())
            spacing = 12.0f;
#endif

        uint32_t i = 0;
        for (auto& it : helpTexts) {
            if (!it.empty()) {
#if defined(GTA3) && defined(LCSFICATION)
                CFont::SetScale(ScaleX(0.35f), ScaleY(0.95f));
              
                if (CFont::GetStringWidth(it.c_str(), true) > ScaleX(80.0f))
                    CFont::SetScale(ScaleX(0.2f), ScaleY(0.95f));
#endif

                CFont::PrintString(x, y, textLoader.Get(it.c_str()));
            }

            y += ScaleY(spacing);

#if defined(GTA3) && !defined(LCSFICATION) 
            if (i == 2) {
                x += ScaleX(198.0f);
                y = starty;
            }
#elif defined(GTA3) && defined(LCSFICATION) 
            if (i == 1 || i == 3 || i == 5) {
                CFont::SetCentreOff();
                CFont::SetRightJustifyOn();
                x = SCREEN_WIDTH - ScaleX(40.0f);
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
        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)FALSE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

#if defined(GTA3) && defined(LCSFICATION)
#else
        CSprite2d::DrawRect(CRect(-5.0f, -5.0f, SCREEN_WIDTH + 5.0f, SCREEN_HEIGHT + 5.0f), CRGBA(0, 0, 0, 255));
#endif

#ifdef GTA3
#ifdef LCSFICATION
        //if (backgroundSprite.m_pTexture)
        //    backgroundSprite.Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(255, 255, 255, 255));
#else
        CSprite2d* splash = plugin::CallAndReturn<CSprite2d*, 0x48D550>("splash1"); // LoadSplash
        splash->Draw(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, CRGBA(50, 50, 50, 255));
#endif
#elif GTAVC
        _this->m_aMenuSprites[MENUSPRITE_BACKGROUND].Draw(CRect(ScaleXKeepCentered(0.0f), ScaleY(0.0f), ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH), ScaleY(DEFAULT_SCREEN_HEIGHT)), CRGBA(255, 255, 255, GetAlpha()));

        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP)
            _this->PrintMap();

        CMenuPoly pos = {};
        if (_this->m_nCurrentMenuPage == MENUPAGE_STATS) {
            pos.x1 = ScaleXKeepCentered(90.0f); pos.y1 = ScaleY(140.0f);  pos.x2 = ScaleXKeepCentered(542); pos.y2 = ScaleY(152.0f);
            pos.x3 = ScaleXKeepCentered(108.0f); pos.y3 = ScaleY(338.0f);  pos.x4 = ScaleXKeepCentered(534.0f); pos.y4 = ScaleY(322.0f);
            Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, CRGBA(HUD_COLOUR_AZURE, GetAlpha(130)));
        }
        else if (_this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_SAVE_SLOT ||
            _this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_LOAD_SLOT ||
            _this->m_nCurrentMenuPage == MENUPAGE_CHOOSE_DELETE_SLOT) {
            pos.x1 = ScaleXKeepCentered(76.0f); pos.y1 = ScaleY(88.0f);  pos.x2 = ScaleXKeepCentered(576.0f); pos.y2 = ScaleY(78.0f);
            pos.x3 = ScaleXKeepCentered(64.0f); pos.y3 = ScaleY(340.0f);  pos.x4 = ScaleXKeepCentered(556.0f); pos.y4 = ScaleY(354.0f);
            Draw2DPolygon(pos.x1, pos.y1, pos.x2, pos.y2, pos.x3, pos.y3, pos.x4, pos.y4, CRGBA(HUD_COLOUR_AZURE, GetAlpha(130)));
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

#if defined(GTA3) && !defined(LCSFICATION)
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

#if defined(GTA3) && defined(LCSFICATION)
        if (_this->m_nCurrentMenuPage == MENUPAGE_SKIN_SELECT)
            DrawPlayerSetupScreen(_this);

        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS)
            _this->DrawControllerSetupScreen();
#endif
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
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), _this->m_nPrefsBrightness / 512.0f, MENUACTION_BRIGHTNESS);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f + 22.0f), (_this->m_fPrefsLOD - 0.8f) * 1.0f, MENUACTION_DRAWDIST);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), _this->m_nPrefsMusicVolume / 128.0f, MENUACTION_MUSICVOLUME);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f + 22.0f), _this->m_nPrefsSfxVolume / 128.0f, MENUACTION_SFXVOLUME);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX()), ScaleY(92.0f), TheCamera.m_fMouseAccelHorzntal * 200.0f, MENUACTION_MOUSESENS);
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
#if defined(GTA3) && defined(GTA3_MENU_MAP) && !defined(LCSFICATION)
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

    static inline float GetTotalTabsWidth(CMenuManager* _this, int32_t start) {
        float spacing = ScaleX(28.0f);
        float w = -spacing;

        int32_t count = 0;
        for (int32_t i = start; i < tabs.size(); i++) {
            if (!IsTabAvailable(_this, i))
                continue;

            if (count > 3)
                break;

            auto& tab = tabs[i];
            w += CFont::GetStringWidth(textLoader.Get(tab.str), false);
            w += spacing;
            count++;
        }

        return w;
    }

    static inline void DrawOneTab(CMenuManager* _this, int32_t i, float x, float y, const plugin::char_t* str) {
        const float strWidth = CFont::GetStringWidth(str, false);

#if defined(GTA3) && defined(GTA3_MENU_MAP)
        if (menuMap) {
            if (i == TAB_MAP) {
                wchar_t* strBuff = LowerCase((wchar_t*)str);
                strBuff[0] = GetUpperCase(str[0]);
                str = strBuff;
            }
        }
#endif

#ifdef GTAVC 
        if (i == currentTab && currentInput == INPUT_TAB)
            UpdateItemPoly(_this, x + strWidth / 2, y + ScaleY(10.0f), ScaleX(4.0f) + strWidth / 2, ScaleY(18.0f));
#endif

        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

        CRect rect = {};
        rect.left = x + ScaleX(-4.0f);
        rect.top = y + ScaleY(-3.5f);
        rect.right = rect.left + ScaleX(130.0f);
        rect.bottom = rect.top + ScaleY(36.5f);

        CRGBA c = {};
#ifdef GTA3
#ifdef LCSFICATION
        c = CRGBA(HUD_COLOUR_ORANGELIGHT, GetAlpha());
#else
        c = CRGBA(0, 0, 0, GetAlpha());
#endif
#elif GTAVC
        c = CRGBA(225, 225, 225, GetAlpha());
#elif GTASA
        c = CRGBA(HUD_COLOUR_ORANGE, GetAlpha());
#endif

#if defined(GTA3) && !defined(LCSFICATION)
        if (i == currentTab)
            _this->m_aFrontEndSprites[FE2_TABACTIVE].Draw(rect, CRGBA(255, 255, 255, GetAlpha()));
#endif

#if defined(GTA3) && !defined(LCSFICATION)
        if (currentInput != INPUT_STANDARD || i == currentTab)
#endif 
        {
#if defined(GTA3) && !defined(LCSFICATION)
            rect.left = x + ScaleX(-4.0f);
            rect.top = y + ScaleY(-3.0f);
            rect.right = rect.left + ScaleX(64.0f);
            rect.bottom = rect.top + ScaleY(22.0f);
#else
            rect.left = x;
            rect.top = y + ScaleY(-3.0f);
            rect.right = x + strWidth;
            rect.bottom = rect.top + ScaleY(22.0f);
#endif

            //CSprite2d::DrawRect(rect, CRGBA(255, 255, 255, 255));
            uint8_t hover = CheckHover(_this, rect);

#if defined(GTA3) && defined(LCSFICATION)
            if (currentTab == i)
                c = CRGBA(HUD_COLOUR_RED_LC01, GetAlpha());
#endif

#ifdef GTASA
            if (currentTab == i)
                c = CRGBA(HUD_COLOUR_WHITE, GetAlpha());
#endif

            if (hover && currentTab != i) {
#ifdef GTA3
                c = CRGBA(HUD_COLOUR_GREYLIGHT, GetAlpha());
#elif GTAVC
                c = CRGBA(HUD_COLOUR_GREENLIGHT, GetAlpha());
#elif GTASA
                c = CRGBA(HUD_COLOUR_ORANGELIGHT, GetAlpha());
#endif

                if (hover == 2) {
                    SwitchTab(_this, i);
                }
            }

            CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
            CFont::SetColor(c);

            CFont::PrintString(x, y, str);
        }
    }

    static inline void DrawFront(CMenuManager* _this) {
        if (timeToWaitBeforeStateChange == -1)
            return;

        if (_this->m_nPrefsLanguage != previousLanguage) {
#ifdef GTAVC
            updateItemPoly = true;
            updateBackPoly = true;
#endif
            UpdateText(_this);
            previousLanguage = _this->m_nPrefsLanguage;
        }

        RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
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
        if ((_this->m_nCurrentMenuPage == MENUPAGE_MAP || _this->m_nCurrentMenuPage == MENUPAGE_GALLERY) && currentInput == INPUT_STANDARD)
            return;
#endif

#if defined(GTA3) || defined(GTASA)
        DrawHeader(_this);
#endif

#if defined(GTA3) && defined(LCSFICATION)
        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP && currentInput == INPUT_STANDARD)
            return;
#endif

        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS)
            return;

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

#if defined GTA3 && !defined(LCSFICATION)
        CFont::SetDropShadowPosition(0);
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetFontStyle(0);
        CFont::SetScale(ScaleX(0.28f), ScaleY(0.54f));
#elif defined GTA3 && defined(LCSFICATION)
        CFont::SetDropShadowPosition(2);
        CFont::SetRightJustifyOff();
        CFont::SetCentreOff();
        CFont::SetFontStyle(2);
        CFont::SetScale(ScaleX(0.5f), ScaleY(1.0f));

        switch (_this->m_nPrefsLanguage) {
            case 1:
            case 2:
            case 3:
            case 4:
                CFont::SetScale(ScaleX(0.5f) * 0.8f, ScaleY(1.0f));
                break;
        }
#elif GTAVC
        CFont::SetDropShadowPosition(2);
        CFont::SetRightJustifyOff();
        CFont::SetCentreOff();
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
        CFont::SetOrientation(ALIGN_LEFT);
        CFont::SetFontStyle(FONT_PRICEDOWN);
        CFont::SetScale(ScaleX(0.5f), ScaleY(1.0f));
        CFont::SetScaleForCurrentlanguage(ScaleX(0.5f), ScaleY(1.0f));
#endif

        float totalWidth = GetTotalTabsWidth(_this, 0);
        float startX = (SCREEN_WIDTH - totalWidth) / 2;
        float currentX = startX;
        float spacing = ScaleX(28.0f);
        float y = tabs[0].y;

        int32_t count = 0;
        for (int32_t i = 0; i < tabs.size(); i++) {
            if (!IsTabAvailable(_this, i))
                continue;

#if defined(GTA3) && !defined(LCSFICATION)
            currentX = ScaleXKeepCentered(tabs[count].x);
            y = tabs[count].y;
#else
            if (count == 4) { // second row
                totalWidth = GetTotalTabsWidth(_this, i);
                startX = (SCREEN_WIDTH - totalWidth) / 2;
                currentX = startX;
                y = tabs[4].y;
            }
#endif

            const plugin::char_t* str = textLoader.Get(tabs.at(i).str);
            const float strWidth = CFont::GetStringWidth(str, false);
            DrawOneTab(_this, i, currentX, ScaleY(y), str);
#if !defined(GTA3) || defined(LCSFICATION)
            currentX += strWidth + spacing;
#endif
            count++;
        }
    }

    static inline void CheckForExternalScripts() {
#if defined(GTA3) && defined(GTA3_MENU_MAP)
        const HMODULE h = ModuleList().GetByPrefix(L"menumap");
        if (h) {
            menuMap = true;
        }
#elif defined(GTASA)
            const HMODULE h = ModuleList().GetByPrefix(L"modloader");
            if (h) {
                modLoader = true;
            }
#endif

        GInput_Load(&gInputPad);
    }

    static inline void Init() {
#ifdef _DEBUG
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        std::setvbuf(stdout, NULL, _IONBF, 0);
#endif

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
#ifdef GTASA
                if (i == MENUPAGE_GALLERY || i == MENUPAGE_GALLERY_DELETE_PHOTO) {
                    aScreens[i].m_aEntries[j].m_nAction = MENUACTION_NOTHING;
                    aScreens[i].m_aEntries[j].m_EntryName[0] = '\0';
                    aScreens[i].m_aEntries[j].m_nSaveSlot = 0;
                    aScreens[i].m_aEntries[j].m_nTargetMenu = MENUPAGE_NONE;
                }
#endif

                if ((!strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FEDS_TB") && i != MENUPAGE_CHOOSE_LOAD_SLOT && i != MENUPAGE_CHOOSE_DELETE_SLOT) ||
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

        strcpy(aScreens[MENUPAGE_EXIT].m_ScreenName, "FEP_QUI");
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
#ifdef GTA3
        strcpy(aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_EntryName, "FEM_QT");
#else
        strcpy(aScreens[MENUPAGE_NEW_GAME].m_aEntries[3].m_EntryName, "FEP_QUI");
#endif
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

#ifdef LCSFICATION
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[1].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[2].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[3].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[4].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[5].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_aEntries[5].m_EntryName[0] = '\0';

        aScreens[MENUPAGE_SKIN_SELECT].m_aEntries[0].m_nAction = MENUACTION_NOTHING;
        aScreens[MENUPAGE_SKIN_SELECT].m_aEntries[0].m_EntryName[0] = '\0';
#endif
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
        strcpy(aScreens[MENUPAGE_GALLERY].m_ScreenName, "FEH_GAL");
        aScreens[MENUPAGE_GALLERY].m_nPreviousPage = MENUPAGE_GALLERY;

        strcpy(aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_ScreenName, "FEH_GAL");
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_nPreviousPage = MENUPAGE_GALLERY;

        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_nAction = MENUACTION_LABEL;
        strcpy(aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_EntryName, "FEG_DL2");
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_nX = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_nY = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_nAlign = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[0].m_nTargetMenu = MENUPAGE_GALLERY;

        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_nAction = MENUACTION_NO;
        strcpy(aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_EntryName, "FEM_NO");
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_nX = 320;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_nY = 210;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_nAlign = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[1].m_nTargetMenu = MENUPAGE_GALLERY;

        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_nAction = MENUACTION_DELETEGALLERYPHOTO;
        strcpy(aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_EntryName, "FEM_YES");
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_nX = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_nY = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_nAlign = 0;
        aScreens[MENUPAGE_GALLERY_DELETE_PHOTO].m_aEntries[2].m_nTargetMenu = MENUPAGE_NONE;

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

        strcpy(aScreens[MENUPAGE_CHOOSE_LOAD_SLOT].m_aEntries[9].m_EntryName, "FESZ_CA");
        aScreens[MENUPAGE_CHOOSE_LOAD_SLOT].m_aEntries[9].m_nAlign = 0;
        aScreens[MENUPAGE_CHOOSE_LOAD_SLOT].m_aEntries[9].m_nX = 0;
        aScreens[MENUPAGE_CHOOSE_LOAD_SLOT].m_aEntries[9].m_nY = 0;

        strcpy(aScreens[MENUPAGE_CHOOSE_DELETE_SLOT].m_aEntries[9].m_EntryName, "FESZ_CA");
        aScreens[MENUPAGE_CHOOSE_DELETE_SLOT].m_aEntries[9].m_nAlign = 0;
        aScreens[MENUPAGE_CHOOSE_DELETE_SLOT].m_aEntries[9].m_nX = 0;
        aScreens[MENUPAGE_CHOOSE_DELETE_SLOT].m_aEntries[9].m_nY = 0;

        strcpy(aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[9].m_EntryName, "FESZ_CA");
        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[9].m_nAlign = 0;
        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[9].m_nX = 0;
        aScreens[MENUPAGE_CHOOSE_SAVE_SLOT].m_aEntries[9].m_nY = 0;
#endif

        //Clear(_this);
    }

    static inline void InitAfterRw() {
        if (initialised)
            return;

#ifdef GTASA
        aScreens = plugin::patch::Get<CMenuScreen*>(0x576B37 + 1);
#endif

        CheckForExternalScripts();

#if defined(GTA3) && defined(GTA3_MENU_MAP)
        if (!menuMap) {
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_EntryName[0] = '\0';
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAction = MENUACTION_NOTHING;
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nTargetMenu = MENUPAGE_NONE;
        }
#endif

#ifdef GTASA
        int32_t slot = CTxdStore::FindTxdSlot("hud");

        if (slot != -1) {
            CTxdStore::SetCurrentTxd(slot);
            skipHighSprite.SetTexture("SkipHigh");
            CTxdStore::PopCurrentTxd();
        }

        if (modLoader && textLoader.Get("ML_F0HH")[0] != '\0') {
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nAction = MENUACTION_CHANGEMENU;
            strcpy(aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_EntryName, "ML_F0HH");
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nSaveSlot = 0;
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nTargetMenu = MENUPAGE_MODLOADER;
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nX = 0;
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nY = 0;
            aScreens[MENUPAGE_NEW_GAME].m_aEntries[4].m_nAlign = 1;
        }
#endif

        const char* pcbtns_name = PLUGIN_PATH("SkyUI\\txd\\pcbtns.txd");
        if (plugin::FileExists(pcbtns_name)) {
            int32_t pcbtns = CTxdStore::AddTxdSlot("pcbtns");
            CTxdStore::LoadTxd(pcbtns, pcbtns_name);
            CTxdStore::AddRef(pcbtns);
            CTxdStore::PushCurrentTxd();
            CTxdStore::SetCurrentTxd(pcbtns);

            int32_t i = 0;
            for (auto& it : pcbtnsSprites) {
                it.SetTexture((char*)pcbtnsSpritesNames[i]);
                i++;
            }
            CTxdStore::PopCurrentTxd();
        }

        initialised = true;
    }

    static inline void ShutdownAfterRw() {
#ifdef GTASA
        GalleryShutdown();
        skipHighSprite.Delete();
#endif

        GInput_Release();
        gInputPad = nullptr;
    }

    static inline void UpdateText(CMenuManager* _this) {
        std::string path = PLUGIN_PATH("SkyUI\\text\\");
        std::string lang = "american.txt";
        switch (FrontEndMenuManager.m_nPrefsLanguage) {
        case 0:
            break;
        case 1:
            lang = "french.txt";
            break;
        case 2:
            lang = "german.txt";
            break;
        case 3:
            lang = "italian.txt";
            break;
        case 4:
            lang = "spanish.txt";
            break;
        };

        path += lang;
        textLoader.Clear();
        textLoader.Load(path);
    }

    static inline void Clear(CMenuManager* _this, bool run = false) {
        if (saveMenuActive)
            currentInput = INPUT_STANDARD;
        else
            currentInput = INPUT_TAB;

#if defined(GTAVC) || defined(GTASA) || (defined(GTA3) && defined(LCSFICATION))
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
        scanGalleryPhotos = true;
#endif

#if defined(GTA3) && defined(LCSFICATION)
        currPlayerSkinPage = 0;
        currPlayerSkin = 0;
        playerSkinAnim = false;
#endif
    }

#ifdef GTASA
    static inline void GalleryShutdown() {
        for (auto& it : galleryPhotos) {
            if (it.texture) {
                RwTextureDestroy(it.texture);
                it.texture = nullptr;
            }

            it.id = 0;
        }
    }

    static inline void ProcessGallery(CMenuManager* _this) {
        if (galleryDeleteTimer > CTimer::m_snTimeInMillisecondsPauseMode)
            return;

        galleryDeleteTimer = 0;

        bool playSound = false;
        if (GetLeft()) {
            currentGalleryPhoto--;
            playSound = true;
        }
        else if (GetRight()) {
            currentGalleryPhoto++;
            playSound = true;
        }

        if (playSound) {
            if (numGalleryPhotos > 1)
                AudioEngine.ReportFrontendAudioEvent(FE_SOUND_SWITCH, 0.0f, 1.0f);
        }

        if (currentGalleryPhoto < 0)
            currentGalleryPhoto = numGalleryPhotos - 1;
        else if (currentGalleryPhoto > numGalleryPhotos - 1)
            currentGalleryPhoto = 0;


        if (GetEnter()) {
            SwitchMenuPage(_this, MENUPAGE_GALLERY_DELETE_PHOTO, true);
        }
    }

    static inline void ProcessGalleryDeletePic(CMenuManager* _this) {
        CFileMgr::SetDirMyDocuments();
        char buff[MAX_PATH];
        sprintf(buff, "Gallery\\gallery%d.jpg", galleryPhotos[currentGalleryPhoto].id);
        std::remove(buff);
        CFileMgr::SetDir("");

        scanGalleryPhotos = true;
        createGalleryPhotos = true;
        galleryDeleteTimer = CTimer::m_snTimeInMillisecondsPauseMode + 500;
    }

    static inline CRect ScaleImage(float imageWidth, float imageHeight, float targetWidth, float targetHeight, float x, float y) {
        float aspectRatio = imageWidth / imageHeight;

        float widthScaled = targetWidth;
        float heightScaled = widthScaled / aspectRatio;

        if (heightScaled > targetHeight) {
            heightScaled = targetHeight;
            widthScaled = heightScaled * aspectRatio;
        }

        return { x - (widthScaled / 2), y - (heightScaled / 2), x + (widthScaled / 2), y + (heightScaled / 2) };
    }

    static inline void ScanGallery(CMenuManager* _this) {
        if (!scanGalleryPhotos)
            return;

        GalleryShutdown();

        int32_t i = 1;
        numGalleryPhotos = 0;
        currentGalleryPhoto = 0;

        CFileMgr::SetDirMyDocuments();
        while (i < MAX_GALLERY_PICS) {
            char buff[MAX_PATH];
            sprintf(buff, "Gallery\\gallery%d.jpg", i);

            if (std::filesystem::exists(buff)) {
                if (createGalleryPhotos) {
                    galleryPhotos[numGalleryPhotos].id = i;
                    galleryPhotos[numGalleryPhotos].texture = LoadIMG(buff);
                }
                numGalleryPhotos++;
            }
            i++;
        }
        CFileMgr::SetDir("");

        if (numGalleryPhotos <= 0)
            EscTab(_this, false);

        createGalleryPhotos = false;
        scanGalleryPhotos = false;
    }

    static inline void DrawGallery(CMenuManager* _this) {
        ScanGallery(_this);

        if (currentInput == INPUT_TAB) {
            CFont::SetProportional(true);
            CFont::SetBackground(false, false);
            CFont::SetOrientation(ALIGN_CENTER);
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 100.0f));
            CFont::SetDropShadowPosition(2);
            CFont::SetDropColor(CRGBA(0, 0, 0, 255));
            CFont::SetColor(CRGBA(HUD_COLOUR_ORANGE, 255));
            CFont::SetFontStyle(FONT_PRICEDOWN);
            CFont::SetScale(ScaleX(2.1f), ScaleY(3.3f));

            char buff[8];
            sprintf(buff, "%d", numGalleryPhotos);
            CFont::PrintString(SCREEN_WIDTH / 2, ScaleY(161.0f), buff);

            CFont::SetDropShadowPosition(2);
            CFont::SetColor(CRGBA(HUD_COLOUR_AZUREDARK, 255));
            CFont::SetFontStyle(FONT_MENU);
            CFont::SetOrientation(ALIGN_LEFT);
            CFont::SetScale(ScaleX(0.41f), ScaleY(0.89f));

            strcpy(gString, textLoader.Get(HStr("FEG_HOW").c_str()));

            if (!HasPadInHands())
                CMessages::InsertPlayerControlKeysInString(gString);
            CFont::PrintString(ScaleXKeepCentered(100.0f), ScaleY(247.0f), gString);
        }
        else {
            const float w = (528.0f / 2);
            const float h = (362.0f / 2);

            if (numGalleryPhotos < currentGalleryPhoto)
                return;

            CSprite2d::DrawRect(CRect((SCREEN_WIDTH / 2) - ScaleX(w), (SCREEN_HEIGHT / 2) - ScaleY(h), (SCREEN_WIDTH / 2) + ScaleX(w), (SCREEN_HEIGHT / 2) + ScaleY(h)), CRGBA(HUD_COLOUR_BLACK, 255));

            RwTexture* t = galleryPhotos[currentGalleryPhoto].texture;
            if (t) {
                CRect rect = ScaleImage(t->raster->width, t->raster->height, ScaleX(w * 2), ScaleY(h * 2), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwTextureGetRaster(t));
                CSprite2d::SetVertices(rect, CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255));
                RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);

                CFont::SetProportional(true);
                CFont::SetBackground(false, false);
                CFont::SetOrientation(ALIGN_RIGHT);
                CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 100.0f));
                CFont::SetDropShadowPosition(0);
                CFont::SetEdge(2);
                CFont::SetDropColor(CRGBA(0, 0, 0, 255));
                CFont::SetColor(CRGBA(HUD_COLOUR_WHITE, 255));
                CFont::SetFontStyle(FONT_PRICEDOWN);
                CFont::SetScale(ScaleX(0.8f), ScaleY(0.8f));

                char buff[32];
                sprintf(buff, "%d/%d (%d)", currentGalleryPhoto + 1, numGalleryPhotos, numGalleryPhotos);
                CFont::PrintString((SCREEN_WIDTH / 2) + ScaleX(w - 33.0f), (SCREEN_HEIGHT / 2) + ScaleY(h - 37.0f), buff);
            }

            DrawUnfilledRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ScaleY(4.0f), ScaleX(w * 2), ScaleY(h * 2), CRGBA(HUD_COLOUR_BLACK, 255));
            DrawUnfilledRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ScaleY(3.0f), ScaleX(w * 2), ScaleY(h * 2), CRGBA(HUD_COLOUR_GREYDARK, 255));
        }
    }

    static void ProcessMenuOptions(CMenuManager* _this, int8_t arrows, bool* back, bool enter) {
        switch (aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_nAction) {
        case MENUACTION_DELETEGALLERYPHOTO:
            if (enter) {
                ProcessGalleryDeletePic(_this);
                _this->SwitchToNewScreen(MENUPAGE_GALLERY);
                return;
            }
        };
    }
#endif

#if defined(GTA3) && defined(LCSFICATION)
#ifdef LCSFICATION
    static void SetLCSFontStyle(bool white = false) {
        CFont::SetScale(ScaleX(LCS_MENU_SCALE_X * 0.9f), ScaleY(LCS_MENU_SCALE_Y * 0.9f));
        CFont::SetFontStyle(0);

        unsigned char a = min(CFont::Details.m_Color.a, GetAlpha());
        if (white)
            CFont::SetColor(CRGBA(HUD_COLOUR_WHITE, a));
        else
            CFont::SetColor(CRGBA(HUD_COLOUR_LCS_MENU, a));
    }
#endif

    static inline std::vector<CPlayerSkinData> playerSkins = {};
    static inline float playerRotation = -25.0f;
    static inline float playerRotationLerp = -25.0f;
    static inline bool playerSkinAnim = false;
    static inline CAnimBlendAssociation* prevMoveAssoc = nullptr;

    static std::string RemoveEx(std::string str) {
        size_t pos = str.find_last_of('.');
        if (pos != std::string::npos)
            str = str.substr(0, pos);
        return str;
    }

    static void ClearSkins() {
        playerSkins = {};
    }

    static void PushSkin(uint32_t id, const char* name, const char* displayName, const char* date) {
        CPlayerSkinData skin;
        skin.m_nSkinId = id;

        if (displayName)
            strcpy(skin.m_aSkinNameDisplayed, displayName);

        if (name)
            strcpy(skin.m_aSkinNameOriginal, name);

        if (date)
            strcpy(skin.m_aDateInfo, date);

        playerSkins.push_back(skin);
    }

    static void ScanPlayerSkins() {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile("skins\\*.bmp", &findFileData);
        SYSTEMTIME systemTime;

        int32_t i = 1;
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                char date[255];
                FileTimeToSystemTime(&findFileData.ftLastWriteTime, &systemTime);
                GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systemTime, 0, date, 255);

                PushSkin(i, findFileData.cFileName, RemoveEx(findFileData.cFileName).c_str(), date);

                i++;
            } while (FindNextFile(hFind, &findFileData) != 0);

            FindClose(hFind);
        }
    }

    static void ApplySkin(CMenuManager* _this, CPlayerSkinData* skin) {
        _this->m_pSelectedSkin = skin;
        strcpy(_this->m_nPrefsSkinFile, skin->m_aSkinNameDisplayed);
        CWorld::Players[0].SetPlayerSkin(_this->m_nPrefsSkinFile);
        _this->SaveSettings();
    }

#define DEFAULT_SKIN_NAME "$$\"\""
#define NUM_VISIBLE_ITEMS (14)

    static inline int32_t currPlayerSkinPage = 0;
    static inline int32_t currPlayerSkin = 0;

    static int32_t GetLastSkinOnCurrentPage() {
        int32_t last = NUM_VISIBLE_ITEMS * currPlayerSkinPage;
        const uint32_t lastSkin = (uint32_t)playerSkins.size();

        for (uint32_t i = NUM_VISIBLE_ITEMS * currPlayerSkinPage; i < lastSkin; i++) {
            if (last > NUM_VISIBLE_ITEMS * (currPlayerSkinPage + 1))
                break;

            if (i > lastSkin)
                break;

            last++;
        }

        return last - 1;
    }

    static int32_t GetLastPage() {
        int32_t items = 0;
        int32_t pages = 0;
        for (auto& it : playerSkins) {
            if (items > NUM_VISIBLE_ITEMS) {
                items = 0;
                pages++;
            }

            items++;
        }

        return pages;
    }

    static RpAtomic* GetAnimHierarchyCallback(RpAtomic* atomic, void* data) {
        *(RpHAnimHierarchy**)data = RpSkinAtomicGetHAnimHierarchy(atomic);
        return nullptr;
    }

    static inline RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump) {
        RpHAnimHierarchy* hier = nullptr;
        RpClumpForAllAtomics(clump, GetAnimHierarchyCallback, &hier);
        return hier;
    }

    static inline float playerZoom = 1.0f;
    static inline float playerZoomLerp = 1.0f;

    static void DrawPlayerSetupScreen(CMenuManager* _this) {
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);

        if (!_this->m_bSkinsFound) {
            ClearSkins();
            PushSkin(0, UnicodeToAscii((wchar_t*)textLoader.Get("FET_DSN")), RemoveEx(UnicodeToAscii((wchar_t*)textLoader.Get("FET_DSN"))).c_str(), nullptr);

            ScanPlayerSkins();

            // Fix default saved name
            if (!strcmp(_this->m_nPrefsSkinFile, DEFAULT_SKIN_NAME))
                strcpy(_this->m_nPrefsSkinFile, playerSkins[0].m_aSkinNameDisplayed);

            _this->m_bSkinsFound = true;
        }

        float x = 206.0f;
        float y = 96.0f;

        // Render menu
        CFont::SetPropOn();
        CFont::SetBackgroundOff();
        CFont::SetCentreOff();
        CFont::SetRightJustifyOff();
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetDropShadowPosition(2);
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha()));
        CFont::SetColor(CRGBA(HUD_COLOUR_RED_LC01, GetAlpha()));
        CFont::SetFontStyle(2);
        CFont::SetScale(ScaleX(0.4f), ScaleY(0.8f));
        CFont::PrintString(ScaleX(x + GetMenuOffsetX()), ScaleY(y), textLoader.Get("FES_SKN"));

        CFont::SetRightJustifyOn();
        CFont::PrintString(ScaleX(x + 256.0f + GetMenuOffsetX()), ScaleY(y), textLoader.Get("FES_DAT"));

        CFont::SetScale(ScaleX(0.3f), ScaleY(0.6f));
        CFont::SetFontStyle(0);
        CFont::SetDropShadowPosition(0);

        CPad* pad = CPad::GetPad(0);
        const int32_t lastPage = GetLastPage();
        const int32_t lastSkin = (uint32_t)playerSkins.size();
        const int32_t lastSkinForThisPage = GetLastSkinOnCurrentPage();

        bool left = pad->NewKeyState.left && !pad->OldKeyState.left;
        bool right = pad->NewKeyState.right && !pad->OldKeyState.right;
        bool up = pad->NewKeyState.up && !pad->OldKeyState.up;
        bool down = pad->NewKeyState.down && !pad->OldKeyState.down;

        bool zoomIn = pad->NewMouseControllerState.wheelUp;
        bool zoomOut = pad->NewMouseControllerState.wheelDown;

        bool enter = (pad->NewKeyState.extenter && !pad->OldKeyState.extenter) || (pad->NewKeyState.enter && !pad->OldKeyState.enter);

        float rot = 0.0f;

        if (!playerSkins.empty()) {
            if (currentInput == INPUT_STANDARD) {
                if (HasPadInHands()) {
                    enter |= pad->NewState.ButtonCross && !pad->OldState.ButtonCross;
                    left |= pad->NewState.DPadLeft && !pad->OldState.DPadLeft;
                    right |= pad->NewState.DPadRight && !pad->OldState.DPadRight;
                    up |= pad->NewState.DPadUp && !pad->OldState.DPadUp;
                    down |= pad->NewState.DPadDown && !pad->OldState.DPadDown;
                    zoomIn |= pad->NewState.RightShoulder1;
                    zoomOut |= pad->NewState.LeftShoulder1;

                    rot = pad->NewState.RightStickX / 14.0f;
                }

                if (enter) {
                    auto& skin = playerSkins[currPlayerSkin];
                    ApplySkin(_this, &skin);
                }

                if (lastPage > 0) {
                    if (left) {
                        currPlayerSkinPage--;
                        currPlayerSkin = 0;
                    }
                    else if (right) {
                        currPlayerSkinPage++;
                        currPlayerSkin = 0;
                    }
                }

                if (up)
                    currPlayerSkin--;
                else if (down)
                    currPlayerSkin++;

                if (currPlayerSkinPage < 0)
                    currPlayerSkinPage = 0;
                else if (currPlayerSkinPage > lastPage)
                    currPlayerSkinPage = lastPage;

                if (currPlayerSkin < NUM_VISIBLE_ITEMS * currPlayerSkinPage)
                    currPlayerSkin = NUM_VISIBLE_ITEMS * currPlayerSkinPage;
                else if (currPlayerSkin > lastSkinForThisPage)
                    currPlayerSkin = lastSkinForThisPage;

                if (zoomOut) {
                    playerZoom += 0.1f;
                }
                else if (zoomIn) {
                    playerZoom -= 0.1f;
                }

                playerZoom = CLAMP(playerZoom, 0.3f, 1.0f);

                static uint32_t prevTime = 0;
                const float f = (CTimer::m_snTimeInMillisecondsPauseMode - prevTime) * 0.02f;
                playerZoomLerp = std::lerp(playerZoomLerp, playerZoom, f * 0.5f);
                prevTime = CTimer::m_snTimeInMillisecondsPauseMode;

            }

            uint32_t renderedItems = 0;
            CRGBA col = { HUD_COLOUR_LCS_MENU, (uint8_t)GetAlpha() };

            for (uint32_t i = NUM_VISIBLE_ITEMS * currPlayerSkinPage; i < lastSkin; i++) {
                if (renderedItems > NUM_VISIBLE_ITEMS)
                    break;

                if (i > lastSkin)
                    break;

                auto& skin = playerSkins[i];
                CFont::SetRightJustifyOff();
                float nexty = 12.0f * renderedItems;

                if (_this->m_bShowMouse &&
                    _this->CheckHover(ScaleX(x + GetMenuOffsetX()), ScaleX(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), ScaleY(y + 22.0f + nexty + 12.0f))) {
                    
                    if (currPlayerSkin != i)
                        currPlayerSkin = i;

                    if (pad->NewMouseControllerState.lmb && !pad->OldMouseControllerState.lmb) {
                        ApplySkin(_this, &skin);
                    }
                }

                CFont::SetColor(CRGBA(HUD_COLOUR_LCS_MENU, GetAlpha()));

                if (currentInput == INPUT_STANDARD) {
                    if (!strcmp(skin.m_aSkinNameDisplayed, _this->m_nPrefsSkinFile))
                        CFont::SetColor(CRGBA(HUD_COLOUR_YELLOW_LIGHT, GetAlpha()));

                    if (i == currPlayerSkin)
                        CFont::SetColor(CRGBA(HUD_COLOUR_WHITE, GetAlpha()));
                }

                CFont::PrintString(ScaleX(x + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), skin.m_aSkinNameDisplayed);

                CFont::SetRightJustifyOn();
                CFont::PrintString(ScaleX(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), skin.m_aDateInfo);

                renderedItems++;
            }

            CRGBA c = { 255, 255, 255, 255 };
            CRect rect;
            rect.left = ScaleX(x + GetMenuOffsetX());
            rect.top = ScaleY(y + 20.0f);

            rect.right = ScaleX(x + 256.0f + GetMenuOffsetX());
            rect.bottom = ScaleY(y + 20.0f + 0.5f);

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            CSprite2d::SetVertices(rect, c, c, c, c, 0);
            RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);

            rect.left = ScaleX(x + GetMenuOffsetX());
            rect.top = ScaleY(y + 38.0f + 12.0f * NUM_VISIBLE_ITEMS);

            rect.right = ScaleX(x + 256.0f + GetMenuOffsetX());
            rect.bottom = ScaleY(y + 38.5f + 12.0f * NUM_VISIBLE_ITEMS);

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            CSprite2d::SetVertices(rect, c, c, c, c, 0);
            RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);

            CFont::SetColor(CRGBA(HUD_COLOUR_LCS_MENU, GetAlpha()));

            char buff[256];
            sprintf(buff, "Current: %s", _this->m_nPrefsSkinFile);
            CFont::SetRightJustifyOff();
            CFont::PrintString(ScaleX(x + GetMenuOffsetX()), ScaleY(y + 42.0f + 12.0f * NUM_VISIBLE_ITEMS), buff);

            sprintf(buff, "%d/%d", currPlayerSkinPage + 1, lastPage + 1);
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleX(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 42.0f + 12.0f * NUM_VISIBLE_ITEMS), buff);

            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

            CRGBA ssc = CRGBA(40, 40, 50, 255);
            skinSelSprite.Draw(CRect((SCREEN_WIDTH / 2) + ScaleX(38.0f - ((1.0f - playerZoomLerp) * 128.0f)), ScaleY(4.0f), (SCREEN_WIDTH / 2) + ScaleX(38.0f + 160.0f + ((1.0f - playerZoomLerp) * 128.0f)), SCREEN_HEIGHT - ScaleY(96.5f)), ssc);

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        }

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);

        D3DVIEWPORT8 previousViewport = {};
        D3DVIEWPORT8 newViewport = {};
        newViewport.X = 0.0f; // (SCREEN_WIDTH / 2) + ScaleX(38.0f);
        newViewport.Y = ScaleY(4.0f), (SCREEN_WIDTH / 2) + ScaleX(198.0f);
        newViewport.Width = SCREEN_WIDTH;// (SCREEN_WIDTH / 2) + ScaleX(198.0f);
        newViewport.Height = SCREEN_HEIGHT - ScaleY(96.5f);

        newViewport.Width -= newViewport.X;
        newViewport.Height -= newViewport.Y;

        GetD3DDevice<IDirect3DDevice8>()->GetViewport(&previousViewport);
        GetD3DDevice<IDirect3DDevice8>()->SetViewport(&newViewport);

        // Render player
        RwV3d pos = { -1.15f, -0.05f, 8.0f };
        pos.x *= playerZoomLerp;
        pos.y -= 1.0f - max(playerZoomLerp, 0.43f);
        pos.z *= playerZoomLerp;

        const RwV3d axis1 = { 1.0f, 0.0f, 0.0f };
        const RwV3d axis2 = { 0.0f, 0.0f, 1.0f };
        const RwV3d scale = { 0.7f, 1.0f, 1.0f };

        static uint32_t prevTime = 0;
        
        // Input   
        if (currentInput == INPUT_STANDARD 
            &&  ((CPad::GetPad(0)->NewMouseControllerState.lmb
                && _this->m_nMousePosX >= (SCREEN_WIDTH / 2) + ScaleX(48.0f) && _this->m_nMousePosX <= (SCREEN_WIDTH / 2) + ScaleX(64.0f + 128.0f))
            || rot))
            playerRotation += (_this->m_nMousePosX - _this->m_nMouseOldPosX) + rot;
        //else
        //    playerRotation += (CTimer::m_snTimeInMillisecondsPauseMode - prevTime) * 0.02f;
        
        
        if (playerRotation > 360.0f)
            playerRotation -= 360.0f;

        const float f = (CTimer::m_snTimeInMillisecondsPauseMode - prevTime) * 0.02f;
        playerRotationLerp = std::lerp(playerRotationLerp, playerRotation, f * 0.5f);
        
        prevTime = CTimer::m_snTimeInMillisecondsPauseMode;

        RpClump* clump = gpPlayerClump;

        // Make anims
        //CPlayerPed* playa = FindPlayerPed();
        //
        //if (currentInput == INPUT_STANDARD) {
        //    if (playa) {
        //        clump = playa->m_pRwClump;
        //
        //        if (!playerSkinAnim) {
        //            prevMoveAssoc = RpAnimBlendClumpGetAssociation(clump, 0);
        //            CAnimBlendAssociation* newMoveAssoc = CAnimManager::AddAnimation(clump, playa->m_nAnimGroupId, 0);
        //            playerSkinAnim = true;
        //        }
        //
        //        RpAnimBlendClumpUpdateAnimations(clump, 1.0f / 50.0f);
        //
        //        RpHAnimHierarchy* hier = GetAnimHierarchyFromSkinClump(clump);
        //        RpHAnimHierarchyUpdateMatrices(hier);
        //    }
        //}
        //else {
        //    if (playa) {
        //        if (playerSkinAnim) {
        //            clump = playa->m_pRwClump;
        //
        //            if (prevMoveAssoc)
        //                prevMoveAssoc->SetCurrentTime(0.0f);
        //
        //            RpAnimBlendClumpUpdateAnimations(clump, 1.0f / 50.0f);
        //
        //            RpHAnimHierarchy* hier = GetAnimHierarchyFromSkinClump(clump);
        //            RpHAnimHierarchyUpdateMatrices(hier);
        //
        //            playerSkinAnim = false;
        //        }
        //    }
        //}

        RwFrame* frame = (RwFrame*)clump->object.parent;

        RwFrameTransform(frame, RwFrameGetMatrix(RwCameraGetFrame(Scene.m_pCamera)), rwCOMBINEREPLACE);
        RwFrameScale(frame, &scale, rwCOMBINEPRECONCAT);
        RwFrameTranslate(frame, &pos, rwCOMBINEPRECONCAT);
        RwFrameRotate(frame, &axis1, -90.0f, rwCOMBINEPRECONCAT);
        RwFrameRotate(frame, &axis2, playerRotationLerp, rwCOMBINEPRECONCAT);

        RwFrameUpdateObjects(frame);
        
        RwRGBAReal ambientColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        SetAmbientColours(&ambientColor);
        
        RpClumpRender(clump);

        GetD3DDevice<IDirect3DDevice8>()->SetViewport(&previousViewport);
    }
#endif

    static bool isAButton(short chr) {
        short c = chr + ' ';

        switch (c) {
        case '<': // left
            return true;
        case '>': // right
            return true;
        case ';': // up
            return true;
        case '=': // down
            return true;
        }

        return false;
    };

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

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
#endif
            CFont::PrintString(ScaleXKeepCentered(52.0f + GetMenuOffsetX()), y + ScaleY(38.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x484F3A, LAMBDA(void, __cdecl, drawBriefs, float, float, wchar_t*));

        auto drawLeftString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            float x = ScaleX(64.0f);

            if (CFont::Details.m_bCentre)
                x = SCREEN_WIDTH / 2;

            CFont::PrintString(x, y, str);
#else
            float center = 0.0f;

            if (CFont::Details.m_bCentre)
                center = 240.0f;

            CFont::PrintString(ScaleXKeepCentered(64.0f + center + GetMenuOffsetX()), y, str);
#endif
        };
        plugin::patch::RedirectCall(0x47C666, LAMBDA(void, __cdecl, drawLeftString, float, float, wchar_t*));

        auto drawRightString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            CFont::PrintString(SCREEN_WIDTH - ScaleX(64.0f + GetMenuOffsetX()), y, str);
#else
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), y, str);
#endif
        };
        plugin::patch::RedirectCall(0x47C74C, LAMBDA(void, __cdecl, drawRightString, float, float, wchar_t*));
        plugin::patch::SetChar(0x47DA11 + 6, 255);

        // Stats
        auto drawCrimRa = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
            str = UpperCase(str);
#endif
            CFont::PrintString(ScaleXKeepCentered(64.0f + GetMenuOffsetX()), ScaleY(52.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x4825AD, LAMBDA(void, __cdecl, drawCrimRa, float, float, wchar_t*));

        auto drawRate = [](float, float, wchar_t* str) {
            char buff[32];
            sprintf(buff, "%d", CStats::FindCriminalRatingNumber());

            float x = CFont::GetStringWidth(buff, 1);
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
            str = UpperCase(str);
#endif
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX() - 16.0f) - x, ScaleY(52.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x482620, LAMBDA(void, __cdecl, drawRate, float, float, wchar_t*));

        auto drawRate2 = [](float, float, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());
            CFont::SetRightJustifyOn();

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
            str = UpperCase(str);
#endif
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), ScaleY(52.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x4826D0, LAMBDA(void, __cdecl, drawRate2, float, float, wchar_t*));

        auto drawStatLeft = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle(true);
#endif
            CFont::PrintString(ScaleXKeepCentered(64.0f + GetMenuOffsetX()), y + ScaleY(30.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x4824E1, LAMBDA(void, __cdecl, drawStatLeft, float, float, wchar_t*));


        auto drawStatRight = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle(true);
#endif
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 64.0f + GetMenuOffsetX()), y + ScaleY(30.0f + MENU_OFFSET_Y), str);
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

        auto drawBriefs = [](float, float y, wchar_t* str) {
            CFont::SetWrapx(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 82.0f + GetMenuOffsetX()));
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
#endif
            CFont::PrintString(ScaleXKeepCentered(92.0f + GetMenuOffsetX()), y, str);
        };
        plugin::patch::RedirectCall(0x49A59F, LAMBDA(void, __cdecl, drawBriefs, float, float, wchar_t*));
        
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

        plugin::Events::shutdownRwEvent += []() {
#ifdef GTASA
            ShutdownAfterRw();
#endif
        };

        onProcess.before += [](CMenuManager* _this) {
            if (!saveMenuActive && _this->m_bSaveMenuActive) {
                SwitchTab(_this, TAB_SAV);
                saveMenuActive = true;
            }

            if (menuActive || saveMenuActive)
                Process(_this);
        };

        onProcess.after += [](CMenuManager* _this) {
            if (_this->m_bMenuActive || _this->m_bSaveMenuActive) {
                if (!menuActive) {
                    Clear(_this, true);
                    menuActive = true;
                }
            }
            else {
                saveMenuActive = false;
                menuActive = false;
                Clear(_this, false);
            }
        };


#if defined(GTA3) || defined(GTAVC)
        onDrawStandardMenu.before += [](CMenuManager* _this) {
            DrawBack(_this);
        };
#endif

        onDrawStandardMenu.after += [](CMenuManager* _this) {
            DrawFront(_this);

#ifdef GTASA
            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_GALLERY:
                DrawGallery(_this);
                break;
            };
#endif
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
            if (currentInput == INPUT_STANDARD) {
                if (GetCheckHoverForStandardInput(_this)) {
                    _this->UserInput();
                }
            }
        };
#endif

#if defined(GTA3) && defined(LCSFICATION)
        // fontstyles
        plugin::patch::SetUChar(0x47B404 + 1, 0);
        plugin::patch::SetUChar(0x47B39B + 1, 0);
        plugin::patch::SetUChar(0x47B2B7 + 1, 0);
        plugin::patch::SetUChar(0x47B32F + 1, 0);
        plugin::patch::SetUChar(0x47B167 + 1, 0);
        plugin::patch::SetUChar(0x47B1D3 + 1, 0);
        plugin::patch::SetUChar(0x48270A + 1, 0);

        // font scale
        plugin::patch::SetFloat(0x47B415 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B406 + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B3AC + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B39D + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B2C8 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B2B9 + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B250 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B241 + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B340 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B331 + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B178 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B169 + 6, LCS_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B1E4 + 6, LCS_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B1D5 + 6, LCS_MENU_SCALE_Y); // y

        // item col selected
        static uint8_t colSelected[] = { HUD_COLOUR_WHITE, 255 };
        plugin::patch::SetUChar(0x47C782 + 1, colSelected[0]);
        plugin::patch::SetUChar(0x47C77D + 1, colSelected[1]);
        plugin::patch::SetUChar(0x47C77B + 1, colSelected[2]);

        // item col 
        static uint8_t col[] = { HUD_COLOUR_LCS_MENU, 255 };
        plugin::patch::SetUChar(0x47C7B0 + 1, col[0]);
        plugin::patch::SetUChar(0x47C7AB + 1, col[1]);
        plugin::patch::SetUChar(0x47C7A9 + 1, col[2]);

        static uint8_t colDisabled[] = { HUD_COLOUR_LCS_GREY, 255 };
        plugin::patch::SetUChar(0x47C6E2 + 1, colDisabled[0]);
        plugin::patch::SetUChar(0x47C6E0 + 1, colDisabled[1]);
        plugin::patch::SetUChar(0x47C6DE + 1, colDisabled[2]);

        // action
        plugin::patch::SetUChar(0x47B013 + 1, col[0]);
        plugin::patch::SetUChar(0x47B00E + 1, col[1]);
        plugin::patch::SetUChar(0x47B00C + 1, col[2]);

        // redefine controls page
        // no control header
        plugin::patch::Nop(0x4813DB, 5);

        // global y
        plugin::patch::SetFloat(0x5F36D4, 50.0f);

        // rect h
        plugin::patch::SetFloat(0x5F3968, 118.0f);

        plugin::patch::SetInt(0x481990 + 1, 0x31 + 0x8);
        plugin::patch::SetInt(0x481997 + 1, 0x36 + 0x8);

        static uint8_t boundcol[] = { HUD_COLOUR_WHITE, 255 };
        plugin::patch::SetUChar(0x489D02 + 1, boundcol[0]);
        plugin::patch::SetUChar(0x489D00 + 1, boundcol[1]);
        plugin::patch::SetUChar(0x489CFE + 1, boundcol[2]);

        plugin::patch::SetUChar(0x489F32 + 1, boundcol[0]);
        plugin::patch::SetUChar(0x489F30 + 1, boundcol[1]);
        plugin::patch::SetUChar(0x489F2E + 1, boundcol[2]);

        // rect
        static uint8_t rectcol[] = { HUD_COLOUR_BLUELIGHT, 0 };
        plugin::patch::SetUChar(0x481751 + 1, rectcol[0]);
        plugin::patch::SetUChar(0x48174C + 1, rectcol[1]);
        plugin::patch::SetUChar(0x48174A + 1, rectcol[2]);
        plugin::patch::SetUChar(0x48173B + 1, rectcol[3]);

        plugin::patch::Nop(0x489FEB, 5);
        plugin::patch::Nop(0x48A051, 5);
        //

        // Setup skin
        plugin::patch::Nop(0x4876F7, 5);
        plugin::patch::Nop(0x487D85, 5);

        // global y
        plugin::patch::SetInt(0x47FB6C + 1, 0x2E + 0x14);
        plugin::patch::SetInt(0x47FB79 + 4, 0x39 + 0x14);
        plugin::patch::SetInt(0x47FB81 + 1, 0x2F + 0x14);

        // global w/h
        static float val = 0.5f;
        plugin::patch::SetPointer({ 0x48185E + 0x2, 0x48184D + 0x2 }, &val);

        plugin::patch::Nop(0x4809B9, 7);
        plugin::patch::Nop(0x4809D3, 10);
        plugin::patch::Nop(0x481010, 5);
        plugin::patch::Nop(0x47F3E8, 5);

        plugin::patch::Nop(0x47AAFC, 7);
        plugin::patch::RedirectJump(0x47AAF5, (void*)0x47AB10);

        plugin::patch::SetChar(0x4871FD + 6, -1);
        plugin::patch::Nop(0x4871D9, 7);
        plugin::patch::Nop(0x487277, 5);
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
        plugin::patch::SetUChar(0x57C2E4, 0xEB);

        onProcessMenuOptions += [](CMenuManager* _this, int8_t arrows, bool* back, bool enter) {
            ProcessMenuOptions(_this, arrows, back, enter);
        };

        auto drawTripSkipSprite = [](CSprite2d* sprite, uint32_t, CRect const& rect, CRGBA const& col) {
            const float x = rect.left;
            const float y = rect.top;

            const float w = (rect.right - rect.left) / 2;
            const float h = (rect.bottom - rect.top) / 2;

            sprite->Draw(CRect(x, y, x + w, y + h), col); // Left top
            sprite->Draw(CRect(x + w + w, y, x + w, y + h), col); // Right top

            sprite->Draw(CRect(x, y + h + h, x + w, y + h), col); // Left bottom
            sprite->Draw(CRect(x + w + w, y + h + h, x + w, y + h), col); // Right bottom

            if (FLASH_ITEM(1000, 500))
                skipHighSprite.Draw(x + (w * 1.15f), y + (h * 0.775f), w * 0.425f, h * 0.425f, col);
        };
        plugin::patch::RedirectCall(0x58A1F3, LAMBDA(void, __fastcall, drawTripSkipSprite, CSprite2d* sprite, uint32_t, CRect const& rect, CRGBA const& col));
#endif

#if defined(GTA3) && defined(LCSFICATION)
        static bool spriteLoaded = false;
        onLoadAllMenuTextures += [](CMenuManager* _this) {
            if (spriteLoaded)
                return;

            int32_t slot = CTxdStore::FindTxdSlot("menu"); 
            if (slot != -1) {
                CTxdStore::SetCurrentTxd(slot);
                skinSelSprite.SetTexture("skinSel");
                CTxdStore::PopCurrentTxd();
            }

            spriteLoaded = true;
        };

        onUnloadMenuTextures.before += [](CMenuManager* _this) {
            if (!spriteLoaded)
                return;

            skinSelSprite.Delete();

            spriteLoaded = false;
        };
#endif

#if defined(GTA3) && defined(FRAME_LIMITER)
        static double previousTime = 0.0;
        static double startTime = -1000.0;
        static int32_t frameCounter = 0;

        onProcessGameState7.before += []() {
            if (startTime == -1000.0)
                startTime = timer.GetTimeInMilliseconds<double>();

            previousTime = timer.GetTimeInMilliseconds<double>();
        };

        onProcessGameState7.after += []() {
            if (!FrontEndMenuManager.m_bPrefsFrameLimiter)
                return;

            double currentTime = timer.GetTimeInMilliseconds<double>();
            double deltaTime = currentTime - previousTime;

            previousTime = currentTime;

            const double targetFrameDuration = 1000.0f / RsGlobal.maxFPS;
            double remainingTime = targetFrameDuration - deltaTime;

            while (remainingTime > 0.0) {
                currentTime = timer.GetTimeInMilliseconds<double>();
                deltaTime = currentTime - previousTime;
                remainingTime = targetFrameDuration - deltaTime;
            }

            frameCounter++;

            if (timer.GetTimeInMilliseconds<double>() - startTime >= 1000.0f) {
                double averageFrameTime = (timer.GetTimeInMilliseconds<double>() - startTime) / frameCounter;
                startTime = timer.GetTimeInMilliseconds<double>();
                frameCounter = 0;
            }
        };
#endif

#if defined(GTA3) && defined(LCSFICATION)
        static float& NumberOfChunksLoaded = *(float*)0x9403BC;
        static const char* prevScreen = nullptr;
        auto drawLoadingScreen = [](const char* str1, const char* str2, const char* splash) {
            if (RsGlobal.quit)
                return;

            FrontEndMenuManager.LoadAllTextures();

            CSprite2d* sprite = LoadSplash(splash);
            if (DoRwStuffStartOfFrame(0, 0, 0, 0, 0, 0, 255)) {
                CSprite2d::SetRecipNearClip();
                CSprite2d::InitPerFrame();
                CFont::InitPerFrame();
                DefinedState();

                sprite->Draw(CRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(255, 255, 255, 255));

                if (str1) {
                    DrawProgressBar(ScaleX(44.0f), SCREEN_HEIGHT - ScaleY(56.0f), ScaleX(172.0f), ScaleY(10.0f), NumberOfChunksLoaded, CRGBA(HUD_COLOUR_RED_LC01, 255), CRGBA(HUD_COLOUR_GREYDARK, 255));
                    FrontEndMenuManager.m_aMenuSprites[MENUSPRITE_GTALOGO].Draw(ScaleX(44.0f - 24.0f), SCREEN_HEIGHT - ScaleY(56.0f + 192.0f), ScaleX(226.0f), ScaleY(226.0f), CRGBA(255, 255, 255, 255));
                }

                DoRwStuffEndOfFrame();
            }

            NumberOfChunksLoaded += 0.1f;// * (previousTime - timer.GetTimeInMilliseconds());
            previousTime = timer.GetTimeInMilliseconds();

            FrontEndMenuManager.UnloadTextures();
        };

        //plugin::patch::RedirectJump(0x48D770, LAMBDA(void, __cdecl, drawLoadingScreen, const char*, const char*, const char*));
        // SaveLoader crash workaround
        plugin::patch::RedirectCall({ 
            0x47627A,
            0x48BB70,
            0x48BF0C,
            0x48BF43,
            0x48BF84,
            0x48C069,
            0x48C0B0,
            0x48C0F6,
            0x48C194,
            0x48C1BC,
            0x48C1D5,
            0x48C1F8,
            0x48C216,
            0x48C263,
            0x48C281,
            0x48C2DC,
            0x48C2F5,
            0x48C327,
            0x48C34F,
            0x48E7E9,
            0x582D9E,
            0x582DD1,
            0x592C50,
            }, LAMBDA(void, __cdecl, drawLoadingScreen, const char*, const char*, const char*));


        // menu background 
        auto changeBackgroundHack = []() {
            CMenuManager* _this = &FrontEndMenuManager;
            uint8_t prev = MENUSPRITE_FINDGAME;
            uint8_t curr = MENUSPRITE_FINDGAME;

            switch (_this->m_nPreviousMenuPage) {
            case MENUPAGE_SKIN_SELECT:
                prev = MENUSPRITE_PLAYERSET;
                break;
            };

            switch (_this->m_nCurrentMenuPage) {
            case MENUPAGE_SKIN_SELECT:
                curr = MENUSPRITE_PLAYERSET;
                break;
            };

            // prev
            plugin::patch::SetUChar(0x47A61D + 1, prev);

            // curr
            plugin::patch::SetUChar(0x47A740 + 1, curr);
        };
        plugin::patch::RedirectCall(0x47A5D1, LAMBDA(void, __cdecl, changeBackgroundHack));
#endif

#ifdef GTA3        
        static bool wasAButton = false;
        auto printCharButton = [](float x, float y, short chr) {
            float w = CFont::Details.m_vScale.y * 14.0f;
            float h = CFont::Details.m_vScale.y * 14.0f;
            short c = chr + ' ';    

            wasAButton = true;
            switch (c) {
            case '<': // left
                pcbtnsSprites.at(PCBTN_LEFT).Draw(x, y, w, h, CRGBA(255, 255, 255, 255));
                return;
            case '>': // right
                pcbtnsSprites.at(PCBTN_RIGHT).Draw(x, y, w, h, CRGBA(255, 255, 255, 255));
                return;
            case ';': // up
                pcbtnsSprites.at(PCBTN_UP).Draw(x, y, w, h, CRGBA(255, 255, 255, 255));
                return;
            case '=': // down
                pcbtnsSprites.at(PCBTN_DOWN).Draw(x, y, w, h, CRGBA(255, 255, 255, 255));
                return;
            }

            CFont::PrintChar(x, y, chr);
            wasAButton = false;
        };
       
        plugin::patch::RedirectCall(0x50179F, LAMBDA(void, __cdecl, printCharButton, float, float, short));

       //auto getCharacterSize = [](short c) {
       //    float value = 0.0f;
       //
       //    short** size = (short**)0x501910 + 4;
       //
       //    if (isAButton(c))
       //        value = 14.0f;
       //    else {
       //        if (CFont::Details.m_bProp)
       //            value = size[CFont::Details.m_nStyle][c];
       //        else
       //            value = size[CFont::Details.m_nStyle][192];
       //    }
       //
       //    return value * CFont::Details.m_vScale.x;
       //};
       //plugin::patch::RedirectJump(0x501840, LAMBDA(float, __cdecl, getCharacterSize, short));

#endif
 }

    ~SkyUI() {

    }
} skyUI;
