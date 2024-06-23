# Reactif

A header-only library providing a reactive layer on top of FLTK.

## Example
```c++
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
```

```c++
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
```

## Requirements
- C++20 compiler.
- FLTK 1.4.
- CMake 3.20 if you want to build/install the lib, build the examples or use the library via CMake's fetchContent or add_subdirectory.

## Building locally
```
git clone https://github.com/MoAlyousef/reactif
cd reactif
cmake -S . -B bin
cmake --build bin
```
This will build the examples automatically.

## Usage
There are several options:

1- Via CMake's fetchContent:
```cmake
include(FetchContent)

FetchContent_Declare(
  reactif
  GIT_REPOSITORY https://github.com/MoAlyouse/reactif
  GIT_TAG        main
  GIT_SHALLOW    True
)

FetchContent_MakeAvailable(reactif)

# then
target_link_libraries(your_exe PRIVATE reactif::reactif)
```

2- Via vendoring the headers into your project.
This can be done by cloning the repo someplace or into your project (or via git submodule).
Then point your compiler to their location (manually or via CMake's target_include_directories).

3- Installing the library locally or system-wide.
To install locally, pass a CMAKE_INSTALL_PREFIX when configuring CMake:
```
cmake -Bbin -DCMAKE_INSTALL_PREFIX=some/path
cmake --build bin
cmake --install bin
```
If no prefix is specified, CMake will proceed with a system-wide install.