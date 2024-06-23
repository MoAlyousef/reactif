#include <reactif/reactif.hpp>

using namespace rf;

enum class Message {
    Increment,
};

const Color BLUE     = Color{0x42A5F500};
const Color SEL_BLUE = Color{0x3f51b500};
const Color GRAY     = Color{0x75757500};

class MyApplication : public Application<Message> {
    int value = 0;

  public:
    MyApplication(Settings &&settings) : Application(std::move(settings)) {}
    std::string title() const override { return "Counter"; }
    void update(const Message &m) override {
        switch (m) {
        case Message::Increment:
            value += 1;
            break;
        }
    }
    std::shared_ptr<Widget<Message>> view() override {
        return flex()
            .column()
            .children({
                box()
                    .label("    FLTK App!")
                    .align(Align::Left | Align::Inside)
                    .labelcolor(Color::White)
                    .labelsize(22)
                    .box(BoxType::Flat)
                    .color(BLUE)
                    .fixed(60)
                    .create(),
                box()
                    .label("You have pushed the button this many times:")
                    .align(Align::Bottom | Align::Inside)
                    .labelsize(18)
                    .labelfont(Font::Times)
                    .create(),
                box()
                    .label(std::to_string(value))
                    .align(Align::Top | Align::Inside)
                    .labelsize(36)
                    .labelcolor(GRAY)
                    .create(),
                flex()
                    .row()
                    .fixed(60)
                    .children({
                        box().create(),
                        button()
                            .label("@+6plus")
                            .labelcolor(Color::White)
                            .box(BoxType::Oflat)
                            .color(BLUE)
                            .selection_color(SEL_BLUE)
                            .on_trigger(TRIGGER(Message::Increment))
                            .fixed(60)
                            .create(),
                        box().fixed(20).create(),
                    })
                    .create(),
                box().fixed(20).create(),
            })
            .create();
    }
};

int main(int argc, char **argv) {
    Settings settings{
        .size          = std::pair(600, 400),
        .background    = std::tuple(255, 255, 255),
        .visible_focus = false,
    };
    MyApplication app(std::move(settings));
    app.run(argc, argv);
}