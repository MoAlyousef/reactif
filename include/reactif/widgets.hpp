#pragma once

#include "box.hpp"
#include "browser.hpp"
#include "button.hpp"
#include "group.hpp"
#include "input.hpp"
#include "menu.hpp"
#include "output.hpp"
#include "tree.hpp"
#include "valuator.hpp"

namespace rf::detail {

#define WIDGETFN(Class, Fn)                                                    \
    Class<Message> Fn() const { return Class<Message>(); }

template <class Message>
class DefaultWidgets {
  public:
    /// box() creates a WIDGETFN wrapper
    WIDGETFN(Box, box)
    /// button() creates a Button wrapper
    WIDGETFN(Button, button)
    /// radio_button() creates a RadioButton wrapper
    WIDGETFN(RadioButton, radio_button)
    /// toggle_button() creates a ToggleButton wrapper
    WIDGETFN(ToggleButton, toggle_button)
    /// round_button() creates a RoundButton wrapper
    WIDGETFN(RoundButton, round_button)
    /// check_button() creates a CheckButton wrapper
    WIDGETFN(CheckButton, check_button)
    /// light_button() creates a LightButton wrapper
    WIDGETFN(LightButton, light_button)
    /// repeat_button() creates a RepeatButton wrapper
    WIDGETFN(RepeatButton, repeat_button)
    /// radio_light_button() creates a RadioLightButton wrapper
    WIDGETFN(RadioLightButton, radio_light_button)
    /// radio_round_button() creates a RadioRoundButton wrapper
    WIDGETFN(RadioRoundButton, radio_round_button)
    /// flex() creates a WIDGETFN wrapper
    WIDGETFN(Flex, flex)
    /// group() creates a Group wrapper
    WIDGETFN(Group, group)
    /// tabs() creates a WIDGETFN wrapper
    WIDGETFN(Tabs, tabs)
    /// scroll() creates a Scroll wrapper
    WIDGETFN(Scroll, scroll)
    /// pack() creates a WIDGETFN wrapper
    WIDGETFN(Pack, pack)
    /// input() creates a Input wrapper
    WIDGETFN(Input, input)
    /// int_input() creates a IntInput wrapper
    WIDGETFN(IntInput, int_input)
    /// float_input() creates a FloatInput wrapper
    WIDGETFN(FloatInput, float_input)
    /// multiline_input() creates a MultilineInput wrapper
    WIDGETFN(MultilineInput, multiline_input)
    /// secret_input() creates a SecretInput wrapper
    WIDGETFN(SecretInput, secret_input)
    /// file_inputinput() creates a FileInput wrapper
    WIDGETFN(FileInput, file_inputinput)
    /// output() creates a Output wrapper
    WIDGETFN(Output, output)
    /// multiline_output() creates a MultilineOutput wrapper
    WIDGETFN(MultilineOutput, multiline_output)
    /// menubar() creates a MenuBar wrapper
    WIDGETFN(MenuBar, menubar)
    /// sysmenubar() creates a SysMenuBar wrapper
    WIDGETFN(SysMenuBar, sysmenubar)
    /// choice() creates a Choice wrapper
    WIDGETFN(Choice, choice)
    /// dial() creates a WIDGETFN wrapper
    WIDGETFN(Dial, dial)
    /// slider() creates a Slider wrapper
    WIDGETFN(Slider, slider)
    /// nice_slider() creates a NiceSlider wrapper
    WIDGETFN(NiceSlider, nice_slider)
    /// value_slider() creates a ValueSlider wrapper
    WIDGETFN(ValueSlider, value_slider)
    /// line_dial() creates a LineDial wrapper
    WIDGETFN(LineDial, line_dial)
    /// counter() creates a Counter wrapper
    WIDGETFN(Counter, counter)
    /// scrollbar() creates a Scrollbar wrapper
    WIDGETFN(Scrollbar, scrollbar)
    /// roller() creates a Roller wrapper
    WIDGETFN(Roller, roller)
    /// adjuster() creates a Adjuster wrapper
    WIDGETFN(Adjuster, adjuster)
    /// value_input() creates a ValueInput wrapper
    WIDGETFN(ValueInput, value_input)
    /// value_output() creates a ValueOutput wrapper
    WIDGETFN(ValueOutput, value_output)
    /// fill_slider() creates a FillSlider wrapper
    WIDGETFN(FillSlider, fill_slider)
    /// fill_dial() creates a FillDial wrapper
    WIDGETFN(FillDial, fill_dial)
    /// hor_slider() creates a HorSlider wrapper
    WIDGETFN(HorSlider, hor_slider)
    /// hor_fill_slider() creates a HorFillSlider wrapper
    WIDGETFN(HorFillSlider, hor_fill_slider)
    /// hor_nice_slider() creates a HorNiceSlider wrapper
    WIDGETFN(HorNiceSlider, hor_nice_slider)
    /// hor_value_slider() creates a HorValueSlider wrapper
    WIDGETFN(HorValueSlider, hor_value_slider)
    /// browser() creates a Browser wrapper
    WIDGETFN(Browser, browser)
    /// hold_browser() creates a HoldBrowser wrapper
    WIDGETFN(HoldBrowser, hold_browser)
    /// multi_browser() creates a MultiBrowser wrapper
    WIDGETFN(MultiBrowser, multi_browser)
    /// select_browser() creates a SelectBrowser wrapper
    WIDGETFN(SelectBrowser, select_browser)
    /// file_browser() creates a FileBrowser wrapper
    WIDGETFN(FileBrowser, file_browser)
    /// tree() creates a WIDGETFN wrapper
    WIDGETFN(Tree, tree)
    /// menu_item creates a MenuItem wrapper
    MenuItem<Message> menu_item(std::string_view label) {
        return MenuItem<Message>(label);
    }
    /// browser_item creates a BrowserItem wrapper
    BrowserItem<Message> browser_item(std::string_view label) {
        return BrowserItem<Message>(label);
    }
    /// tree_item creates a TreeItem wrapper
    TreeItem<Message> tree_item(std::string_view label) {
        return TreeItem<Message>(label);
    }
};
} // namespace rf::detail
