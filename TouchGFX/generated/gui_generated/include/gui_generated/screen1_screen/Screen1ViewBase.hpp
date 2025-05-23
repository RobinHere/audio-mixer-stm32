/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN1VIEWBASE_HPP
#define SCREEN1VIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include "Screen1State.hpp"
#include "audio_module.h"
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class Screen1ViewBase : public touchgfx::View<Screen1Presenter>
{
public:
    Screen1ViewBase();
    virtual ~Screen1ViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::ScalableImage scalableImage1;
    touchgfx::Slider outputSlider;
    touchgfx::Slider inL1Slider;
    touchgfx::Slider inR1Slider;
    touchgfx::Slider inL2Slider;
    touchgfx::Slider inR2Slider;
    touchgfx::TextArea outputGainText;
    touchgfx::TextArea inL1GainText;
    touchgfx::TextArea inR1GainText;
    touchgfx::TextArea inL2GainText;
    touchgfx::TextArea inR2GainText;
    touchgfx::TextArea nextScreenText;
    touchgfx::TextArea dbText1;
    touchgfx::TextArea dbText2;
    touchgfx::TextArea dbText3;
    touchgfx::TextArea dbText4;
    touchgfx::TextArea dbText5;
    touchgfx::TextArea dbText6;
    touchgfx::TextArea dbText7;
    touchgfx::TextArea dbText8;
    touchgfx::TextArea dbText9;
    touchgfx::TextArea dbText10;
    touchgfx::IconButtonStyle< touchgfx::ClickButtonTrigger >  nextScreenButton;
    Screen1State& screen1State = Screen1State::getInstance();

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen1ViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;
    touchgfx::Callback<Screen1ViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SCREEN1VIEWBASE_HPP
