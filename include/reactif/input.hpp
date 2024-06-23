#pragma once

#include "enums.hpp"
#include "widget.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Secret_Input.H>

namespace rf::detail {

template <class Message, class B>
struct InputProps {
    std::optional<std::shared_ptr<std::string>> value;
    std::optional<Color> textcolor;
    std::optional<Font> textfont;
    std::optional<int> textsize;
    std::optional<std::shared_ptr<std::function<Message()>>> on_trigger;
    void view(B *w) {
        if (value)
            w->value((*value)->c_str());
        if (textcolor)
            w->textcolor(*textcolor);
        if (textfont)
            w->textfont(*textfont);
        if (textsize)
            w->textsize(*textsize);
    }
    void update(B *w, const InputProps &other) {
        if (*this == other)
            return;
        if (other.value != value) {
            value = other.value;
            if (value)
                w->value((*value)->c_str());
        }
        if (other.textcolor != textcolor) {
            textcolor = other.textcolor;
            if (textcolor)
                w->textcolor(*textcolor);
        }
        if (other.textfont != textfont) {
            textfont = other.textfont;
            if (textfont)
                w->textfont(*textfont);
        }
        if (other.textsize != textsize) {
            textsize = other.textsize;
            if (textsize)
                w->textcolor(*textsize);
        }
    }
    bool operator==(const InputProps &) const = default;
};

template <class Message, class W, class B>
class InputBase : public WidgetBase<Message, W, B> {
  protected:
    InputProps<Message, B> iprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->iprops.view(this->inner);
        if (this->iprops.value) {
            int when = FL_WHEN_CHANGED;
            if (this->iprops.on_trigger)
                when |= FL_WHEN_ENTER_KEY_ALWAYS;
            this->inner->when(when);
            static_cast<FlWidgetWrapper<B> *>(this->inner)
                ->cb([data = this->iprops.value->get(),
                      cb   = this->iprops.on_trigger
                                 ? this->iprops.on_trigger->get()
                                 : nullptr](auto *w) {
                    if (data && Fl::callback_reason() == FL_REASON_CHANGED) {
                        auto i     = static_cast<B *>(w);
                        auto val   = i->value();
                        auto data1 = static_cast<std::string *>(data);
                        *data1     = std::string(val);
                    }
                    if (cb && Fl::callback_reason() == FL_REASON_ENTER_KEY) {
                        Fl::awake(cb);
                    }
                });
        }
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        WidgetBase<Message, W, B>::update(other);
        auto f = (W *)other;
        this->iprops.update(this->inner, f->iprops);
    }
    virtual ~InputBase() = default;

    /// Set the input's value
    W &value(std::shared_ptr<std::string> value) {
        iprops.value = value;
        return *(W *)this;
    }
    /// Set the input's textcolor
    W &textcolor(Color col) {
        iprops.textcolor = col;
        return *(W *)this;
    }
    /// Set the input's textsize
    W &textsize(int sz) {
        iprops.textsize = sz;
        return *(W *)this;
    }
    /// Set the input's textfont
    W &textfont(Font font) {
        iprops.textfont = font;
        return *(W *)this;
    }
    /// Set the input's callback message
    W &on_trigger(std::function<Message()> &&msg) {
        iprops.on_trigger =
            std::make_shared<std::function<Message()>>(std::move(msg));
        return *(W *)this;
    }
};

#define INPUT(Class, Base)                                                     \
    template <class Message>                                                   \
    class Class : public InputBase<Message, Class<Message>, Base> {};

INPUT(Input, Fl_Input)
INPUT(IntInput, Fl_Int_Input)
INPUT(FloatInput, Fl_Float_Input)
INPUT(MultilineInput, Fl_Multiline_Input)
INPUT(SecretInput, Fl_Secret_Input)
INPUT(FileInput, Fl_File_Input)
} // namespace rf::detail
