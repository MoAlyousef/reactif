#pragma once

#include "enums.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Widget.H>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>

namespace rf {

template <class Message>
class Widget {
  public:
    /// Create a Widget wrapper
    virtual std::shared_ptr<Widget> create() = 0;
    /// view is called to create the FLTK widget and set its properties
    virtual Fl_Widget *view() = 0;
    /// Update the properties of the widget
    virtual void update(Widget *) = 0;
    virtual ~Widget()             = default;
};

namespace detail {

template <class T>
    requires(std::is_base_of_v<Fl_Widget, T>)
class FlWidgetWrapper : public T {
  public:
    std::function<void(FlWidgetWrapper *, int, int, int, int)> resize_cb;
    std::shared_ptr<std::function<void(FlWidgetWrapper *)>> cb_;
    FlWidgetWrapper(int x, int y, int w, int h, const char *label = nullptr)
        : T(x, y, w, h, label) {}
    void resize(int x, int y, int w, int h) override {
        T::resize(x, y, w, h);
        if (resize_cb)
            resize_cb(this, x, y, h, w);
    }
    void cb(std::function<void(FlWidgetWrapper *)> &&f) {
        cb_ = std::make_shared<std::function<void(FlWidgetWrapper *)>>(f);
        this->callback(
            [](auto *w, auto *data) {
                if (data) {
                    auto cb = (std::function<void(FlWidgetWrapper *)> *)data;
                    (*cb)((FlWidgetWrapper *)w);
                }
            },
            cb_.get()
        );
    }
};

template <class Message, class B>
struct WidgetProps {
    std::optional<std::string> label;
    std::optional<std::string> tooltip;
    std::optional<std::pair<int, int>> pos;
    std::optional<std::pair<int, int>> size;
    std::optional<int> subtype;
    std::optional<int> fixed;
    std::optional<Color> color;
    std::optional<Color> labelcolor;
    std::optional<Color> selection_color;
    std::optional<int> labelsize;
    std::optional<Font> labelfont;
    std::optional<LabelType> labeltype;
    std::optional<BoxType> box;
    std::optional<bool> hidden;
    std::optional<Align> align;
    std::optional<bool> deactivated;
    std::optional<When> when;

