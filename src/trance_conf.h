
#include <Arduino.h>

enum SettingType {
    STRING,
    INT,
    BOOL_CHECK,
    BOOL_RADIO
};

struct Setting {
    const char* name;
    SettingType type;
    String value;

    Setting(const char* name, SettingType type, String value)
        : name(name), type(type), value(value) {}
};

struct Section {
    const char* name;
    void (*callback)();
    int settings_count;
    
    Setting* settings;

    Section(const char* name, void (*callback)(), int settings_count, Setting* settings)
        : name(name), callback(callback), settings_count(settings_count), settings(settings) {}
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

extern Configuration_ TranceConf;