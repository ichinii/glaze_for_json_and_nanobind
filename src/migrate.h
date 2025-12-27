#pragma once

#include <string>
#include <print>

#include <glaze/glaze.hpp>

// ----------------------------------------------------------------------------
// migration to a new version is another story
// ----------------------------------------------------------------------------

struct Version1 {
    int unchanged = 1;
    int changed_default_value = 2;
    int renamed = 3;
    int some_var = 4;
    int obsolete = 5;
    int reshaped = 7;
};

struct Version2 {
    int unchanged = 1;
    int changed_default_value = 42;
    int renamed_renamed = 3;
    std::string changed_type = "4";
    // obsolete field is removed
    int new_field = 6;

    struct Reshaped {
        int value = 7;
        int extra = 8;
    };
    Reshaped reshaped;
};

using CurrentVersion = Version2;

CurrentVersion migrate(Version2 old_version) {
    std::println("Migrating from Version2 to CurrentVersion (also Version2)");

    return old_version; // already current
}

CurrentVersion migrate(const Version1& old_version) {
    std::println("Migrating from Version1 to CurrentVersion (Version2)");

    CurrentVersion new_version;

    new_version.unchanged = old_version.unchanged;                         // unchanged field
    new_version.changed_default_value = old_version.changed_default_value; // changed default value does not matter, because, when reading json. the values comes from json anyway
    new_version.renamed_renamed = old_version.renamed;                     // renamed field
    new_version.changed_type = std::to_string(old_version.some_var);       // changed type from int to string
    // new_version.new_field                                               // new field with default value
    // obsolete                                                            // obsolete field is simply not copied
    new_version.reshaped.value = old_version.reshaped;                     // reshaped field

    return migrate(new_version); // in case there are more versions to migrate through
}

// ----------------------------------------------------------------------------
// envelope to hold different versions
// changes to this struct should be backward compatible
// ----------------------------------------------------------------------------
struct Envelope {
    struct Format {
        struct Version {
            int major = 1;
            int minor = 0;
            int patch = 0;
        } version;
    };

    // parse header to distinguish version
    Format format;

    // keep the rest as raw json / toml / or other formats, even binary
    // this is the glaze way to say "I don't care about the content here, now, but keep it safe"
    glz::raw_json payload;
};
