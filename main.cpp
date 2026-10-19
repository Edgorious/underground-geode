#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/GameEvent.hpp>

using namespace geode::prelude;

namespace {
    struct TestMode {
        char const* name;
        int displayedTicks;
    };

    // These are display/test presets only. They do not alter game timing,
    // collision, input handling, or the simulation.
    constexpr TestMode kModes[] = {
        {"OFF", 0},
        {"STAGE 1", 2},
        {"STAGE 2", 4},
    };
    constexpr int kModeCount = static_cast<int>(sizeof(kModes) / sizeof(kModes[0]));

    int g_modeIndex = 0;
    class UndergroundConsole;
    UndergroundConsole* g_console = nullptr;

    class UndergroundConsole final : public CCLayer {
    protected:
        CCLabelBMFont* m_title = nullptr;
        CCLabelBMFont* m_mode = nullptr;
        CCLabelBMFont* m_ticks = nullptr;
        CCLabelBMFont* m_note = nullptr;

        bool init() override {
            if (!CCLayer::init()) return false;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            this->setTouchEnabled(false);

            auto panel = CCLayerColor::create({15, 18, 28, 220}, 250.f, 102.f);
            panel->setAnchorPoint({0.f, 1.f});
            panel->setPosition({12.f, winSize.height - 12.f});
            this->addChild(panel);

            m_title = CCLabelBMFont::create("UNDERGROUND // TEST CONSOLE", "bigFont.fnt");
            m_title->setAnchorPoint({0.f, 0.5f});
            m_title->setScale(0.34f);
            m_title->setPosition({10.f, 82.f});
            panel->addChild(m_title);

            m_mode = CCLabelBMFont::create("MODE: OFF", "bigFont.fnt");
            m_mode->setAnchorPoint({0.f, 0.5f});
            m_mode->setScale(0.42f);
            m_mode->setPosition({10.f, 58.f});
            panel->addChild(m_mode);

            m_ticks = CCLabelBMFont::create("DISPLAY TICKS: 0", "bigFont.fnt");
            m_ticks->setAnchorPoint({0.f, 0.5f});
            m_ticks->setScale(0.38f);
            m_ticks->setPosition({10.f, 36.f});
            panel->addChild(m_ticks);

            m_note = CCLabelBMFont::create("DISPLAY-ONLY TEST MODE", "chatFont.fnt");
            m_note->setAnchorPoint({0.f, 0.5f});
            m_note->setScale(0.55f);
            m_note->setPosition({10.f, 15.f});
            panel->addChild(m_note);

            refresh();
            g_console = this;
            return true;
        }

    public:
        static UndergroundConsole* create() {
            auto ret = new UndergroundConsole();
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void refresh() {
            if (!m_mode || !m_ticks) return;
            auto const& mode = kModes[g_modeIndex];
            m_mode->setString(fmt::format("MODE: {}", mode.name).c_str());
            m_ticks->setString(fmt::format("DISPLAY TICKS: {}", mode.displayedTicks).c_str());
        }

        void onExit() override {
            if (g_console == this) g_console = nullptr;
            CCLayer::onExit();
        }
    };

    void cycleMode() {
        g_modeIndex = (g_modeIndex + 1) % kModeCount;
        if (g_console) g_console->refresh();
        log::info("Underground test console: mode={} displayTicks={}",
            kModes[g_modeIndex].name, kModes[g_modeIndex].displayedTicks);
    }
}

class $modify(UndergroundPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (auto console = UndergroundConsole::create()) {
            this->addChild(console, 9999);
        }

        this->addEventListener(
            KeybindSettingPressedEventV3(Mod::get(), "cycle-mode"),
            [](Keybind const&, bool down, bool repeat, double) {
                if (down && !repeat) cycleMode();
            }
        );
        return true;
    }
};
