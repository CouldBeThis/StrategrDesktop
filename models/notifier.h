//
// Created by Dmitry Khrykin on 2019-11-26.
//

#ifndef STRATEGR_NOTIFIER_H
#define STRATEGR_NOTIFIER_H

#include <thread>
#include <stdexcept>
#include <ostream>

#include "strategy.h"

namespace stg {
    class strategy;
    struct notification {
        using seconds = unsigned int;
        using minutes = unsigned int;

        enum class type {
            prepare_start,
            start,
            prepare_end,
            end,
            prepare_strategy_end,
            strategy_end
        };

        notification(const session &session, type type);

        std::string identifier = make_string_uuid();

        std::string title;
        std::string message;

        seconds delivery_time;

    private:
        static auto make_string_uuid() -> std::string;
        static auto make_title(const session &session, type type) -> std::string;
        static auto make_sub_title(const session &session, type type) -> std::string;
        static auto make_delivery_time(const session &session, type type) -> seconds;

        friend bool operator==(const notification &lhs, const notification &rhs);

        friend std::ostream &operator<<(std::ostream &os, const notification &notification);
    };

    class notifier {
    public:
        using seconds = notification::seconds;
        using minutes = notification::minutes;

        using scheduler_t = std::function<void(const std::vector<notification> &)>;
        using resetter_t = std::function<void(const std::vector<std::string> &)>;
        using sender_t = std::function<void(const notification &)>;

        static auto immediate_delivery_seconds(minutes minutes_time) -> seconds;
        static auto prepare_delivery_seconds(minutes minutes_time) -> seconds;

        scheduler_t on_schedule_notifications = nullptr;
        resetter_t on_delete_notifications = nullptr;
        sender_t on_send_notiifcation = nullptr;

        explicit notifier(const strategy &strategy);

        void start_watching();
        void stop_watching();
        void schedule();

        void send_now_if_needed(seconds polling_seconds_interval);

        auto scheduled_notifications() const -> const std::vector<stg::notification> &;
        auto scheduled_identifiers() const -> std::vector<std::string>;

        static const seconds prepare_seconds_interval = 5 * 60;
        static const seconds immediate_seconds_interval = 20;
    private:
        static void remove_stale(std::vector<notification> &notifications);

        const strategy &strategy;
        seconds last_poll_time = 0;
        bool is_watching = false;

        std::vector<stg::notification> _scheduled_notifications;
    };
}

#endif //STRATEGR_NOTIFIER_H
