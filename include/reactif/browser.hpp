#pragma once

#include "widget.hpp"
#include <FL/Fl_Browser.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Select_Browser.H>
#include <span>

namespace rf::detail {

template <class Message>
class BrowserItem {
    std::string label_;
    int width_ = 50; // NOLINT

  public:
    /// Create a browser item with a label
    BrowserItem(std::string_view label) : label_(std::string(label)) {}
    /// Set the width of the browser item
    BrowserItem &width(int w) {
        width_ = w;
        return *this;
    }
    /// Get the width of the item
    [[nodiscard]] int width() const { return width_; }
    bool operator==(const BrowserItem &) const = default;
    void view(Fl_Browser *m) const { m->add(label_.c_str()); }
    void update(Fl_Browser *w, const BrowserItem &other) {
        w->add(label_.c_str());
    }
};

template <class Message, class B>
struct BrowserProps {
    std::vector<BrowserItem<Message>> items;
    char column_char = '\t';
    int textsize     = FL_NORMAL_SIZE;
    std::optional<int> select;
    std::optional<int> topline;
    std::optional<int> middleline;
    std::optional<int> bottomline;
    std::vector<int> widths;
    void view(B *w) {
        if (!items.empty()) {
            for (const auto &i : items) {
                i.view(w);
                widths.push_back(i.width());
            }
            widths.push_back(0);
            w->column_widths(widths.data());
            w->textsize(textsize);
            w->column_char(column_char);
            if (select)
                w->select(*select);
            if (topline)
                w->topline(*topline);
            if (middleline)
                w->middleline(*middleline);
            if (bottomline)
                w->bottomline(*bottomline);
        }
    }
    void update(B *w, const BrowserProps &other) {
        if (*this == other)
            return;
        if (other.items != items) {
            w->clear();
            items = other.items;
            for (auto i = 0; i < items.size(); i++) {
                items[i].update(w, other.items[i]);
                widths.push_back(items[i].width());
            }
            widths.push_back(0);
            w->column_widths(widths.data());
        }
        if (other.textsize != textsize) {
            textsize = other.textsize;
            w->textsize(textsize);
        }
        if (other.column_char != column_char) {
            column_char = other.column_char;
            w->column_char(column_char);
        }
        if (other.select != select) {
            if (select) {
                if (w->selected(*select))
                    w->select(*select, 0);
            }
            select = other.select;
            if (select)
                w->select(*select);
        }
        if (other.topline != topline) {
            topline = other.topline;
            if (topline)
                w->topline(*topline);
        }
        if (other.middleline != middleline) {
            middleline = other.middleline;
            if (middleline)
                w->middleline(*middleline);
        }
        if (other.bottomline != bottomline) {
            bottomline = other.bottomline;
            if (bottomline)
                w->bottomline(*bottomline);
        }
    }
    bool operator==(const BrowserProps &) const = default;
};

template <class Message, class W, class B>
class BrowserBase : public WidgetBase<Message, W, B> {
  protected:
    BrowserProps<Message, B> bprops = {};

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
        auto f = (W *)other;
        WidgetBase<Message, W, B>::update(other);
        this->bprops.update(this->inner, f->bprops);
    }
    virtual ~BrowserBase() = default;

    W &items(std::initializer_list<BrowserItem<Message>> items) {
        bprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
    W &items(std::span<BrowserItem<Message>> items) {
        bprops.items.assign(items.begin(), items.end());
        return *(W *)this;
    }
    /// Set the column char
    W &column_char(char c) {
        bprops.column_char = c;
        return *(W *)this;
    }
    /// Set the text size
    W &textsize(int c) {
        bprops.textsize = c;
        return *(W *)this;
    }
};

#define BROWSER(Class, Base)                                                   \
    template <class Message>                                                   \
    class Class : public BrowserBase<Message, Class<Message>, Base> {};

BROWSER(Browser, Fl_Browser)
BROWSER(HoldBrowser, Fl_Hold_Browser)
BROWSER(SelectBrowser, Fl_Select_Browser)
BROWSER(MultiBrowser, Fl_Multi_Browser)
BROWSER(FileBrowser, Fl_File_Browser)
} // namespace rf::detail
