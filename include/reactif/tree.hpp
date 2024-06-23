#pragma once

#include "widget.hpp"
#include <FL/Fl_Tree.H>
#include <span>

namespace rf::detail {
template <class Message>
class TreeItem {
    std::string label_;
    int labelsize = FL_NORMAL_SIZE;

  public:
    /// Create a tree item with a label
    TreeItem(std::string_view label) : label_(std::string(label)) {}
    bool operator==(const TreeItem &) const = default;
    void view(Fl_Tree *m) const {
        auto i = m->add(label_.c_str());
        i->labelsize(labelsize);
    }
    void update(Fl_Tree *w, const TreeItem &other) {
        auto i = w->add(label_.c_str());
        i->labelsize(labelsize);
    }
};

template <class Message, class B>
struct TreeProps {
    std::vector<TreeItem<Message>> items;
    std::optional<std::string> root_label;
    void view(B *w) {
        if (root_label)
            w->root_label(*root_label);
        if (!items.empty()) {
            for (const auto &i : items) {
                i.view(w);
            }
        }
    }
    void update(B *w, const TreeProps &other) {
        if (*this == other)
            return;
        if (other.root_label != root_label) {
            root_label = other.root_label;
            if (root_label)
                w->root_label(*root_label);
        }
        if (other.items != items) {
            w->clear();
            items = other.items;
            for (auto i = 0; i < items.size(); i++) {
                items[i].update(w, other.items[i]);
            }
        }
    }
    bool operator==(const TreeProps &) const = default;
};

template <class Message, class W, class B>
class TreeBase : public WidgetBase<Message, W, B> {
  protected:
    TreeProps<Message, B> tprops = {};

  public:
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new W(*(W *)this));
    }
    Fl_Widget *view() override {
        WidgetBase<Message, W, B>::view();
        this->tprops.view(this->inner);
        return this->inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);
        this->tprops.update(this->inner, f->tprops);
    }
    virtual ~TreeBase() = default;

    W &items(std::initializer_list<TreeItem<Message>> items) {
        tprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
    W &items(std::span<TreeItem<Message>> items) {
        tprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
    /// Sets the root label
    W &root_label(std::string_view label) {
        tprops.root_label = std::string(label);
        return *(W *)this;
    }
};

#define TREE(Class, Base)                                                      \
    template <class Message>                                                   \
    class Class : public TreeBase<Message, Class<Message>, Base> {};

TREE(Tree, Fl_Tree)
} // namespace rf::detail
