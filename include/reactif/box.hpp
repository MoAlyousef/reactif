#pragma once

#include "widget.hpp"
#include <FL/Fl_Box.H>

namespace rf::detail {
template <class Message>
class Box : public WidgetBase<Message, Box<Message>, Fl_Box> {};
} // namespace rf::detail
