#pragma once

#include "widget.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Fill_Dial.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Hor_Fill_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Line_Dial.H>
#include <FL/Fl_Nice_Slider.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Value_Slider.H>

namespace rf::detail {

template <class B>
struct ValuatorProps {
    std::optional<double> value;
    std::optional<double> minimum;
    std::optional<double> maximum;
    std::optional<double> step;
    std::optional<int> precision;
    void view(B *w) {
        if (value)
            w->value(*value);
        if (minimum)
            w->minimum(*minimum);
        if (maximum)
            w->maximum(*maximum);
        if (step)
            w->step(*step);
        if (precision)
            w->precision(*precision);
    }
    void update(B *w, const ValuatorProps &other) {
        if (*this == other)
            return;
        if (other.value != value) {
            value = other.value;
            if (value)
                w->value(*value);
        }
        if (other.minimum != minimum) {
            minimum = other.minimum;
            if (minimum)
                w->minimum(*minimum);
        }
        if (other.maximum != maximum) {
            maximum = other.maximum;
            if (maximum)
                w->maximum(*maximum);
        }
        if (other.step != step) {
            step = other.step;
            if (step)
                w->step(*step);
        }
        if (other.precision != precision) {
            precision = other.precision;
            if (precision)
                w->precision(*precision);
        }
    }
    bool operator==(const ValuatorProps &) const = default;
};

template <class Message, class W, class B>
class ValuatorBase : public WidgetBase<Message, W, B> {
  protected:
    ValuatorProps<B> vprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->vprops.view(this->inner);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);
        this->vprops.update(this->inner, f->vprops);
    }
    virtual ~ValuatorBase() = default;

    /// Set the valuator's value
    W &value(double value) {
        vprops.value = value;
        return *(W *)this;
    }
    /// Set the valuator's minimum
    W &min(double v) {
        vprops.minimum = v;
        return *(W *)this;
    }
    /// Set the valuator's maximum
    W &max(double v) {
        vprops.maximum = v;
        return *(W *)this;
    }
    /// Set the valuator's step
    W &step(double v) {
        vprops.step = v;
        return *(W *)this;
    }
    /// Set the valuator's precision
    W &precision(int v) {
        vprops.precision = v;
        return *(W *)this;
    }
};

#define VALUATOR(Class, Base)                                                  \
    template <class Message>                                                   \
    class Class : public ValuatorBase<Message, Class<Message>, Base> {};

VALUATOR(Dial, Fl_Dial)
VALUATOR(Slider, Fl_Slider)
VALUATOR(NiceSlider, Fl_Nice_Slider)
VALUATOR(ValueSlider, Fl_Value_Slider)
VALUATOR(LineDial, Fl_Line_Dial)
VALUATOR(Counter, Fl_Counter)
VALUATOR(Scrollbar, Fl_Scrollbar)
VALUATOR(Roller, Fl_Roller)
VALUATOR(Adjuster, Fl_Adjuster)
VALUATOR(ValueInput, Fl_Value_Input)
VALUATOR(ValueOutput, Fl_Value_Output)
VALUATOR(FillSlider, Fl_Fill_Slider)
VALUATOR(FillDial, Fl_Fill_Dial)
VALUATOR(HorSlider, Fl_Hor_Slider)
VALUATOR(HorFillSlider, Fl_Hor_Fill_Slider)
VALUATOR(HorNiceSlider, Fl_Hor_Nice_Slider)
VALUATOR(HorValueSlider, Fl_Hor_Value_Slider)
} // namespace rf::detail
