#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Experimental Android64 starter implementation.
// The click is split across updates so the press and release are not issued
// in the same update. This source has not been compiled or device-tested here.
class $modify(MobileAutoClickerPlayLayer, PlayLayer) {
    double m_elapsed = 0.0;
    bool m_releaseNextUpdate = false;

    void update(float dt) {
        PlayLayer::update(dt);

        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            m_elapsed = 0.0;
            if (m_releaseNextUpdate) {
                this->handleButton(false, 1, true);
                m_releaseNextUpdate = false;
            }
            return;
        }

        if (m_releaseNextUpdate) {
            this->handleButton(false, 1, true);
            m_releaseNextUpdate = false;
        }

        const int cps = Mod::get()->getSettingValue<int64_t>("cps");
        const double interval = 1.0 / static_cast<double>(cps > 0 ? cps : 1);
        m_elapsed += dt;

        if (m_elapsed >= interval) {
            m_elapsed = 0.0;
            this->handleButton(true, 1, true);
            m_releaseNextUpdate = true;
        }
    }
};
