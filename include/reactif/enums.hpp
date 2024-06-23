#pragma once

#include <cstdint>

namespace rf {

struct Scheme {
    enum Predef {
        Base,
        Oxy,
        Gtk,
        Gleam,
        Plastic,
    };
    Predef p_;
    Scheme(Predef p) : p_(p) {}
    const char *c_str() {
        switch (p_) {
        case Base:
            return "base";
        case Oxy:
            return "oxy";
        case Gtk:
            return "gtk+";
        case Gleam:
            return "gleam";
        case Plastic:
            return "plastic";
        default:
            return nullptr;
        }
    }
};

class Color {
    uint32_t val;

  public:
    enum Predef : uint32_t {
        foreground  = 0,
        Background2 = 7,
        Inactive    = 8,
        Selection   = 15,
        Gray0       = 32,
        Dark3       = 39,
        Dark2       = 45,
        Dark1       = 47,
        Background  = 49,
        Light1      = 50,
        Light2      = 52,
        Light3      = 54,
        Black       = 56,
        Red         = 88,
        Green       = 63,
        Yellow      = 95,
        Blue        = 216,
        Magenta     = 248,
        Cyan        = 223,
        DarkRed     = 72,
        DarkGreen   = 60,
        DarkYellow  = 76,
        DarkBlue    = 136,
        DarkMagenta = 152,
        DarkCyan    = 140,
        White       = 255,
    };
    Color(Predef p) : val(static_cast<uint32_t>(p)) {}
    Color(uint32_t p) : val(p) {}
    operator uint32_t() const { return val; }
};

class Font {
    int val;

  public:
    enum Predef {
        Helvetica = 0,
        HelveticaBold,
        HelveticaItalic,
        HelveticaBoldItalic,
        Courier,
        CourierBold,
        CourierItalic,
        CourierBoldItalic,
        Times,
        TimesBold,
        TimesItalic,
        TimesBoldItalic,
        Symbol,
        Screen,
        ScreenBold,
        Zapfdingbats,
    };
    Font(Predef p) : val(p) {}
    Font(int p) : val(p) {}
    operator int() const { return val; }
};

class When {
    int val;

  public:
    enum Predef {
        Never           = 0,
        Changed         = 1,
        NotChanged      = 2,
        Release         = 4,
        ReleaseAlways   = 6,
        EnterKey        = 8,
        EnterKeyAlways  = 10,
        EnterKeyChanged = 11,
    };
    When(Predef p) : val(p) {}
    When(int p) : val(p) {}
    operator int() const { return val; }
};

class Align {
    int val;

  public:
    enum Predef {
        Center          = 0,
        Top             = 1,
        Bottom          = 2,
        Left            = 4,
        Right           = 8,
        Inside          = 16,
        TextOverImage   = 20,
        Clip            = 40,
        Wrap            = 80,
        ImageNextToText = 100,
        TextNextToImage = 120,
        ImageBackdrop   = 200,
        TopLeft         = 1 | 4,
        TopRight        = 1 | 8,
        BottomLeft      = 2 | 4,
        BottomRight     = 2 | 8,
        LeftTop         = 7,
        RightTop        = 11,
        LeftBottom      = 13,
        RightBottom     = 14,
        PositionMask    = 15,
        ImageMask       = 320,
    };
    Align(Predef p) : val(p) {}
    Align(int p) : val(p) {}
    operator int() const { return val; }
};

class Shortcut {
    int val;

  public:
    enum Predef {
        None     = 0,
        Shift    = 0x00010000,
        CapsLock = 0x00020000,
        Ctrl     = 0x00040000,
        Alt      = 0x00080000,
    };
    Shortcut(Predef p) : val(p) {}
    Shortcut(int p) : val(p) {}
    operator int() const { return val; }
};

enum class BoxType {
    None = 0,
    Flat,
    Up,
    Down,
    UpFrame,
    DownFrame,
    ThinUp,
    ThinDown,
    ThinUpFrame,
    ThinDownFrame,
    Engraved,
    Embossed,
    EngravedFrame,
    EmbossedFrame,
    Border,
    Shadow,
    BorderFrame,
    ShadowFrame,
    Rounded,
    Rshadow,
    RoundedFrame,
    Rflat,
    RoundUp,
    RoundDown,
    DiamandUp,
    DiamondDown,
    Oval,
    Oshadow,
    OvalFrame,
    Oflat,
    PlasticUp,
    PlasticDown,
    PlasticUpFrame,
    PlasticDownFrame,
    PlasticThinUp,
    PlasticThinDown,
    PlasticRoundUp,
    PlsaticRoundDown,
    GtkUp,
    GtkDown,
    GtkUpFrame,
    GtkDownFrame,
    GtkThinUp,
    GtkThinDownBox,
    GtkThinUpFrame,
    GtkThinDownFrame,
    GtkRoundUpFrame,
    GtkRoundDownFrame,
    GleamUpBox,
    GleamDownBox,
    GleamUpFrame,
    GleamDownFrame,
    GleamThinUpBox,
    GleamThinDownBox,
    GleamRoundUpBox,
    GleamRoundDownBox,
    Free,
};

enum class LabelType {
    Normal = 0,
    None,
    Shadow,
    Engraved,
    Embossed,
    Multi,
    Icon,
    Image,
    Free,
};

class MenuFlag {
    int val;

  public:
    enum Predef {
        Normal         = 0,
        Inactive       = 1,
        Toggle         = 2,
        Value          = 4,
        Radio          = 8,
        Invisible      = 0x10,
        SubmenuPointer = 0x20,
        Submenu        = 0x40,
        MenuDivider    = 0x80,
        MenuHorizontal = 0x100,
    };
    MenuFlag(Predef p) : val(p) {}
    MenuFlag(int p) : val(p) {}
    operator int() const { return val; }
};

} // namespace rf
