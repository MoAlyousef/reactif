#include <reactif/reactif.hpp>

using namespace rf;

struct Message {
    enum { NewTask, RemoveTask } op;
    std::string value;
    static Message add_task(std::string val) { return {NewTask, val}; }
    static Message remove_task(std::string val) { return {RemoveTask, val}; }
};

class MyApplication : public Application<Message> {
    int value                          = 0;
    std::shared_ptr<std::string> inval = std::make_shared<std::string>();
    std::vector<std::string> tasks;

  public:
    MyApplication(Settings &&settings) : Application(std::move(settings)) {}
    std::string title() const override { return "TodoApp"; }
    void update(const Message &m) override {
        auto val = m.value;
        if (val.empty())
            return;
        switch (m.op) {
        case Message::NewTask:
            tasks.push_back(val);
            break;
        case Message::RemoveTask: {
            auto idx = std::find(tasks.begin(), tasks.end(), val);
            tasks.erase(idx);
            break;
        }
        }
    }
    std::shared_ptr<Widget<Message>> view() override {
        auto p     = pack().vertical();
        auto boxes = std::vector<std::shared_ptr<Widget<Message>>>();
        for (const auto &t : tasks) {
            auto f = flex()
                         .row()
                         .size(0, 30)
                         .children({
                             box()
                                 .label(t)
                                 .align(Align::Left | Align::Inside)
                                 .create(),
                             check_button()
                                 .fixed(30)
                                 .align(Align::Left | Align::Inside)
                                 .value(true)
                                 .on_trigger(TRIGGER(Message::remove_task(t)))
                                 .create(),
                         })
                         .create();
            boxes.push_back(f);
        }
        p.children(boxes);
        return flex()
            .column()
            .margins(30, 20, 30, 20)
            .children({
                flex()
                    .row()
                    .fixed(30)
                    .children({
                        input().value(inval).on_trigger(TRIGGER(Message::add_task(*inval))).create(),
                        button()
                            .label("@>")
                            .fixed(30)
                            .on_trigger(TRIGGER(Message::add_task(*inval)))
                            .create(),
                    })
                    .create(),
                scroll().fill(0).children({p.create()}).create(),
            })
            .create();
    }
};

int main(int argc, char **argv) {
    MyApplication app({ .size = std::pair(400, 300) });
    app.run(argc, argv);
}