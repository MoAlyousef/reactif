#pragma once

#include "widget.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Output.H>

namespace rf::detail {

template <class B>
struct OutputProps {
    std::optional<std::string> value;
    std::optional<Color> textcolor;
    std::optional<Font> textfont;
    std::optional<int> textsize;
    void view(B *w) {
        if (value)
            w->value(value->c_str());
        if (textcolor)
            w->textcolor(*textcolor);
        if (textfont)
            w->textfont(*textfont);
        if (textsize)
            w->textsize(*textsize);
    }
    void update(B *w, const OutputProps &other) {
        if (*this == other)
            return;
        if (other.value != value) {
            value = other.value;
            if (value)
                w->value(value->c_str());
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
    bool operator==(const OutputProps &) const = default;
};

template <class Message, class W, class B>
class OutputBase : public WidgetBase<Message, W, B> {
  protected:
    OutputProps<B> oprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->oprops.view(this->inner);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);
        this->oprops.update(this->inner, f->oprops);
    }
    virtual ~OutputBase() = default;

    /// Set the output's value
    W &value(std::shared_ptr<std::string> value) {
        oprops.value = value;
        return *(W *)this;
    }
    /// Set the output's textcolor
    W &textcolor(Color col) {
        oprops.textcolor = col;
        return *(W *)this;
    }
    /// Set the output's textsize
    W &textsize(int sz) {
        oprops.textsize = sz;
        return *(W *)this;
    }
    /// Set the output's textfont
    W &textfont(Font font) {
        oprops.textfont = font;
        return *(W *)this;
    }
};

#define OUTPUT(Class, Base)                                                    \
    template <class Message>                                                   \
    class Class : public OutputBase<Message, Class<Message>, Base> {};

OUTPUT(Output, Fl_Output)
OUTPUT(MultilineOutput, Fl_Multiline_Output)
} // namespace rf::detail
