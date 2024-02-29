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
#include "CWorld.h"
#include "CModelInfo.h"
#include "CAnimManager.h"
#include "CDirectory.h"
#include "CFileMgr.h"

#include "SpriteLoader.h"
#include "TextLoader.h"
#include "Timer.h"
#include "extensions\ScriptCommands.h"

#ifdef GTA3
#include "cDMAudio.h"
#include "CPlayerSkin.h"
#include "CScene.h"
#include "dxsdk/d3d8.h"
#include "CCivilianPed.h"
#include "CPopulation.h"
#include "CCutsceneMgr.h"
#include "CClumpModelInfo.h"
#include "CPed.h"

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

class __declspec(dllexport) SkyUI {
public:
#ifdef GTA3
    static inline plugin::ThiscallEvent <plugin::AddressList<0x48E721, plugin::H_CALL, 0x48C8A4, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x47AB12, plugin::H_CALL, 0x47AB07, plugin::H_CALL, 0x47AAFE, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawStandardMenu;
    static inline plugin::CdeclEvent <plugin::AddressList<0x582E3C, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickNone, void(int32_t, int32_t)> onProcessGameState7;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x485183, plugin::H_CALL, 0x5816A5, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onLoadAllMenuTextures;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x47A1F3, plugin::H_CALL, 0x485682, plugin::H_CALL, 0x48ABA0, plugin::H_CALL, 0x48C71D, plugin::H_CALL,
        0x58163D, plugin::H_CALL, 0x582F62, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onUnloadMenuTextures;

#elif GTAVC
    static inline plugin::ThiscallEvent <plugin::AddressList<0x4A3299, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onDrawRedefinePage;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x4A4433, plugin::H_CALL, 0x4A5C88, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x4A325E, plugin::H_CALL, 0x4A32AD, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*, int)> onDrawStandardMenu;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x49902A, plugin::H_CALL, 0x4990A1, plugin::H_CALL, 0x601875, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onLoadAllMenuTextures;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x498FF5, plugin::H_CALL, 0x4A3E0E, plugin::H_CALL, 0x4A4821, plugin::H_CALL, 0x60050C, plugin::H_CALL,
        0x601781, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onUnloadMenuTextures;

#elif GTASA
    static inline plugin::ThiscallEvent <plugin::AddressList<0x57BA58, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*, uint8_t)> onDrawStandardMenu;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x53BF44, plugin::H_CALL, 0x53E7A5, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onProcess;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x576ED3, plugin::H_CALL, 0x576FA3, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onLoadAllMenuTextures;
    static inline plugin::ThiscallEvent <plugin::AddressList<0x53C5EE, plugin::H_CALL, 0x576D0E, plugin::H_CALL, 0x579465, plugin::H_CALL, 0x748DE0, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CMenuManager*, 0>, void(CMenuManager*)> onUnloadMenuTextures;
#endif

    static inline plugin::Timer timer = {};

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

#define CONSOLE_MENU_SCALE_X (0.5f)
#define CONSOLE_MENU_SCALE_Y (1.0f)
#if defined(GTA3) && defined(LCSFICATION)
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

    inline enum {
        MENUPAGE_CONTROLLER_PS2 = MENUPAGE_OPTIONS,
    };

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

