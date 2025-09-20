
#include <Arduino.h>

enum SettingType {
    STRING,
    INT,
    BOOL_CHECK,
    BOOL_RADIO
};

struct Setting {
    String name;
    SettingType type;
    String value;

    Setting(String name, SettingType type, String value)
        : name(name), type(type), value(value) {}
};

struct Section {
    String name;
    void (*callback)(Section*);
    
    Setting* settings;

    Section(String name, void (*callback)(Section*), Setting* settings)
        : name(name), callback(callback), settings(settings) {}
};


class Configuration_ {
    public:
        Configuration_();
        void begin();
        void handle();
        void register_section(Section* section);
        void remove_section(Section* section);
    private:
        Section* _sections[32];
};

extern Configuration_ Configuration;