    void view(B *w) {
        if (label)
            w->copy_label(label->c_str());
        if (tooltip)
            w->tooltip(label->c_str());
        if (pos || size) {
            auto [x, y]          = pos ? *pos : std::pair(0, 0);
            auto [width, height] = size ? *size : std::pair(0, 0);
            w->resize(x, y, width, height);
        }
        if (subtype)
            w->type(*subtype);
        if (fixed) {
            auto *flex = dynamic_cast<Fl_Flex *>(w->parent());
            if (flex)
                flex->fixed(w, *fixed);
        }
        if (color)
            w->color(*color);
        if (selection_color)
            w->selection_color(*selection_color);
        if (labelcolor)
            w->labelcolor(*labelcolor);
        if (labelsize)
            w->labelsize(*labelsize);
        if (labelfont)
            w->labelfont((Fl_Font)*labelfont);
        if (labeltype)
            w->labeltype((Fl_Labeltype)*labeltype);
        if (box)
            w->box((Fl_Boxtype)*box);
        if (hidden) {
            if (*hidden)
                w->hide();
            else
                w->show();
        }
        if (deactivated) {
            if (*deactivated)
                w->deactivate();
            else
                w->activate();
        }
        if (align)
            w->align(*align);
        if (when)
            w->when(*when);
    }
    void update(B *w, const WidgetProps &other) {
        if (*this == other)
            return;
        if (other.label != label) {
            label = other.label;
            if (label)
                w->copy_label(label->c_str());
        }
        if (other.tooltip != tooltip) {
            tooltip = other.tooltip;
            if (tooltip)
                w->tooltip(tooltip->c_str());
        }
        if (other.pos != pos) {
            pos = other.pos;
        }
        if (other.size != size) {
            size = other.size;
        }
        if (other.pos != pos || other.size != size) {
            auto [x, y]          = pos ? *pos : std::pair(0, 0);
            auto [width, height] = size ? *size : std::pair(0, 0);
            w->resize(x, y, width, height);
        }
        if (other.subtype != subtype) {
            subtype = other.subtype;
            if (subtype)
                w->type(*subtype);
        }
        if (other.color != color) {
            color = other.color;
            w->color(*color);
        }
        if (other.selection_color != selection_color) {
            selection_color = other.selection_color;
            w->selection_color(*selection_color);
        }
        if (other.labelcolor != labelcolor) {
            labelcolor = other.labelcolor;
            w->labelcolor(*labelcolor);
        }
        if (other.labelfont != labelfont) {
            labelfont = other.labelfont;
            w->labelfont((Fl_Font)*labelfont);
        }
        if (other.labelsize != labelsize) {
            labelsize = other.labelsize;
            w->labelsize(*labelsize);
        }
        if (other.box != box) {
            box = other.box;
            w->labelsize((Fl_Boxtype)*box);
        }
        if (other.hidden != hidden) {
            hidden = other.hidden;
            if (hidden) {
                if (*hidden)
                    w->hide();
                else
                    w->show();
            }
        }
        if (other.deactivated != deactivated) {
            deactivated = other.deactivated;
            if (deactivated) {
                if (*deactivated)
                    w->deactivate();
                else {
                    w->activate();
                }
            }
        }
        if (other.align != align) {
            align = other.align;
            if (align)
                w->align(*align);
        }
        if (other.when != when) {
            when = other.when;
            if (when)
                w->when(*when);
        }
    }
    bool operator==(const WidgetProps &) const = default;
};

template <class Message, class W, class B>
class WidgetBase : public Widget<Message> {
  protected:
    FlWidgetWrapper<B> *inner      = nullptr;
    WidgetProps<Message, B> wprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        inner = new FlWidgetWrapper<B>(0, 0, 0, 0); // NOLINT
        wprops.view(inner);
        return inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        wprops.update(inner, f->wprops);
    }
    virtual ~WidgetBase() = default;

    /// Set the label
    W &label(std::string_view label) {
        wprops.label = label;
        return *(W *)this;
    }
    /// Set the tooltip
    W &tooltip(std::string_view tooltip) {
        wprops.tooltip = tooltip;
        return *(W *)this;
    }
    /// Set the alignment
    W &align(Align align) {
        wprops.align = align;
        return *(W *)this;
    }
    /// Set the callback trigger
    W &when(When when) {
        wprops.when = when;
        return *(W *)this;
    }
    /// Set whether the widget is hidden
    W &hidden(bool flag) {
        wprops.hidden = flag;
        return *(W *)this;
    }
    /// Set whether the widget is deactivated
    W &deactivated(bool flag) {
        wprops.deactivated = flag;
        return *(W *)this;
    }
    /// Set the size
    W &size(int a, int b) {
        wprops.size = std::make_pair(a, b);
        return *(W *)this;
    }
    /// Set the position
    W &pos(int a, int b) {
        wprops.pos = std::make_pair(a, b);
        return *(W *)this;
    }
    /// Set whether the widget is fixed if the parent is a flex widget
    W &fixed(int sz) {
        wprops.fixed = sz;
        return *(W *)this;
    }
    /// Set the color
    W &color(Color col) {
        wprops.color = col;
        return *(W *)this;
    }
    /// Set the selection color
    W &selection_color(Color col) {
        wprops.selection_color = col;
        return *(W *)this;
    }
    /// Set the label color
    W &labelcolor(Color col) {
        wprops.labelcolor = col;
        return *(W *)this;
    }
    /// Set the label font
    W &labelfont(Font font) {
        wprops.labelfont = font;
        return *(W *)this;
    }
    /// Set the label size
    W &labelsize(int sz) {
        wprops.labelsize = sz;
        return *(W *)this;
    }
    /// Set the label type
    W &labeltype(LabelType t) {
        wprops.labeltype = t;
        return *(W *)this;
    }
    /// Set the box type
    W &box(BoxType b) {
        wprops.box = b;
        return *(W *)this;
    }
};
} // namespace detail
} // namespace rf