#define TABS_SPACING (20.0f)

    inline enum {
#if defined GTA3 && !defined(LCSFICATION)
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
#elif defined(GTAVC) || (defined(GTA3) && defined(LCSFICATION))
        TAB_MAP,
        TAB_BRI,
        TAB_SAV,
        TAB_STA,
        TAB_CON,
        TAB_AUD,
        TAB_DIS,
        TAB_PS,
#elif defined(GTASA)
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

    inline enum {
#ifdef GTASA
        FIRST_MENUACTION = 102,
#else
        FIRST_MENUACTION = 127,
#endif
        MENUACTION_CONTROLLER_CONFIG = FIRST_MENUACTION,
        MENUACTION_CONTROLLER_DISPLAY,
        MENUACTION_CONTROLLER_VIBRATION,
        MENUACTION_SWITCH_DISPLAY_ADVANCED,
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
    static inline bool justEnteredTab = false;
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
    static inline bool ginput = false;
    static inline uint16_t *gInputSavedMode = nullptr;

#ifdef GTASA
#define PAD_IV_CONTROLS_MODE (1)
#else
#define PAD_IV_CONTROLS_MODE (4)
#endif

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
    static inline CSprite2d backgroundSprite = {};
    static inline CSprite2d skinSelSprite = {};

    static inline bool playerSkinInitialised = false;
    static inline float previousFOV = 42.0f;
#endif

    static inline bool modLoader = false;

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
#endif

    static inline plugin::TextLoader textLoader = {};

    static inline plugin::SpriteLoader frontendSprites = {};
#ifdef GTASA
    static inline plugin::SpriteLoader hudSprites = {};
#endif

    struct tMenuTab {
        char str[8];
        float x, y;
        uint8_t targetPage;
    };

    static inline std::vector<tMenuTab> tabs = {
#if defined(GTA3) && !defined(LCSFICATION)
#ifdef GTA3_MENU_MAP
#define MAP_TAB_OFFSET_X + 66.0f
        { "FEH_MAP", 26.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_MAP },
#else
#define MAP_TAB_OFFSET_X + 0.0f
#endif
        { "FEH_STA", 92.0f  MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_STATS },
        { "FEH_LOA", 158.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_NEW_GAME },
        { "FEH_BRI", 224.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_BRIEFS },
        { "FEH_CON", 290.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_CONTROLLER_PC },
        { "FEH_AUD", 356.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEH_DIS", 422.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_DISPLAY_SETTINGS },
        { "FEH_LAN", 488.0f MAP_TAB_OFFSET_X, DEFAULT_SCREEN_HEIGHT - 40.0f, MENUPAGE_LANGUAGE_SETTINGS },

#elif defined(GTAVC) || (defined(GTA3) && defined(LCSFICATION))
        { "FEH_MAP", 0.0f, 0.0f, MENUPAGE_MAP },
        { "FEH_BRI", 0.0f, 0.0f, MENUPAGE_BRIEFS },
        { "FEH_LOA", 0.0f, 0.0f, MENUPAGE_NEW_GAME },
        { "FEH_STA", 0.0f, 0.0f, MENUPAGE_STATS },
        { "FEH_CON", 0.0f, 0.0f, MENUPAGE_CONTROLLER_PC },
        { "FEH_AUD", 0.0f, 0.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEH_DIS", 0.0f, 0.0f, MENUPAGE_DISPLAY_SETTINGS },
        { "FEH_PS",  0.0f, 0.0f, MENUPAGE_SKIN_SELECT },
#elif GTASA
        { "FEH_LOA", 0.0f, 0.0f, MENUPAGE_NEW_GAME },
        { "FEH_BRI", 0.0f, 0.0f, MENUPAGE_BRIEFS },
        { "FEH_MAP", 0.0f, 0.0f, MENUPAGE_MAP },
        { "FEH_STA", 0.0f, 0.0f, MENUPAGE_STATS },
        { "FEH_CON", 0.0f, 0.0f, MENUPAGE_CONTROLLER_PC },
        { "FEH_AUD", 0.0f, 0.0f, MENUPAGE_SOUND_SETTINGS },
        { "FEH_GAL", 0.0f, 0.0f, MENUPAGE_GALLERY },
        { "FEH_DIS", 0.0f, 0.0f, MENUPAGE_DISPLAY_SETTINGS },
#endif
    };

    static inline bool GetWheelUp() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewMouseControllerState.wheelUp && !pad->OldMouseControllerState.wheelUp);
    }

    static inline bool GetWheelDown() {
        CPad* pad = CPad::GetPad(0);
        return (pad->NewMouseControllerState.wheelDown && !pad->OldMouseControllerState.wheelDown);
    }

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

    static inline bool GetLMBPressed() {
        CPad* pad = CPad::GetPad(0);
        return pad->NewMouseControllerState.lmb;
    }

    static inline bool GetLMB() {
        CPad* pad = CPad::GetPad(0);
#if defined(GTASA) || defined(GTAVC)
        return !pad->NewMouseControllerState.lmb && pad->OldMouseControllerState.lmb;
#else
        return pad->NewMouseControllerState.lmb && !pad->OldMouseControllerState.lmb;
#endif
    }

    static inline bool GetEsc() {
        CPad* pad = CPad::GetPad(0);
        bool back = (!pad->NewState.ButtonTriangle && pad->OldState.ButtonTriangle);

        if (prefsConfigSetup == PAD_IV_CONTROLS_MODE) {
            back = (!pad->NewState.ButtonCircle && pad->OldState.ButtonCircle);
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

            if (GetLMBPressed())
                result = 3;

            if (GetLMB())
                result = 2;
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

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
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

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
        if (page == MENUPAGE_SKIN_SELECT && !playerSkinInitialised) {
            auto playa = FindPlayerPed();

            if (!gpPlayerClump && playa) {
                CBaseModelInfo* mi = CModelInfo::GetModelInfo("player", 0);
                if (mi) {
                    RpClump* clump = (RpClump*)mi->CreateInstance();
                    if (clump) {
                        CWorld::Players[0].LoadPlayerSkin();

                        previousFOV = CDraw::ms_fFOV;
                        CDraw::ms_fFOV = 40.0f;

                        RpAnimBlendClumpInit(clump);

                        CAnimBlendAssociation* animAssoc = CAnimManager::AddAnimation(clump, playa->m_nAnimGroupId, 3);
                        gpPlayerClump = clump;
                    }
                }
            }

            _this->m_bSkinsEnumerated = false;
            playerRotation = -25.0f;
            playerRotationLerp = -25.0f;
            playerZoom = 1.0f;
            playerZoomLerp = 1.0f;
            playerSkinInitialised = true;
        }

        if (page != MENUPAGE_SKIN_SELECT && playerSkinInitialised) {
            if (gpPlayerClump) {
                RpClumpDestroy(gpPlayerClump);
                gpPlayerClump = nullptr;
            }
            CDraw::ms_fFOV = previousFOV;

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

#ifdef GTAVC
        RestoreAllScreenHacks(_this, true);
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
        //ClearInput();

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
        justEnteredTab = true;
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
        justEnteredTab = false;
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
            else if (currentTab >= TAB_CON && currentTab <= TAB_PS) {
                firstTab = TAB_CON;
                lastTab = TAB_PS;
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
                case 7:
                    tab = 3;
                    break;
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
            if (hover == 2) {
                EnterTab(_this, currentTab, true);
            }
        }
    }

    static inline uint8_t GetCheckHoverForStandardInput(CMenuManager* _this) {
        if (justEnteredTab)
            return false;

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

#ifdef GTAVC
            updateItemPoly = true;
#endif
            justEnteredTab = false;
        }

        if (_this->m_nCurrentMenuPage != GetTargetPage() 
#ifdef GTAVC
            || IsCurrentScreenCustom(_this)
#endif
            ) {
#ifdef GTAVC
            if (GetEsc() && IsCurrentScreenCustom(_this))
                RestoreAllScreenHacks(_this, true);
#endif
        }
        else {
            if (GetEsc()) {
                EscTab(_this);
            }
            else if (GetEnter() || (GetCheckHoverForStandardInput(_this) == 2 && !strcmp(aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_EntryName, "FEDS_TB"))) {
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

    static inline void Process(CMenuManager* _this) {
        justEnteredTab = false;
        ClearHelpText();

        bool classicControls = prefsConfigSetup != PAD_IV_CONTROLS_MODE;

#ifdef GTA3
        switch (currentTab) {
        default:
            SetHelpText(0, "FEDS_SE");

            if (currentInput == INPUT_TAB && _this->m_bGameNotLoaded) {

            }
            else {
                SetHelpText(1, classicControls ? "FEDS_BA" : "FEDSBAC");

#ifndef LCSFICATION
                if (HasPadInHands() && !_this->m_bGameNotLoaded)
                    SetHelpText(2, "FEDS_ST");
#endif
            }

            if (currentInput == INPUT_TAB)
                SetHelpText(4, "FEDS_AM");
            else
                SetHelpText(4, "FEDSAS4");
            break;
        case TAB_STA:
            SetHelpText(0, "FEDS_ST");
            SetHelpText(1, "FEDS_AM");

            SetHelpText(4, "FEDSSC1");
            SetHelpText(5, "FEDSSC2");
            break;
        case TAB_BRI:
            SetHelpText(0, "FEDS_ST");
            SetHelpText(4, "FEDS_AM");
            break;
        case TAB_MAP:
            if (currentInput == INPUT_STANDARD) {
                SetHelpText(0, classicControls ? "FEDS_BA" : "FEDSBAC");

#ifdef LCSFICATION
                SetHelpText(2, "FE_HLPA");
                SetHelpText(3, "FE_HLPB");
                SetHelpText(4, "FE_HLPC");
#else
                SetHelpText(1, "FE_HLPG");
                SetHelpText(3, "FE_HLPA");
                SetHelpText(4, "FE_HLPB");
                SetHelpText(5, "FE_HLPC");
#endif

            }
            else {
                SetHelpText(0, "FEDS_SE");
                SetHelpText(1, classicControls ? "FEDS_BA" : "FEDSBAC");
            }
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
        SetHelpText(1, classicControls ? "FEI_BTT" : "FEI_BTC");
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

        if (currentInput == INPUT_TAB && _this->m_bGameNotLoaded) {
        }
        else
            SetHelpText(1, classicControls ? "FE_HLPE" : "FE_HLPW");

        // Right
        SetHelpText(4, "FE_HLPG");

        switch (_this->m_nCurrentMenuPage) {
        case MENUPAGE_MAP:
            if (currentInput == INPUT_STANDARD) {
                if (_this->n_nMenuSystemPanelId == -99) {
                    SetHelpText(0, "FE_HLPA");
                    SetHelpText(2, "FE_HLPB");
                    SetHelpText(3, classicControls ? "FE_HLPC" : "FE_HLPS");

                    SetHelpText(4, "FE_HLPO");
                    SetHelpText(5, "FE_HLPT");
                }
                else {
                    SetHelpText(4, "FE_HLPI");
                    SetHelpText(5, "FE_HLPT");
                }
            }
            break;
        case MENUPAGE_GALLERY:
            if (currentInput == INPUT_STANDARD) {
                SetHelpText(0, "FE_HLPM");
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

        if (currentInput == INPUT_STANDARD) {
            bool enter = GetEnter() || (GetLMB() && _this->m_nHoverOption != 44);
            int32_t arrows = GetLeft() || GetWheelDown() ? -1 : GetRight() || GetWheelUp() ? 1 : 0;
            bool back = nullptr;
            if (enter || arrows != 0) {
                uint8_t res = ProcessMenuOptions(_this, arrows, &back, enter);
                if (res && !enter) {
#if defined(GTA3) || defined(GTAVC)
                    DMAudio.PlayFrontEndSound(FE_SOUND_ENTER, 0);
#elif GTASA
                    AudioEngine.ReportFrontendAudioEvent(FE_SOUND_ENTER, 0.0f, 1.0f);
#endif
                }
            }
        }
        if (timeToVibrate < CTimer::m_snTimeInMillisecondsPauseMode) {
            timeToVibrate = 0;
            CPad::GetPad(0)->StopShaking(0);
        }
    }

    static inline const plugin::char_t* ProcessMenuOptionsStrings(CMenuManager* _this, int32_t i) {
        const plugin::char_t* str = nullptr;
        int32_t action = aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nAction;
        switch (action) {
            case MENUACTION_CONTROLLER_CONFIG:
#ifdef GTASA
                str = plugin::FormatStatic("%s %d", textLoader.Get("FEC_SET"), prefsConfigSetup + 1);
#else
                str = plugin::FormatStatic(L"%ls %d", textLoader.Get("FEC_SET"), prefsConfigSetup + 1);
#endif
                break;
            case MENUACTION_CONTROLLER_DISPLAY:
                str = prefsDisplayOnFootInCar ? textLoader.Get("FEC_INC") : textLoader.Get("FEC_ONF");
                break;
            case MENUACTION_CONTROLLER_VIBRATION:
                str = prefsVibration ? textLoader.Get("FEM_ON") : textLoader.Get("FEM_OFF");
                break;
        }

        return str;
    }

#ifdef GTAVC
    enum eCustomMenuScreen {
        MENUPAGE_DISPLAY_ADVANCED,
    };

    static inline CMenuScreen aNewScreens[] = {
        // MENUPAGE_DISPLAY_ADVANCED
        { "FEH_DIS", MENUPAGE_DISPLAY_SETTINGS, 9,
            MENUACTION_SCREENRES, "FED_RES", 0, MENUPAGE_DISPLAY_SETTINGS, 84, 103, 1,

            MENUACTION_SWITCH_DISPLAY_ADVANCED, "FEH_FIR", 0, MENUPAGE_DISPLAY_SETTINGS, 320, 160, 0,
        },
    };

    static inline std::unordered_map<uint32_t, CMenuScreen> previousScreens = {};
    static inline void SwitchCurrentScreenHack(CMenuManager* _this, uint32_t curr, uint32_t dest) {
        auto& currScreen = aScreens[curr];
        auto& destScreen = aNewScreens[dest];

        auto& prevScreen = previousScreens[curr];
        prevScreen = currScreen;
        currScreen = destScreen;

        SwitchMenuPage(_this, curr, true);
    }

    //static inline void RestoreScreenHack(CMenuManager* _this, uint32_t screen) {
        //auto& prevScreen = previousScreens[screen];
        //auto& currScreen = aScreens[prevScreen.page];
        //
        //currScreen = prevScreen.screen;
        //previousScreens.erase(screen);
        //
        //SwitchMenuPage(_this, screen, true);
    //}

    static inline void RestoreAllScreenHacks(CMenuManager* _this, bool switchPage) {
        int32_t entry = 0;
        for (auto& it : previousScreens) {
            auto& prevScreen = it.second;
            auto& currScreen = aScreens[it.first];

            if (_this->m_nCurrentMenuPage == it.first)
                entry = it.second.m_nParentEntry;

            currScreen = prevScreen;
        }
        previousScreens = {};

        if (switchPage) {
            SwitchMenuPage(_this, _this->m_nCurrentMenuPage, true);
            _this->m_nCurrentMenuEntry = entry;
        }
    }

    static inline bool IsCurrentScreenCustom(CMenuManager* _this) {
        auto& prev = previousScreens.find(_this->m_nCurrentMenuPage);
        if (prev != previousScreens.end())
            return true;

        return false;
    }

#endif

    static inline uint8_t ProcessMenuOptions(CMenuManager* _this, int8_t arrows, bool* back, bool enter) {
        int32_t action = aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_nAction;
        switch (action) {
#ifdef GTASA
            case MENUACTION_DELETEGALLERYPHOTO:
                if (enter) {
                    ProcessGalleryDeletePic(_this);
                    _this->SwitchToNewScreen(MENUPAGE_GALLERY);
                }
                return 1;
#endif
            case MENUACTION_CONTROLLER_CONFIG:
#ifdef GTASA
                prefsConfigSetup ^= 1;
#else
                if (arrows < 0)
                    prefsConfigSetup--;
                else
                    prefsConfigSetup++;

                if (prefsConfigSetup > 4)
                    prefsConfigSetup = 0;
                if (prefsConfigSetup < 0)
                    prefsConfigSetup = 4;
#endif
                configLayout = 0;
                SetGInputStuff();
                SetFocus();
                SaveSettings();
                return 1;
            case MENUACTION_CONTROLLER_DISPLAY:
                prefsDisplayOnFootInCar ^= true;
                return 1;
            case MENUACTION_CONTROLLER_VIBRATION:
                prefsVibration ^= true;
                if (prefsVibration) {
                    CPad::GetPad(0)->StopShaking(0);
                    CPad::GetPad(0)->StartShake(300, 150);
                    timeToVibrate = CTimer::m_snTimeInMillisecondsPauseMode + 300;
                }

                SetGInputStuff();
                SetFocus();
                SaveSettings();
                return 1;
#ifdef GTAVC
            case MENUACTION_SWITCH_DISPLAY_ADVANCED:
                if (IsCurrentScreenCustom(_this))
                    RestoreAllScreenHacks(_this, true);
                else
                    SwitchCurrentScreenHack(_this, MENUPAGE_DISPLAY_SETTINGS, MENUPAGE_DISPLAY_ADVANCED);
                return 1;
#endif
        }

        return 0;
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

            if (selected && CheckHover(_this, CRect(_x - ScaleX(2.0f), _y - ScaleY(16.0f + 4), _x + _w + ScaleX(2.0f), _y + ScaleY(4.0f))) == 3) {
                const int max_steps = 16 + 1;
                int32_t steps = 0;
                bool lower = current < progress;
                if (lower) {
                    float difference = progress - current;
                    steps = static_cast<int32_t>(difference * max_steps);
                }
                else {
                    float difference = current - progress;
                    steps = static_cast<int32_t>(difference * max_steps);
                }

                int8_t delta = (lower) ? -1 : 1;
                for (int32_t step = 0; step < steps; ++step) {
                    _this->CheckSliderMovement(delta);
                }
            }

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
        helpTexts = {};
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
        
        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS) {
            CFont::SetScale(ScaleX(0.8f), ScaleY(1.2f));
            CFont::PrintString(SCREEN_WIDTH - ScaleX(44.0f + GetMenuOffsetX()), ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f), UpperCase(textLoader.Get(tabs.at(currentTab).str)));
        }
        else {
            CFont::SetScale(ScaleX(0.8f), ScaleY(1.2f));
            CFont::PrintString(ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 44.0f + GetMenuOffsetX()), ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f), UpperCase(textLoader.Get(tabs.at(currentTab).str)));
        }
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
        CFont::SetJustifyOff();
#elif GTASA
        CFont::SetProportional(true);
        CFont::SetBackground(false, false);
        CFont::SetOrientation(ALIGN_LEFT);
#endif
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetRightJustifyWrap(0.0f);
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

        float startx = ScaleX(40.0f + GetMenuOffsetX());
        float starty = ScaleY(DEFAULT_SCREEN_HEIGHT - 74.0f);
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
                CFont::PrintString(x, y, textLoader.Get(it.c_str()));
            }

            y += ScaleY(spacing);

#if defined(GTA3) && !defined(LCSFICATION) 
            if (i == 2) {
                x += ScaleX(198.0f);
                y = starty;
            }
#elif defined(GTA3) && defined(LCSFICATION) 
            if (i == 1) {
                CFont::SetCentreOff();
                CFont::SetRightJustifyOff();
                x = (SCREEN_WIDTH / 2) - ScaleX(40.0f);
                y = starty;
            }

            if (i == 3) {
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

    static inline bool prefsDisplayOnFootInCar = false;
    static inline int8_t prefsConfigSetup = 0;
    static inline bool prefsVibration = false;
    static inline int32_t timeToVibrate = 0;
    static inline uint8_t configLayout = 0;

    static inline void DrawControllerScreen(CMenuManager* _this) {
        CRGBA col = { 255, 255, 255, (uint8_t)GetAlpha(255) };

#ifdef GTASA
        float x = ScaleXKeepCentered(320.0f);
        float y = ScaleY(298.0f);
        float halfw = ScaleX(256.0f / 2.0f);
        float halfh = ScaleY(276.0f / 2.0f);
#else
        float x = ScaleXKeepCentered(288.0f + GetMenuOffsetX());
        float y = ScaleY(188.0f);
        float halfw = ScaleX(236.0f / 2.0f);
        float halfh = ScaleY(188.0f / 2.0f);
#endif
#ifdef GTAVC
        y += ScaleY(90.0f);
#endif

        static std::string sprite = "fe_arrows1";
#ifndef GTASA
        frontendSprites.GetSprite("fe_controller")->Draw(CRect(x - halfw, y - halfh, x + halfw, y + halfh), col);
        frontendSprites.GetSprite(sprite)->Draw(CRect(x - halfw, y - halfh, x + halfw, y + halfh), col);
#else
        frontendSprites.GetSprite("CONTROLLER_PS2")->Draw(CRect(x - halfw, y - halfh, x + halfw, y + halfh), col);
#endif
        static bool switched = false;
        if (currentInput == INPUT_STANDARD && CTimer::m_snTimeInMillisecondsPauseMode & 1024) {
            if (!switched) {
                configLayout++;
                switched = true;
            }
        }
        else {
            switched = false;
        }

        if (configLayout > 2)
            configLayout = 0;

        CFont::SetProportional(true);
        CFont::SetFontStyle(FONT_SUBTITLES);
        CFont::SetScale(ScaleX(0.4f), ScaleY(0.58f));
        CFont::SetDropColor(CRGBA(0, 0, 0, GetAlpha(255)));

#ifdef GTAVC
        CFont::SetDropShadowPosition(0);
        CFont::SetColor(CRGBA(0, 0, 0, GetAlpha(255)));

        CFont::SetWrapx(ScaleX(640.0f + 60.0f));
        CFont::SetRightJustifyWrap(ScaleX(160.0f));
#elif GTASA
        CFont::SetEdge(1);
        CFont::SetDropShadowPosition(0);
        CFont::SetColor(CRGBA(150, 150, 150, GetAlpha(255)));
        CFont::SetScale(ScaleX(0.4f), ScaleY(0.74f));

        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::SetRightJustifyWrap(0.0f);
#else
        CFont::SetDropShadowPosition(1);
        CFont::SetColor(CRGBA(225, 225, 225, GetAlpha(255)));

        CFont::SetWrapx(ScaleX(640.0f + 60.0f));
        CFont::SetRightJustifyWrap(ScaleX(160.0f));
#endif


#ifdef GTASA
        switch (prefsConfigSetup) {
            case 0:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-92.0f), textLoader.Get(configLayout ? "FEC_LL" : "FEC_RL"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_CAW"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_TSK"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_RSC"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-4.0f), textLoader.Get("FEC_RSP"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(22.0f), textLoader.Get(configLayout ? "FEC_HOR" : "FEC_HOV"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_CAM"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-92.0f), textLoader.Get(configLayout ? "FEC_LL" : "FEC_RR"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_HAB"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-54.0f), textLoader.Get(configLayout ? "FEC_BRA" : "FEC_REV"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-22.0f), textLoader.Get(configLayout ? "FEC_FWE" : "FEC_ALW"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-4.0f), textLoader.Get(configLayout == 2 ? "FEC_THR" : configLayout == 1 ? "FEC_HCA" : "FEC_ACC"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(10.0f), textLoader.Get(configLayout ? "FEC_TUC": "FEC_MCA"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(22.0f), textLoader.Get(configLayout ? "FEC_SM3" : "FEC_GER"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-92.0f), textLoader.Get(configLayout ? "FEC_CWL" : "FEC_ZSO"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_FWT"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_TKP"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_GAM"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_TKN"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-4.0f), textLoader.Get("FEC_GPM"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(10.0f), textLoader.Get(configLayout ? "FEC_MOV" : "FEC_STR"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(22.0f), textLoader.Get(configLayout ? "FEC_CR3" : "FEC_DK3"));
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_CAM"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-92.0f), textLoader.Get(configLayout ? "FEC_CWR" : "FEC_ZSI"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_TAR"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_JMP"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-38.0f), textLoader.Get(configLayout == 2 ? "FEC_ENV" : configLayout == 1 ? "FEC_PKU" : "FEC_USE"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-22.0f), textLoader.Get(configLayout == 2 ? "FEC_DUW" : configLayout == 1 ? "FEC_ATT" : "FEC_FWE"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-4.0f), textLoader.Get(configLayout ? "FEC_SWF" : "FEC_RN2"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(10.0f), textLoader.Get(configLayout ? "FEC_MCA" : "FEC_MSC"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(22.0f), textLoader.Get("FEC_LB4"));
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                }
                break;
            case 1:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-92.0f), textLoader.Get(configLayout ? "FEC_BRA" : "FEC_REV"));  // l2
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_LL")); // l1
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_TSK")); // dpad right
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_RSC")); // up
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_NA"));  // left 
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-4.0f), textLoader.Get("FEC_RSP"));  // down
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));  // stick
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(22.0f), textLoader.Get("FEC_HOV"));  // l3
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(44.0f), textLoader.Get(configLayout ? "FEC_TRP" : "FEC_CAM"));   // select

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-92.0f), textLoader.Get("FEC_ACC"));   // r2
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_LR"));  // r1
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_NA"));  // square
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_EXV"));  // triangle
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_ALW"));  // circle
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-4.0f), textLoader.Get("FEC_HCA"));   // cr
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MCA"));   // stick
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(22.0f), textLoader.Get("FEC_GER"));   // r3
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));   // start
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-92.0f), textLoader.Get("FEC_TAR"));  // l2
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_USE"));  // l1
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_CWR"));  // dpad right
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_NA"));  // up
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_CWL"));  // left 
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(-4.0f), textLoader.Get("FEC_NA"));   // down
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));   // stick
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(22.0f), textLoader.Get("FEC_CR3"));   // l3
                    CFont::PrintString(x + ScaleX(-138.0f), y + ScaleY(44.0f), textLoader.Get(configLayout ? "FEC_TRP" : "FEC_CAM"));   // select

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-92.0f), textLoader.Get("FEC_FWE"));   // r2
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-80.0f), textLoader.Get("FEC_NA"));   // r1
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-54.0f), textLoader.Get("FEC_JMP"));   // square
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-38.0f), textLoader.Get("FEC_ENV"));   // triangle
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-22.0f), textLoader.Get(configLayout == 2 ? "FEC_DUW" : configLayout == 1 ? "FEC_ATT" : "FEC_FWE")); // circle
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(-4.0f), textLoader.Get(configLayout ? "FEC_SWF" : "FEC_RN2")); // cross
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MCA"));    // stick
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(22.0f), textLoader.Get("FEC_LB4"));    // r3
                    CFont::PrintString(x + ScaleX(138.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));    // start
                }
                break;
        }
