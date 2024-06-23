#pragma once

#include "widget.hpp"
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <span>

namespace rf::detail {
template <class Message>
class MenuItem {
    std::string label_;
    std::optional<Shortcut> shortcut_;
    std::optional<MenuFlag> flag_;
    std::optional<std::shared_ptr<std::function<Message()>>> on_trigger_;
    std::optional<int> labelsize_;

  public:
    /// Create a menu item with a label
    MenuItem(std::string_view label) : label_(std::string(label)) {}
    MenuItem &labelsize(int sz) {
        labelsize_ = sz;
        return *this;
    }
    /// Set the shortcut
    MenuItem &shortcut(Shortcut s) {
        shortcut_ = s;
        return *this;
    }
    /// Set the menu item flag
    MenuItem &flag(MenuFlag f) {
        flag_ = f;
        return *this;
    }
    /// Set the callback trigger
    MenuItem &on_trigger(std::function<Message()> &&msg) {
        on_trigger_ =
            std::make_shared<std::function<Message()>>(std::move(msg));
        return *this;
    }
    bool operator==(const MenuItem &) const = default;
    void view(Fl_Menu_ *m) const {
        auto i = m->add(
            label_.c_str(),
            shortcut_ ? (int)*shortcut_ : 0,
            on_trigger_ ? [](Fl_Widget *, void *data) { Fl::awake(data); }
                        : [](auto, auto) {},
            on_trigger_ ? on_trigger_->get() : 0,
            flag_ ? (int)*flag_ : 0
        );
        auto menu = (Fl_Menu_Item *)m->menu(); // NOLINT
        if (labelsize_)
            menu[i].labelsize(*labelsize_);
    }
    void update(Fl_Menu_ *w, const MenuItem &other) {
        auto i = w->add(
            label_.c_str(),
            shortcut_ ? (int)*shortcut_ : 0,
            on_trigger_ ? [](Fl_Widget *, void *data) { Fl::awake(data); }
                        : [](auto, auto) {},
            on_trigger_ ? on_trigger_->get() : 0,
            flag_ ? (int)*flag_ : 0
        );
        auto menu = (Fl_Menu_Item *)w->menu(); // NOLINT
        if (labelsize_)
            menu[i].labelsize(*labelsize_);
    }
};

template <class Message, class B>
struct MenuProps {
    std::vector<MenuItem<Message>> items;
    void view(B *w) {
        if (!items.empty()) {
            for (const auto &i : items) {
                i.view(w);
            }
        }
    }
    void update(B *w, const MenuProps &other) {
        if (*this == other)
            return;
        if (other.items != items) {
            w->clear();
            items = other.items;
            for (auto i = 0; i < items.size(); i++) {
                items[i].update(w, other.items[i]);
            }
        }
    }
    bool operator==(const MenuProps &) const = default;
};

template <class Message, class W, class B>
class MenuBase : public WidgetBase<Message, W, B> {
  protected:
    MenuProps<Message, B> mprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->mprops.view(this->inner);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);
        this->mprops.update(this->inner, f->mprops);
    }
    virtual ~MenuBase() = default;

    W &items(std::initializer_list<MenuItem<Message>> items) {
        mprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
    W &items(std::span<MenuItem<Message>> items) {
        mprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
};

#define MENU(Class, Base)                                                      \
    template <class Message>                                                   \
    class Class : public MenuBase<Message, Class<Message>, Base> {};

MENU(MenuBar, Fl_Menu_Bar)
MENU(SysMenuBar, Fl_Sys_Menu_Bar)
MENU(Choice, Fl_Choice)
} // namespace rf::detail
