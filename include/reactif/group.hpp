#pragma once

#include "widget.hpp"
#include <FL/Fl_Flex.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Tile.H>
#include <span>

#include <iostream>

namespace rf::detail {

template <class Message, class B>
struct GroupProps {
    std::vector<std::shared_ptr<Widget<Message>>> children;
    std::optional<int> fill;
    void view(B *w) {
        if (fill) {
            static_cast<FlWidgetWrapper<B> *>(w)->resize_cb =
                [this](FlWidgetWrapper<B> *wid, int x, int y, int w, int h) {
                    wid->as_group()->child(*fill)->resize(x, y, w, h);
                };
            w->resizable(w->child(*fill));
        }
        for (auto c : children)
            c->view();
    }
    void update(B *w, const GroupProps &other) {
        if (*this == other)
            return;
        if (other.children != children) {
            auto old_size = children.size();
            auto new_size = other.children.size();
            for (auto i = 0; i < std::min(old_size, new_size); i++) {
                if (typeid(*children[i]) == typeid(*other.children[i]))
                    children[i]->update(other.children[i].get());
                else {
                    children[i] = other.children[i];
                    auto *c      = w->child(i);
                    w->remove(i);
                    delete c; // NOLINT
                    w->insert(*children[i]->view(), i);
                }
            }
            if (new_size > old_size) {
                children.resize(new_size);
                for (auto i = old_size; i < new_size; i++) {
                    children[i] = other.children[i];
                    w->add(*children[i]->view());
                }
            }
            if (new_size < old_size) {
                for (auto i = new_size; i < old_size; i++) {
                    if (w->children() > 0) {
                        auto *c = w->child(i);
                        w->remove(i);
                        delete c; // NOLINT
                    }
                }
                children.resize(new_size);
            }
            Fl::redraw();
        }
    }
    bool operator==(const GroupProps &) const = default;
};

template <class Message, class W, class B>
class GroupBase : public WidgetBase<Message, W, B> {
  protected:
    GroupProps<Message, B> gprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        gprops.view(this->inner);
        this->inner->end();
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);

        gprops.update(this->inner, f->gprops);
    }
    virtual ~GroupBase() = default;

    W &children(std::initializer_list<std::shared_ptr<Widget<Message>>> children
    ) {
        gprops.children.assign(children.begin(), children.end());
        return *(W *)this;
    }
    W &children(std::span<std::shared_ptr<Widget<Message>>> children) {
        gprops.children.assign(children.begin(), children.end());
        return *(W *)this;
    }
    /// Set which child fills the group
    W &fill(int child) {
        gprops.fill = child;
        return *(W *)this;
    }
};

template <class Message>
class Flex : public GroupBase<Message, Flex<Message>, Fl_Flex> {
    std::tuple<int, int, int, int> margins_ = std::make_tuple(0, 0, 0, 0);
    Fl_Widget *view() override {
        GroupBase<Message, Flex<Message>, Fl_Flex>::view();
        auto [l, t, r, b] = margins_;
        this->inner->margin(l, t, r, b);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        GroupBase<Message, Flex<Message>, Fl_Flex>::update(other);
        auto f = (Flex *)other;
        if (margins_ != f->margins_) {
            margins_          = f->margins_;
            auto [l, t, r, b] = margins_;
            this->inner->margin(l, t, r, b);
        }
    }

  public:
    /// Set whether the Flex is a column
    Flex &column() {
        this->wprops.subtype = 0;
        return *this;
    }
    /// Set whether the Flex is a row
    Flex &row() {
        this->wprops.subtype = 1;
        return *this;
    }
    /// Set the flex's margins
    Flex &margins(int l, int t, int r, int b) {
        margins_ = std::make_tuple(l, t, r, b);
        return *this;
    }
    /// Set the flex's margins
    Flex &margins(int margin) {
        margins_ = std::make_tuple(margin, margin, margin, margin);
        return *this;
    }
};

template <class Message>
class Pack : public GroupBase<Message, Pack<Message>, Fl_Pack> {
    int spacing_ = 0;
    Fl_Widget *view() override {
        GroupBase<Message, Pack<Message>, Fl_Pack>::view();
        this->inner->spacing(spacing_);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        GroupBase<Message, Pack<Message>, Fl_Pack>::update(other);
        auto f = (Pack *)other;
        if (spacing_ != f->spacing_) {
            spacing_ = f->spacing_;
            this->inner->spacing(spacing_);
        }
    }

  public:
    /// Set whether the pack is vertical
    Pack &vertical() {
        this->wprops.subtype = 0;
        return *this;
    }
    /// Set whether the pack is horizontal
    Pack &horizontal() {
        this->wprops.subtype = 1;
        return *this;
    }
    /// Set the pack's spacing
    Pack &spacing(int l) {
        spacing_ = l;
        return *this;
    }
};

#define GROUP(Class, Base)                                                     \
    template <class Message>                                                   \
    class Class : public GroupBase<Message, Class<Message>, Base> {};

GROUP(Scroll, Fl_Scroll)
GROUP(Tabs, Fl_Tabs)
GROUP(Tile, Fl_Tile)
GROUP(Group, Fl_Group)
} // namespace rf::detail