#else
        switch (prefsConfigSetup) {
            case 0:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOL"));

                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-60.0f), textLoader.Get("FEC_RSC"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_VES"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(0.0f), y + ScaleY(-100.0f), textLoader.Get("FEC_LB"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-32.0f), y + ScaleY(80.0f), textLoader.Get("FEC_HO3"));

                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_HND"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(40.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_BRA"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_CAW"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_ACC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_TUC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_SMT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows2";
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWL"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_LOF"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_MOV"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_TAR"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_JUM"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_ENV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_ATT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_RUN"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_FPC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_LB3"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows1";
                }
                break;
            case 1:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOL"));

                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-60.0f), textLoader.Get("FEC_HRN")); 
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(0.0f), y + ScaleY(-100.0f), textLoader.Get("FEC_LB"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-32.0f), y + ScaleY(80.0f), textLoader.Get("FEC_NA"));

                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_RSC"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_HND"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(40.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_BRA"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_CAW"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_ACC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_TUC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_SMT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows4";
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWL"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_LOF"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_TAR"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_JUM"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_ENV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_ATT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_RUN"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_FPC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_LB3"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows3";
                }
                break;
            case 2:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOL"));

                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-60.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_VES"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(0.0f), y + ScaleY(-100.0f), textLoader.Get("FEC_LB"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-32.0f), y + ScaleY(80.0f), textLoader.Get("FEC_RS3"));

                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_HRN"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(40.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_BRA"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_HND"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_CAW"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_ACC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_TUC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_SMT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows2";
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWL"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_ENV"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_MOV"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_TAR"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_JUM"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_LOF"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_RUN"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_ATT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_FPC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_LB3"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows1";
                }
                break;
            case 3:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOL"));

                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-60.0f), textLoader.Get("FEC_HAB"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_TUC"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(0.0f), y + ScaleY(-100.0f), textLoader.Get("FEC_LB"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-32.0f), y + ScaleY(80.0f), textLoader.Get("FEC_HO3"));

                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_LOR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_CAW"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(40.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_SMT"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_RSC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_ACC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_BRA"));

                    sprite = "fe_arrows2";
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWL"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_TAR"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CAM"));
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_CWR"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_ATT"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_JUM"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_ENV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_LOF"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_RUN"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_FPC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_LB3"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows1";
                }
                break;
            case 4:
                if (prefsDisplayOnFootInCar) {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_BRA"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-60.0f), textLoader.Get("FEC_CAW"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-30.0f), textLoader.Get("FEC_RS"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_VES"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CRE"));
                    CFont::PrintString(x + ScaleX(0.0f), y + ScaleY(-100.0f), textLoader.Get("FEC_NA"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(-32.0f), y + ScaleY(80.0f), textLoader.Get("FEC_HO3"));

                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_ACC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_LOC"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_NA"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_EXV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_HND"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_TUC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_SMT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows2";
                }
                else {
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(-68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_TAR"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_NA"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(-24.0f), textLoader.Get("FEC_CWE"));
                    CFont::PrintString(x + ScaleX(-122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_MOV"));

                    CFont::SetOrientation(ALIGN_CENTER);
                    CFont::PrintString(x + ScaleX(-14.0f), y + ScaleY(58.0f), textLoader.Get("FEC_CRE"));

                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(14.0f), y + ScaleY(44.0f), textLoader.Get("FEC_PAU"));
                    CFont::PrintString(x + ScaleX(68.0f), y + ScaleY(-108.0f), textLoader.Get("FEC_ATT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-64.0f), textLoader.Get("FEC_CEN"));
                    CFont::SetOrientation(ALIGN_RIGHT);
                    CFont::PrintString(x + ScaleX(26.0f), y + ScaleY(-74.0f), textLoader.Get("FEC_JUM"));
                    CFont::SetOrientation(ALIGN_LEFT);
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-34.0f), textLoader.Get("FEC_ENV"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-22.0f), textLoader.Get("FEC_ATT"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(-8.0f), textLoader.Get("FEC_RUN"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(10.0f), textLoader.Get("FEC_FPC"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(20.0f), textLoader.Get("FEC_LB3"));
                    CFont::PrintString(x + ScaleX(122.0f), y + ScaleY(30.0f), textLoader.Get("FEC_R3"));

                    sprite = "fe_arrows1";
                }
                break;
        }
#endif
    }

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
    
#endif

#if defined(GTA3)
        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS) {
            _this->DrawControllerSetupScreen();
            return;
        }
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

        _this->m_aFrontEndSprites[sprite].Draw(rect, CRGBA(0, 0, 0, GetAlpha(_this->m_nMenuFadeAlpha)));

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

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
        if (_this->m_nCurrentMenuPage == MENUPAGE_SKIN_SELECT)
            DrawPlayerSetupScreen(_this);
#endif

#ifdef GTAVC
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

        if (currentInput == INPUT_TAB || aScreens[_this->m_nCurrentMenuPage].m_aEntries[_this->m_nCurrentMenuEntry].m_EntryName[0] != '\0')
            DrawTrapezoid();
#endif

        // Sliders 
        for (int i = 0; i < NUM_ENTRIES; i++) {
            auto action = aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nAction;

#ifdef GTA3
            float x = ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 172.0f + GetMenuOffsetX());
#ifdef LCSFICATION
            x = SCREEN_WIDTH - ScaleX(278.0f + GetMenuOffsetX());
#endif

            switch (action) {
            case MENUACTION_BRIGHTNESS:
                DrawSlider(_this, x, ScaleY(92.0f), _this->m_nPrefsBrightness / 512.0f, action);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(_this, x, ScaleY(92.0f + 22.0f), (_this->m_fPrefsLOD - 0.8f) * 1.0f, action);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(_this, x, ScaleY(92.0f), _this->m_nPrefsMusicVolume / 128.0f, action);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(_this, x, ScaleY(92.0f + 22.0f), _this->m_nPrefsSfxVolume / 128.0f,  action);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(_this, x, ScaleY(92.0f), TheCamera.m_fMouseAccelHorzntal * 200.0f, action);
                break;
            }
#elif defined(GTAVC)
            float y = (aScreens[_this->m_nCurrentMenuPage].m_aEntries[i].m_nY * DEFAULT_SCREEN_HEIGHT / 448.0f) + 16.0f;

            switch (action) {
            case MENUACTION_BRIGHTNESS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsBrightness / 384.0f, action);
                break;
            case MENUACTION_DRAWDIST:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), (_this->m_fPrefsLOD - 0.925f) / 0.875f, action);
                break;
            case MENUACTION_MUSICVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsMusicVolume / 64.0f, action);
                break;
            case MENUACTION_SFXVOLUME:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsSfxVolume / 64.0f, action);
                break;
            case MENUACTION_MOUSESENS:
                DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), TheCamera.m_fMouseAccelHorzntal * 200.0f, action);
                break;

