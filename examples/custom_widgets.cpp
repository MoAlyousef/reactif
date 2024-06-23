#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <reactif/reactif.hpp>
#include <FL/Fl_Box.H>

/// This example shows two ways of wrapping a widget derived from an FLTK widget

class MyBox1: public Fl_Box {
    public:
    MyBox1(int x, int y, int w, int h, const char *l = nullptr): Fl_Box(x, y, w, h, l) {
        box(FL_FLAT_BOX);
        color(FL_BLACK);
        labelcolor(FL_WHITE);
        callback([](auto *w){ puts("Pushed!"); });
    }
    int handle(int ev) override {
        int ret = Fl_Box::handle(ev);
        switch(ev) {
            case FL_PUSH: do_callback(); return 1;
            default: return ret;
        }
    }
    void draw() override {
        Fl_Box::draw();
        fl_color(FL_YELLOW);
        fl_font(FL_TIMES_BOLD, 30);
        fl_draw("Click me!", x(), y(), w(), h(), FL_ALIGN_CENTER);
    }
};

class MyBox2: public Fl_Box {
    public:
    MyBox2(int x, int y, int w, int h, const char *l = nullptr): Fl_Box(x, y, w, h, l) {
        box(FL_FLAT_BOX);
        color(FL_WHITE);
        callback([](auto *w){ puts("Pushed!"); });
    }
    int handle(int ev) override {
        int ret = Fl_Box::handle(ev);
        switch(ev) {
            case FL_PUSH: do_callback(); return 1;
            default: return ret;
        }
    }
    void draw() override {
        Fl_Box::draw();
        fl_color(FL_BLUE);
        fl_font(FL_TIMES_BOLD, 30);
        fl_draw("Click me!", x(), y(), w(), h(), FL_ALIGN_CENTER);
    }
};


using namespace rf;

/// First is to derive from detail::WidgetBase
template<class Message>
class MyBox1Wrapper: public detail::WidgetBase<Message, MyBox1Wrapper<Message>, MyBox1> {};

/// Or the more manual derive from Widget<Message>
template<class Message>
class MyBox2Wrapper: public Widget<Message> {
    MyBox2 *inner      = nullptr;
    public:
    Fl_Widget *view() override {
        this->inner = new MyBox2(0, 0, 0, 0); // NOLINT
        // set props
        return inner;
    }
    void update(Widget<Message> *other) override {
        auto f = (MyBox2Wrapper *)other;
        // update props
    }
    std::shared_ptr<Widget<Message>> create() override {
        return std::shared_ptr<Widget<Message>>(new MyBox2Wrapper(*this));
    }
};

using Message = int;

class MyApplication : public Application<Message> {
    int value = 0;
    MyBox1Wrapper<Message> mybox1() {
        return {};
    }
    MyBox2Wrapper<Message> mybox2() {
        return {};
    }
  public:
    MyApplication(Settings &&settings) : Application(std::move(settings)) {}
    std::string title() const override { return "Derived"; }
    void update(const Message &m) override {}
    std::shared_ptr<Widget<Message>> view() override {
        return flex()
            .column()
            .margins(40)
            .children({
                mybox1().create(),
                mybox2().create(),
            })
            .create();
    }
};

int main(int argc, char **argv) {
    MyApplication app({ .size = std::pair(400, 300) });
    app.run(argc, argv);
}