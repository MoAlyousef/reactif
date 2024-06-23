#include <reactif/reactif.hpp>

using namespace rf;

enum class Message {
    Increment,
    Decrement,
};

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
        case Message::Decrement:
            value -= 1;
            break;
        }
    }
    std::shared_ptr<Widget<Message>> view() override {
        return flex()
            .column()
            .margins(40)
            .children({
                button()
                    .label("Increment")
                    .fixed(40)
                    .on_trigger(TRIGGER(Message::Increment))
                    .create(),
                box()
                    .label(std::to_string(value))
                    .labeltype(LabelType::Engraved)
                    .labelsize(20)
                    .create(),
                button()
                    .label("Decrement")
                    .fixed(40)
                    .on_trigger(TRIGGER(Message::Decrement))
                    .create(),
            })
            .create();
    }
};

int main(int argc, char **argv) {
    MyApplication app({ .size = std::pair(400, 300) });
    app.run(argc, argv);
}