#ifdef GTAVC
            case MENUACTION_MP3VOLUMEBOOST:
                if (_this->m_nPrefsAudio3DProviderIndex != -3 && DMAudio.IsMP3RadioChannelAvailable())
                    DrawSlider(_this, ScaleXKeepCentered(DEFAULT_SCREEN_WIDTH - 196.0f + GetMenuOffsetX()), ScaleY(y), _this->m_nPrefsMP3BoostVolume / 64.0f, action);
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
#if (defined(GTA3) && defined(LCSFICATION))
        if (tab == TAB_PS && _this->m_bGameNotLoaded)
            return false;

        if (tab == TAB_PS && !_this->m_bGameNotLoaded) {
            auto playa = FindPlayerPed();

            if (playa) {
                CBaseModelInfo* mi = CModelInfo::GetModelInfo(playa->m_nModelIndex);
                if (strcmp(mi->m_szName, "player"))
                    return false;
            }
        }
#elif  defined(GTAVC)
        if (tab == TAB_PS)
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
        float spacing = ScaleX(TABS_SPACING);
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

        x += ScaleX(GetMenuOffsetX());

//#if defined(GTA3) && defined(GTA3_MENU_MAP)
//        if (menuMap) {
//            if (i == TAB_MAP) {
//                wchar_t* strBuff = LowerCase((wchar_t*)str);
//                strBuff[0] = GetUpperCase(str[0]);
//                str = strBuff;
//            }
//        }
//#endif

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

