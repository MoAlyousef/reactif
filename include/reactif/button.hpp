#pragma once

#include "widget.hpp"
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Toggle_Button.H>

namespace rf::detail {

template <class Message, class B>
struct ButtonProps {
    std::optional<bool> value;
    std::optional<Shortcut> shortcut;
    std::optional<BoxType> downbox;
    std::optional<std::shared_ptr<std::function<Message()>>> on_trigger;
    void view(B *w) {
        if (value)
            w->value(*value);
        if (downbox)
            w->down_box((Fl_Boxtype)*downbox);
        if (shortcut)
            w->shortcut(*shortcut);
        if (on_trigger)
            static_cast<FlWidgetWrapper<B> *>(w)->cb(
                [data = on_trigger->get()](auto *) { Fl::awake(data); }
            );
    }
    void update(B *w, const ButtonProps &other) {
        if (*this == other)
            return;
        if (other.value != value) {
            value = other.value;
            if (value)
                w->value(*value);
        }
        if (other.shortcut != shortcut) {
            shortcut = other.shortcut;
            if (value)
                w->shortcut(*shortcut);
        }
        if (other.downbox != downbox) {
            downbox = other.downbox;
            if (downbox)
                w->down_box((Fl_Boxtype)*downbox);
        }
    }
    bool operator==(const ButtonProps &) const = default;
};

template <class Message, class W, class B>
class ButtonBase : public WidgetBase<Message, W, B> {
  protected:
    ButtonProps<Message, B> bprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->bprops.view(this->inner);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        WidgetBase<Message, W, B>::update(other);
        auto f = (W *)other;
        this->bprops.update(this->inner, f->bprops);
    }
    virtual ~ButtonBase() = default;
    /// Set the button's downbox
    W &downbox(BoxType b) {
        bprops.downbox = b;
        return *(W *)this;
    }
    /// Set the button's value
    W &value(bool b) {
        bprops.value = b;
        return *(W *)this;
    }
    /// Set the button's shortcut
    W &shortcut(Shortcut b) {
        bprops.shortcut = b;
        return *(W *)this;
    }
    /// Set the button's callback message
    W &on_trigger(std::function<Message()> &&msg) {
        bprops.on_trigger =
            std::make_shared<std::function<Message()>>(std::move(msg));
        return *(W *)this;
    }
};

#define BUTTON(Class, Base)                                                    \
    template <class Message>                                                   \
    class Class : public ButtonBase<Message, Class<Message>, Base> {};

BUTTON(Button, Fl_Button)
BUTTON(RadioButton, Fl_Radio_Button)
BUTTON(ToggleButton, Fl_Toggle_Button)
BUTTON(RoundButton, Fl_Round_Button)
BUTTON(CheckButton, Fl_Check_Button)
BUTTON(LightButton, Fl_Light_Button)
BUTTON(RepeatButton, Fl_Repeat_Button)
BUTTON(RadioLightButton, Fl_Radio_Light_Button)
BUTTON(RadioRoundButton, Fl_Radio_Round_Button)
} // namespace rf::detail
