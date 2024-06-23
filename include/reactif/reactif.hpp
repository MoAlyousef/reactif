#pragma once

#include "widgets.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <memory>
#include <string>
#include <utility>

#define TRIGGER(x) [=, this] { return x; }

namespace rf {

struct Settings {
    /// The main window's position
    std::pair<int, int> pos = std::pair(0, 0);
    /// The main window's size
    std::pair<int, int> size = std::pair(400, 300); // NOLINT
    /// Free the window's position
    bool force_position = false;
    /// Set whether the main window is resizable
    bool resizable = true;
    /// Tells the main window to ignore the esc key
    bool ignore_esc_close = false;
    /// Set the applications default font size
    int font_size = FL_NORMAL_SIZE;
    /// Set the background color
    std::optional<std::tuple<unsigned char, unsigned char, unsigned char>>
        background;
    /// Set the foreground color
    std::optional<std::tuple<unsigned char, unsigned char, unsigned char>>
        foreground;
    /// Set the background2 color
    std::optional<std::tuple<unsigned char, unsigned char, unsigned char>>
        background2;
    /// Set the inactive color
    std::optional<std::tuple<unsigned char, unsigned char, unsigned char>>
        inactive;
    /// Set the selection color
    std::optional<std::tuple<unsigned char, unsigned char, unsigned char>>
        selection;
    /// Set the default font
    std::optional<int> font;
    /// Set whether visible focus is active
    std::optional<bool> visible_focus;
    /// Set the default scheme
    std::optional<Scheme> scheme;
    /// Set the window's size range
    std::optional<std::tuple<int, int, int, int>> size_range;
};

/// The default Application object
template <class Message>
class Application : public detail::DefaultWidgets<Message> {
    Settings settings_ = {};

  public:
    Application(Settings &&settings) : settings_(std::move(settings)) {}
    virtual ~Application() = default;
    /// Set the application's title
    [[nodiscard]] virtual std::string title() const = 0;
    /// Set the view of the application
    virtual std::shared_ptr<Widget<Message>> view() = 0;
    /// Handle updates
    virtual void update(const Message &msg) = 0;
    /// Run the application
    void run(int argc, char **argv) {
        fl_define_FL_ROUND_UP_BOX();
        fl_define_FL_SHADOW_BOX();
        fl_define_FL_ROUNDED_BOX();
        fl_define_FL_RFLAT_BOX();
        fl_define_FL_RSHADOW_BOX();
        fl_define_FL_DIAMOND_BOX();
        fl_define_FL_OVAL_BOX();
        fl_define_FL_PLASTIC_UP_BOX();
        fl_define_FL_GTK_UP_BOX();
        fl_define_FL_GLEAM_UP_BOX();
        fl_define_FL_SHADOW_LABEL();
        fl_define_FL_ENGRAVED_LABEL();
        fl_define_FL_EMBOSSED_LABEL();
        fl_define_FL_MULTI_LABEL();
        fl_define_FL_ICON_LABEL();
        fl_define_FL_IMAGE_LABEL();
        Fl::use_high_res_GL(1);
        auto widget = view();
        if (settings_.scheme)
            Fl::scheme(settings_.scheme->c_str());
        else
            Fl::scheme("gtk+");
        if (settings_.background) {
            auto [r, g, b] = *settings_.background;
            Fl::background(r, g, b);
        }
        if (settings_.background2) {
            auto [r, g, b] = *settings_.background2;
            Fl::background2(r, g, b);
        }
        if (settings_.foreground) {
            auto [r, g, b] = *settings_.foreground;
            Fl::foreground(r, g, b);
        }
        if (settings_.inactive) {
            auto [r, g, b] = *settings_.inactive;
            Fl::set_color(FL_INACTIVE_COLOR, r, g, b);
        }
        if (settings_.selection) {
            auto [r, g, b] = *settings_.selection;
            Fl::set_color(FL_SELECTION_COLOR, r, g, b);
        }
        if (settings_.visible_focus)
            Fl::visible_focus(*settings_.visible_focus);
        auto [x, y] = settings_.pos;
        auto [w, h] = settings_.size;
        auto *win   = new Fl_Double_Window(x, y, w, h); // NOLINT
        if (!settings_.force_position)
            win->free_position();
        win->copy_label(title().c_str());
        win->default_xclass(title().c_str());
        if (settings_.font_size)
            FL_NORMAL_SIZE = settings_.font_size;
        if (settings_.font)
            Fl::set_font(FL_HELVETICA, *settings_.font);
        if (widget) {
            auto *wid = widget->view();
            wid->resize(0, 0, w, h);
            if (settings_.resizable)
                win->resizable(wid);
        }
        win->end();
        if (settings_.size_range) {
            auto [x, y, w, h] = *settings_.size_range;
            win->size_range(x, y, w, h);
        }
        win->show(argc, argv);
        if (settings_.ignore_esc_close) {
            win->callback([](Fl_Widget *w) {
                if (Fl::event() == FL_CLOSE)
                    w->hide();
            });
        }
        Fl::lock();
        while (Fl::wait()) {
            auto msg = Fl::thread_message();
            if (msg) {
                auto msg1 = *static_cast<std::function<Message()> *>(msg);
                update(msg1());
                auto widget2 = view();
                if (widget && widget2) {
                    if (typeid(*widget) == typeid(*widget2))
                        widget->update(widget2.get());
                    else {
                        widget = widget2;
                        win->clear();
                        win->begin();
                        auto *wid = widget->view();
                        wid->resize(0, 0, w, h);
                        if (settings_.resizable)
                            win->resizable(wid);
                        win->end();
                        win->redraw();
                    }
                }
            }
        }
    }
};
} // namespace rf