#if defined(GTA3) && defined(LCSFICATION)
        if (_this->m_nCurrentMenuPage == MENUPAGE_MAP && currentInput == INPUT_STANDARD)
            return;
#endif

        if (_this->m_nCurrentMenuPage == MENUPAGE_KEYBOARD_CONTROLS)
            return;

        if (_this->m_nCurrentMenuPage == MENUPAGE_CONTROLLER_PS2)
            DrawControllerScreen(_this);

        DrawHelpText(_this);

#ifdef GTASA
        if ((_this->m_nCurrentMenuPage == MENUPAGE_MAP || _this->m_nCurrentMenuPage == MENUPAGE_GALLERY) && currentInput == INPUT_STANDARD)
            return;
#endif

#if defined(GTA3) || defined(GTASA)
        DrawHeader(_this);
#endif

        if (saveMenuActive)
            return;

        CFont::SetAlphaFade(255);
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
        float spacing = ScaleX(TABS_SPACING);
        float y = ScaleY(410.0f);

        int32_t count = 0;
        for (int32_t i = 0; i < tabs.size(); i++) {
            if (!IsTabAvailable(_this, i))
                continue;

#if defined(GTA3) && !defined(LCSFICATION)
            currentX = ScaleXKeepCentered(tabs[count].x);
            y = ScaleY(tabs[count].y);
#else
            if (count == 4) { // second row
                totalWidth = GetTotalTabsWidth(_this, i);
                startX = (SCREEN_WIDTH - totalWidth) / 2;
                currentX = startX;
                y = ScaleY(432.0f);
            }
#endif

            const plugin::char_t* str = textLoader.Get(tabs.at(i).str);
            const float strWidth = CFont::GetStringWidth(str, false);
            DrawOneTab(_this, i, currentX, y, str);
#if !defined(GTA3) || defined(LCSFICATION)
            currentX += strWidth + spacing;
#endif
            count++;
        }
    }

    static inline void CheckForExternalScripts() {
        HMODULE h = nullptr;
#if defined(GTA3) && defined(GTA3_MENU_MAP)
        h = ModuleList().GetByPrefix(L"menumap");
        if (h) {
            menuMap = true;
        }
#elif defined(GTASA)
        h = ModuleList().GetByPrefix(L"modloader");
        if (h) {
            modLoader = true;
        }
#endif
    
        if (GInput_Load(&gInputPad)) {
            ginput = true;
            gInputPad->SendEvent(GINPUT_EVENT_REGISTER_SETTINGS_RELOAD_CALLBACK, SetGInputStuff);
        }
    }

    static inline void Init() {
#ifdef _DEBUG
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        std::setvbuf(stdout, NULL, _IONBF, 0);
#endif   
    }

    static inline void InitAfterRw() {
        if (initialised)
            return;

#ifdef GTASA
        aScreens = plugin::patch::Get<CMenuScreen*>(0x576B37 + 1);
#endif

        CheckForExternalScripts();

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

                bool menuPage = true;

#ifndef GTAVC
                menuPage = i != MENUPAGE_CHOOSE_LOAD_SLOT && i != MENUPAGE_CHOOSE_DELETE_SLOT;
#endif

                if ((!strcmp(aScreens[i].m_aEntries[j].m_EntryName, "FEDS_TB") && menuPage) ||
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
            aScreens[it.targetPage].m_nPreviousPage = it.targetPage;
            aScreens[it.targetPage].m_nPreviousGamePage = it.targetPage;
        }

        aScreens[MENUPAGE_EXIT].m_nPreviousPage = MENUPAGE_NEW_GAME;
        aScreens[MENUPAGE_EXIT].m_nPreviousGamePage = MENUPAGE_NEW_GAME;

        aScreens[MENUPAGE_EXIT].m_nParentEntry = 3;
        aScreens[MENUPAGE_EXIT].m_nParentGameEntry = 3;
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

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousGamePage = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry = 8;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentGameEntry = 8;

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
        strcpy(aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_EntryName, "FEH_LAN");
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAction = MENUACTION_CHANGEMENU;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nTargetMenu = MENUPAGE_LANGUAGE_SETTINGS;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nX = 320;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nY = 290;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAlign = 0;

        strcpy(aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_EntryName, "FEH_SEC");
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nAction = MENUACTION_SWITCH_DISPLAY_ADVANCED;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nTargetMenu = 0;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nX = 0;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nY = 0;
        aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[9].m_nAlign = 0;

        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nPreviousPage = MENUPAGE_DISPLAY_SETTINGS;
        aScreens[MENUPAGE_LANGUAGE_SETTINGS].m_nParentEntry = 8;
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

        strcpy(aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[2].m_EntryName, "FET_CTL");
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[2].m_nTargetMenu = MENUPAGE_CONTROLLER_PS2;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[2].m_nAction = MENUACTION_CHANGEMENU;

        strcpy(aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[3].m_EntryName, "FET_DEF");
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[3].m_nTargetMenu = MENUPAGE_NONE;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[3].m_nAction = MENUACTION_RESTOREDEF;

#ifndef GTA3
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[0].m_nX = 320;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[0].m_nY = 160;

        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[1].m_nX = 0;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[1].m_nY = 0;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[2].m_nX = 0;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[2].m_nY = 0;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[3].m_nX = 0;
        aScreens[MENUPAGE_CONTROLLER_PC].m_aEntries[3].m_nY = 0;
#endif

        aScreens[MENUPAGE_KEYBOARD_CONTROLS].m_nParentEntry = 0;
#ifdef GTA3
        aScreens[MENUPAGE_KEYBOARD_CONTROLS].m_nParentGameEntry = 0;
#endif
        aScreens[MENUPAGE_MOUSE_SETTINGS].m_nParentEntry = 1;

#ifdef GTA3
        aScreens[MENUPAGE_MOUSE_SETTINGS].m_nParentGameEntry = 1;
#endif

        aScreens[MENUPAGE_CONTROLLER_PS2] = {};
        aScreens[MENUPAGE_CONTROLLER_PS2].m_nParentEntry = 2;
#ifdef GTA3
        aScreens[MENUPAGE_CONTROLLER_PS2].m_nParentGameEntry = 2;
#endif

        aScreens[MENUPAGE_CONTROLLER_PS2].m_nPreviousPage = MENUPAGE_CONTROLLER_PC;

#ifdef GTA3
        aScreens[MENUPAGE_CONTROLLER_PS2].m_nPreviousGamePage = MENUPAGE_CONTROLLER_PC;
#endif

        strcpy(aScreens[MENUPAGE_CONTROLLER_PS2].m_ScreenName, "FET_CTL");
        strcpy(aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_EntryName, "FEC_CCF");
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nAction = MENUACTION_CONTROLLER_CONFIG;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nTargetMenu = MENUPAGE_NONE;

        strcpy(aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_EntryName, "FEC_CDP");
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_nAction = MENUACTION_CONTROLLER_DISPLAY;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_nTargetMenu = MENUPAGE_NONE;

        strcpy(aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_EntryName, "FEC_VIB");
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_nAction = MENUACTION_CONTROLLER_VIBRATION;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_nTargetMenu = MENUPAGE_NONE;

#if defined(GTAVC) || defined(GTASA)
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nX = 40;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nY = 76;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nAlign = 1;

        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_nX = 0;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_nY = 0;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[1].m_nAlign = 1;

        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_nX = 0;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_nY = 0;
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[2].m_nAlign = 1;

#ifdef GTAVC        
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nX += 42;
#else
        aScreens[MENUPAGE_CONTROLLER_PS2].m_aEntries[0].m_nY += 22;
#endif

#endif

#if defined(GTA3) && defined(GTA3_MENU_MAP)
        if (!menuMap) {
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_EntryName[0] = '\0';
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nAction = MENUACTION_NOTHING;
            aScreens[MENUPAGE_DISPLAY_SETTINGS].m_aEntries[8].m_nTargetMenu = MENUPAGE_NONE;
        }
#endif

#ifdef GTASA
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

        frontendSprites.Clear();
        frontendSprites.LoadAllSpritesFromFolder(PLUGIN_PATH("SkyUI\\frontend"));

#ifdef GTASA
        hudSprites.Clear();
        hudSprites.LoadAllSpritesFromFolder(PLUGIN_PATH("SkyUI\\hud"));
#endif

        // GInput patches
        const HMODULE hGInput = ModuleList().GetByPrefix(L"GInput");
        if (hGInput) {
#ifdef GTAVC
            // Fix buttons colors
            uint32_t addr = plugin::pattern::GetExternal(hGInput, "74 23 0F B6 01");
            if (addr)
                plugin::patch::SetUChar(addr, 0x75);

#endif

            gInputSavedMode = plugin::pattern::ReadExternal<uint16_t*>(hGInput, "66 89 04 75", 4);

            if (!gInputSavedMode)
                gInputSavedMode = plugin::pattern::ReadExternal<uint16_t*>(hGInput, "66 A3 ? ? ? ? 8D 44 24 18", 2);

            SetGInputStuff();
            SetFocus();
        }

        initialised = true;
    }

    static inline void SetGInputStuff() {
        if (gInputSavedMode) {
#ifdef GTASA
            if (prefsConfigSetup > 1)
                prefsConfigSetup = 1;
#endif
            gInputSavedMode[0] = prefsConfigSetup;
            gInputSavedMode[1] = prefsConfigSetup;

            //CPad::GetPad(0)->Mode = prefsConfigSetup;
            //CPad::GetPad(1)->Mode = prefsConfigSetup;

            FrontEndMenuManager.m_bPrefsUseVibration = prefsVibration;
        }
    }

    static inline void SetFocus() {
        HWND wnd = RsGlobal.ps->window;
        if (wnd)
            SendMessage(wnd, WM_SETFOCUS, 0, 0);
    }

    static inline void Shutdown() {
        if (!initialised)
            return;

#ifdef GTASA
        GalleryShutdown();
#endif

        frontendSprites.Clear();

#ifdef GTASA
        hudSprites.Clear();
#endif

        GInput_Release();
        gInputPad = nullptr;

        initialised = false;
    }

    static inline void UpdateText(CMenuManager* _this) {
        textLoader.Clear();

        std::string path = PLUGIN_PATH("SkyUI\\text\\");
        std::string lang = "american.txt";
        switch (_this->m_nPrefsLanguage) {
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

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
        currPlayerSkinPage = 0;
        currPlayerSkin = 0;
        playerSkinAnim = false;
#endif

        justEnteredTab = false;

#ifdef GTAVC
        RestoreAllScreenHacks(_this, true);
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

                    plugin::Image* img = nullptr;
                    plugin::CreateImageFromFile(buff, img);
                    galleryPhotos[numGalleryPhotos].texture = CreateRwTexture(img->width, img->height, img->pixels);
                    img->Release();
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

            strcpy(gString, textLoader.Get("FEG_HOW"));

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

#endif

#if (defined(GTA3) && defined(LCSFICATION)) || defined(GTAVC)
#ifdef LCSFICATION
    static inline void SetLCSFontStyle(bool white = false) {
        CFont::SetScale(ScaleX(CONSOLE_MENU_SCALE_X * 0.9f), ScaleY(CONSOLE_MENU_SCALE_Y * 0.9f));
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

    static inline std::string RemoveEx(std::string str) {
        size_t pos = str.find_last_of('.');
        if (pos != std::string::npos)
            str = str.substr(0, pos);
        return str;
    }

    static void ClearSkins() {
        playerSkins = {};
    }

    static inline void PushSkin(uint32_t id, const char* name, const char* displayName, const char* date) {
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

    static inline void ScanPlayerSkins() {
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

    static inline void ApplySkin(CMenuManager* _this, CPlayerSkinData* skin) {
        _this->m_pSelectedSkin = skin;
        strcpy(_this->m_nPrefsSkinFile, skin->m_aSkinNameDisplayed);
        CWorld::Players[0].SetPlayerSkin(_this->m_nPrefsSkinFile);
        _this->SaveSettings();
    }

#define DEFAULT_SKIN_NAME "$$\"\""
#define NUM_VISIBLE_ITEMS (14)

    static inline int32_t currPlayerSkinPage = 0;
    static inline int32_t currPlayerSkin = 0;

    static inline int32_t GetLastSkinOnCurrentPage() {
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

    static inline int32_t GetLastPage() {
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

    static inline RpAtomic* GetAnimHierarchyCallback(RpAtomic* atomic, void* data) {
        *(RpHAnimHierarchy**)data = RpSkinAtomicGetHAnimHierarchy(atomic);
        return nullptr;
    }

    static inline RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump) {
        RpHAnimHierarchy* hier = nullptr;
        RpClumpForAllAtomics(clump, GetAnimHierarchyCallback, &hier);
        return hier;
    }

    static inline RpClump* GetClump(const char* img, const char* dir, const char* dff) {
        bool removeImage = false;
        if (plugin::CallAndReturn<int32_t, 0x406370>() == 0) {
            plugin::CallAndReturn<uint8_t, 0x406270>(img);
            removeImage = true;
        }

        CDirectory::DirectoryInfo info = {};
        int32_t file = CFileMgr::OpenFile(dir, "rb");

        do {
            if (!CFileMgr::Read(file, (char*)&info, sizeof(CDirectory::DirectoryInfo)))
                break;
        } while (strcmp(dff, info.m_szName) != 0);

        uint8_t* buf = new uint8_t[(info.m_nSize << 11) + (2048 - 1)];
        plugin::Call<0x405E40>(0, buf, info.m_nOffset, info.m_nSize);
        plugin::Call<0x406010>(0);

        RwMemory mem = {};
        mem.start = buf;
        mem.length = info.m_nSize << 11;
        RwStream* stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &mem);

        RpClump* clump = nullptr;
        if (RwStreamFindChunk(stream, rwID_CLUMP, nullptr, nullptr))
            clump = RpClumpStreamRead(stream);

        RwStreamClose(stream, &mem);
        delete[] buf;

        if (removeImage)
            plugin::Call<0x406300>();

        return clump;
    }

    static inline float playerZoom = 1.0f;
    static inline float playerZoomLerp = 1.0f;

    static inline RpAtomic* IsSkinnedCb(RpAtomic* atomic, void* data) {
        RpAtomic** ret = (RpAtomic**)data;
        if (*ret)
            return NULL;
        assert(atomic->geometry->object.type = rpGEOMETRY);
        if (RpSkinGeometryGetSkin(atomic->geometry))
            *ret = atomic;
        return atomic;
    }

    static inline bool IsClumpSkinned(RpClump* clump) {
        RpAtomic* atomic = nullptr;
        RpClumpForAllAtomics(clump, IsSkinnedCb, &atomic);
        return atomic != nullptr;
    }

    static inline void RenderClump(RpClump* clump) {
        RwV3d pos = { -1.15f, -0.005f, 8.0f };

        pos.x *= playerZoomLerp;
        pos.y -= 1.0f - max(playerZoomLerp, 0.445f);
        pos.z *= playerZoomLerp;

        const RwV3d axis1 = { 1.0f, 0.0f, 0.0f };
        const RwV3d axis2 = { 0.0f, 0.0f, 1.0f };
        const RwV3d scale = { 0.7f, 1.0f, 1.0f };

        RpAnimBlendClumpUpdateAnimations(clump, 1.0f / 50.0f);
        if (IsClumpSkinned(clump)) {
            RpHAnimHierarchy* hier = GetAnimHierarchyFromSkinClump(clump);
            RpHAnimHierarchyUpdateMatrices(hier);
        }

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
    }

    static void DrawPlayerSetupScreen(CMenuManager* _this) {
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

        if (!_this->m_bSkinsEnumerated) {
            ClearSkins();
            PushSkin(0, UnicodeToAscii((wchar_t*)textLoader.Get("FET_DSN")), RemoveEx(UnicodeToAscii((wchar_t*)textLoader.Get("FET_DSN"))).c_str(), nullptr);

            ScanPlayerSkins();

            // Fix default saved name
            if (!strcmp(_this->m_nPrefsSkinFile, DEFAULT_SKIN_NAME))
                strcpy(_this->m_nPrefsSkinFile, playerSkins[0].m_aSkinNameDisplayed);

            _this->m_bSkinsEnumerated = true;
        }

        float x = 96.0f;
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
        CFont::PrintString(ScaleXKeepCentered(x + GetMenuOffsetX()), ScaleY(y), textLoader.Get("FES_SKN"));

        CFont::SetRightJustifyOn();
        CFont::PrintString(ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX()), ScaleY(y), textLoader.Get("FES_DAT"));

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
                    _this->CheckHover(ScaleXKeepCentered(x + GetMenuOffsetX()), ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), ScaleY(y + 22.0f + nexty + 12.0f))) {
                    
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

                CFont::PrintString(ScaleXKeepCentered(x + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), skin.m_aSkinNameDisplayed);

                CFont::SetRightJustifyOn();
                CFont::PrintString(ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 22.0f + nexty), skin.m_aDateInfo);

                renderedItems++;
            }

            CRGBA c = { 255, 255, 255, 255 };
            CRect rect;
            rect.left = ScaleXKeepCentered(x + GetMenuOffsetX());
            rect.top = ScaleY(y + 20.0f);

            rect.right = ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX());
            rect.bottom = ScaleY(y + 20.0f + 0.5f);

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            CSprite2d::SetVertices(rect, c, c, c, c);
            RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);

            rect.left = ScaleXKeepCentered(x + GetMenuOffsetX());
            rect.top = ScaleY(y + 38.0f + 12.0f * NUM_VISIBLE_ITEMS);

            rect.right = ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX());
            rect.bottom = ScaleY(y + 38.5f + 12.0f * NUM_VISIBLE_ITEMS);

            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            CSprite2d::SetVertices(rect, c, c, c, c);
            RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);

            CFont::SetColor(CRGBA(HUD_COLOUR_LCS_MENU, GetAlpha()));

            char buff[256];
            sprintf(buff, "Current: %s", _this->m_nPrefsSkinFile);
            CFont::SetRightJustifyOff();
            CFont::PrintString(ScaleXKeepCentered(x + GetMenuOffsetX()), ScaleY(y + 42.0f + 12.0f * NUM_VISIBLE_ITEMS), buff);

            sprintf(buff, "%d/%d", currPlayerSkinPage + 1, lastPage + 1);
            CFont::SetRightJustifyOn();
            CFont::PrintString(ScaleXKeepCentered(x + 256.0f + GetMenuOffsetX()), ScaleY(y + 42.0f + 12.0f * NUM_VISIBLE_ITEMS), buff);

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

            CRGBA ssc = CRGBA(40, 40, 50, 255);
            if (skinSelSprite.m_pTexture)
                skinSelSprite.Draw(CRect((SCREEN_WIDTH / 2) + ScaleX(38.0f - ((1.0f - playerZoomLerp) * 128.0f)), 0.0f, (SCREEN_WIDTH / 2) + ScaleX(38.0f + 160.0f + ((1.0f - playerZoomLerp) * 128.0f)), SCREEN_HEIGHT - ScaleY(90.25f)), ssc);

            RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

            CSprite2d::DrawRect(CRect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT), CRGBA(0, 0, 0, 0));
            RenderFrontendPlayer(_this, rot);
        }
    }

    static inline void RenderFrontendPlayer(CMenuManager* _this, float rot) {
        D3DVIEWPORT8 previousViewport = {};
        D3DVIEWPORT8 newViewport = {};
        newViewport.X = 0.0f; // (SCREEN_WIDTH / 2) + ScaleX(38.0f);
        newViewport.Y = ScaleY(4.0f), (SCREEN_WIDTH / 2) + ScaleX(198.0f);
        newViewport.Width = SCREEN_WIDTH;// (SCREEN_WIDTH / 2) + ScaleX(198.0f);
        newViewport.Height = SCREEN_HEIGHT - ScaleY(90.25f);

        newViewport.Width -= newViewport.X;
        newViewport.Height -= newViewport.Y;

        GetD3DDevice<IDirect3DDevice8>()->GetViewport(&previousViewport);
        GetD3DDevice<IDirect3DDevice8>()->SetViewport(&newViewport);

        // Render player
        static uint32_t prevTime = 0;

        // Input   
        if (currentInput == INPUT_STANDARD
            && ((CPad::GetPad(0)->NewMouseControllerState.lmb
            && _this->m_nMousePosX >= (SCREEN_WIDTH / 2) + ScaleX(48.0f) && _this->m_nMousePosX <= (SCREEN_WIDTH / 2) + ScaleX(64.0f + 128.0f))
            || rot))
            playerRotation += (_this->m_nMousePosX - _this->m_nMouseOldPosX) + rot;

        if (playerRotation >= 360.0f) {
            playerRotation -= 360.0f;
            playerRotationLerp -= 360.0f;
        }

        if (playerRotation < 0.0f) {
            playerRotation += 360.0f;
            playerRotationLerp += 360.0f;
        }

        const float f = (CTimer::m_snTimeInMillisecondsPauseMode - prevTime) * 0.02f;
        playerRotationLerp = std::lerp(playerRotationLerp, playerRotation, f * 0.5f);

        prevTime = CTimer::m_snTimeInMillisecondsPauseMode;

        RpClump* clump = gpPlayerClump;
        if (clump)
            RenderClump(clump);

        GetD3DDevice<IDirect3DDevice8>()->SetViewport(&previousViewport);
    }
