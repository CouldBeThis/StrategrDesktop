#ifndef STRATEGR_ACTIVITY_H
#define STRATEGR_ACTIVITY_H

#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <utility>

#include "color.h"

namespace stg {
    struct activity {
        using color_t = stg::color;
        using name_t = std::string;
        using color_info_t = std::pair<color_t, std::string>;

        class invalid_property_exception;

        static constexpr auto default_color = "#000000";
        static const std::vector<color_info_t> &default_colors();

        explicit activity(name_t name, color_t color = default_color) noexcept(false);

        const name_t &name() const;
        const color_t &color() const;

        activity copy_changing_name(const name_t &name) const;
        activity copy_changing_color(const color_t &color) const;

        friend bool operator==(const activity &lhs, const activity &rhs);
        friend bool operator!=(const activity &lhs, const activity &rhs);

        friend std::ostream &operator<<(std::ostream &os,
                                        const activity &activity);

        nlohmann::json to_json();
        static activity from_json(const nlohmann::json &j);

    private:
        name_t _name;
        color_t _color;

        struct keys {
            static constexpr auto name = "name";
            static constexpr auto color = "color";
        };

        static invalid_property_exception empty_name_exception();

        static bool is_valid(const name_t &name);
    };
}

#endif // STRATEGR_ACTIVITY_H
