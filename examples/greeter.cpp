#include <reactif/reactif.hpp>

using namespace rf;

struct Message {
    std::string value;
    static Message name(std::string_view s) { return {std::string(s)}; }
};

class MyApplication : public Application<Message> {
    std::string message;
    std::shared_ptr<std::string> inval = std::make_shared<std::string>();

  public:
    MyApplication(Settings &&settings) : Application(std::move(settings)) {}
    std::string title() const override { return "Greeter"; }
    void update(const Message &m) override {
        if (!m.value.empty())
            message = std::string("Hello ") + m.value;
    }
    std::shared_ptr<Widget<Message>> view() override {
        return flex()
            .column()
            .margins(40, 20, 40, 20)
            .children({
                box().label("Enter name!").create(),
                input()
                    .fixed(40)
                    .value(inval)
                    .on_trigger(
                        TRIGGER(Message::name(*inval))
                    )
                    .create(),
                box().label(message).create(),
                button()
                    .label("Greet")
                    .fixed(40)
                    .on_trigger(
                        TRIGGER(Message::name(*inval))
                    )
                    .create(),
            })
            .create();
    }
};

int main(int argc, char **argv) {
    MyApplication app({ .size = std::pair(400, 300) });
    app.run(argc, argv);
}