#endif

    static inline void LoadSettings() {
        plugin::config_file config(true, false);
        prefsConfigSetup = config["PrefsConfigSetup"].asInt(0);
        prefsVibration = config["PrefsVibration"].asBool(false);

    }

    static inline void SaveSettings() {
        plugin::config_file config(true, true);

        std::string str;
        config << "; SkyUI for " << plugin::GetGameVersionName() << "." << config.endl();
        config << "; Controller settings will be saved here when changed from menu." << config.endl();

#ifdef GTASA
        config << "; Setup 0-1 (Set to 1 for GTA IV styled controls).";
#else
        config << "; Setup 0-4 (Set to 4 for GTA IV styled controls).";
#endif

        config["PrefsConfigSetup"] = prefsConfigSetup;
        config["PrefsVibration"] = prefsVibration;
        config.setUseAlignment(false);
        config.setUsePrecision(true);
        config.save();
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

#if defined(GTA3) && defined(LCSFICATION)
            SetLCSFontStyle();
#endif
            CFont::PrintString(ScaleXKeepCentered(52.0f + GetMenuOffsetX()), y + ScaleY(38.0f + MENU_OFFSET_Y), str);
        };
        plugin::patch::RedirectCall(0x484F3A, LAMBDA(void, __cdecl, drawBriefs, float, float, wchar_t*));

        auto drawLeftString = [](float, float y, wchar_t* str) {
            CFont::Details.m_Color.a = min(CFont::Details.m_Color.a, GetAlpha());
            CFont::Details.m_DropColor.a = min(CFont::Details.m_DropColor.a, GetAlpha());

            if (FrontEndMenuManager.m_nCurrentMenuPage == MENUPAGE_CONTROLLER_PS2)
                CFont::SetOrientation(ALIGN_LEFT);

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
        plugin::patch::SetUChar(0x47DA11 + 6, 255);

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

        auto leftStringTextGet = [](CText*, uint32_t, const char* str) {
            return textLoader.Get(str);
        };
        plugin::patch::RedirectCall({ 0x49EE23, 0x49EAB8 }, LAMBDA(const plugin::char_t*, __fastcall, leftStringTextGet, CText*, uint32_t, const char*));

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

        plugin::patch::RedirectJump(0x4A3241, (void*)0x4A325E);
        plugin::patch::RedirectJump(0x4A3290, (void*)0x4A32AD);

        // Setup skin
        plugin::patch::Nop(0x4998FA, 5);
        plugin::patch::Nop(0x4984B7, 5);

        plugin::patch::SetChar(0x49955E + 6, -1);
        plugin::patch::Nop(0x499526, 7);
        plugin::patch::Nop(0x4995EF, 7);
#elif GTASA
        // No half size map key.
        plugin::patch::Nop(0x57516D, 4);
        plugin::patch::Nop(0x576307, 4);

        // No help text
        plugin::patch::Nop(0x57E3AE, 5);

        // Legend key
        auto legendKey = []() {
            if (FrontEndMenuManager.n_nMenuSystemPanelId != -99)
                return false;

            if (HasPadInHands())
                return CPad::GetPad(0)->NewState.ButtonCross && !CPad::GetPad(0)->OldState.ButtonCross;

            return false;
        };
        plugin::patch::RedirectCall(0x578BE8, LAMBDA(bool, _cdecl, legendKey));
        plugin::patch::SetUInt(0x578BED, 0x5F75C084);
        plugin::patch::SetUShort(0x578BF1, 0x05EB);

        // No header
        injector::MakeNOP(0x57F737, 5);
        injector::MakeNOP(0x579698, 5);
#endif

        Init();

        plugin::Events::initRwEvent += []() {
            LoadSettings();

            InitAfterRw();
        };

        plugin::Events::shutdownRwEvent += []() {
            Shutdown();
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

#ifdef GTA3
        plugin::patch::StaticHook(0x47B594, 0x47B594 + 8, [](plugin::patch::RegPack& regs) {
            int32_t i = *(int32_t*)(regs.esp + 0x408 + -0x3E8);
            (*(const plugin::char_t**)(regs.esp + 0x408 + -0x3FC)) = ProcessMenuOptionsStrings((CMenuManager*)regs.ebp, i);
        });
#elif GTAVC
        plugin::patch::StaticHook(0x49E726, 0x49E726 + 8, [](plugin::patch::RegPack& regs) {
            regs.ebx = 0;
            regs.edx = regs.eax;
            *(int32_t*)(regs.esp + 0x158 + -0x148) = regs.eax;

            int32_t i = *(int32_t*)(&regs.eax);
            (*(const plugin::char_t**)(&regs.ebx)) = ProcessMenuOptionsStrings((CMenuManager*)regs.ebp, i);
        });
#elif GTASA
        plugin::patch::StaticHook(0x579907, 0x579907 + 6, [](plugin::patch::RegPack& regs) {
            regs.esi = 0;
            *(int32_t*)(regs.esp + 0x130 + -0x120) = regs.edi;
        
            int32_t i = *(int32_t*)(regs.esp + 0x130 + -0x10C);
            (*(const plugin::char_t**)(&regs.esi)) = ProcessMenuOptionsStrings((CMenuManager*)regs.ebp, i);
        });
#endif



#if defined(GTA3) || defined(GTAVC)
        onDrawStandardMenu.before += [](CMenuManager* _this) {
            DrawBack(_this);

#ifdef GTA3
            int32_t y = 40;
            switch (_this->m_nCurrentMenuPage) {
                case MENUPAGE_CONTROLLER_PS2:
                    y = 200;
                    break;
            }
            plugin::patch::SetInt(0x47B41F + 4, y);
#endif
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

#if defined(GTA3)
#if defined(LCSFICATION)
        // fontstyles
        plugin::patch::SetUChar(0x47B404 + 1, 0);
        plugin::patch::SetUChar(0x47B39B + 1, 0);
        plugin::patch::SetUChar(0x47B2B7 + 1, 0);
        plugin::patch::SetUChar(0x47B32F + 1, 0);
        plugin::patch::SetUChar(0x47B167 + 1, 0);
        plugin::patch::SetUChar(0x47B1D3 + 1, 0);
        plugin::patch::SetUChar(0x48270A + 1, 0);

        // font scale
        plugin::patch::SetFloat(0x47B415 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B406 + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B3AC + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B39D + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B2C8 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B2B9 + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B250 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B241 + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B340 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B331 + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B178 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B169 + 6, CONSOLE_MENU_SCALE_Y); // y

        plugin::patch::SetFloat(0x47B1E4 + 6, CONSOLE_MENU_SCALE_X); // x
        plugin::patch::SetFloat(0x47B1D5 + 6, CONSOLE_MENU_SCALE_Y); // y

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
#endif
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
        //static uint8_t rectcol[] = { HUD_COLOUR_BLUELIGHT, 150 };
        //plugin::patch::SetUChar(0x481751 + 1, rectcol[0]);
        //plugin::patch::SetUChar(0x48174C + 1, rectcol[1]);
        //plugin::patch::SetUChar(0x48174A + 1, rectcol[2]);
        //plugin::patch::SetUChar(0x48173B + 1, rectcol[3]);

        auto rect = [](CRect const&, CRGBA const& col) {
            CSprite2d::DrawRect(CRect(ScaleX(14.0f), ScaleY(34.0f), SCREEN_WIDTH - ScaleX(14.0f), SCREEN_HEIGHT - ScaleY(38.0f)), col);
        };
        plugin::patch::RedirectCall(0x4817D0, LAMBDA(void, __cdecl, rect, CRect const&, CRGBA const&));

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

        // no print controller
        plugin::patch::Nop(0x47F292, 7);
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

        auto drawTripSkipSprite = [](CSprite2d* sprite, uint32_t, CRect const& rect, CRGBA const& col) {
            const float x = rect.left;
            const float y = rect.top;

            const float w = (rect.right - rect.left) / 2;
            const float h = (rect.bottom - rect.top) / 2;

            auto skipIcon = hudSprites.GetSprite("SkipHigh");
            skipIcon->Draw(CRect(x, y, x + w, y + h), col); // Left top
            skipIcon->Draw(CRect(x + w + w, y, x + w, y + h), col); // Right top

            skipIcon->Draw(CRect(x, y + h + h, x + w, y + h), col); // Left bottom
            skipIcon->Draw(CRect(x + w + w, y + h + h, x + w, y + h), col); // Right bottom

            if (FLASH_ITEM(1000, 500))
                hudSprites.GetSprite("SkipHigh")->Draw(x + (w * 1.15f), y + (h * 0.775f), w * 0.425f, h * 0.425f, col);
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

 }

    ~SkyUI() {

    }
} skyUI;
