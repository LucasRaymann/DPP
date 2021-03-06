#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace discord {

    const inline std::unordered_map<std::string, int> event_m{
        { "HELLO", 0 },
        { "READY", 1 },
        { "RESUMED", 2 },
        { "INVALID_SESSION", 3 },
        { "CHANNEL_CREATE", 4 },
        { "CHANNEL_UPDATE", 5 },
        { "CHANNEL_DELETE", 6 },
        { "CHANNEL_PINS_UPDATE", 7 },
        { "GUILD_CREATE", 8 },
        { "GUILD_UPDATE", 9 },
        { "GUILD_DELETE", 10 },
        { "GUILD_BAN_ADD", 11 },
        { "GUILD_BAN_REMOVE", 12 },
        { "GUILD_EMOJIS_UPDATE", 13 },
        { "GUILD_INTEGRATIONS_UPDATE", 14 },
        { "GUILD_MEMBER_ADD", 15 },
        { "GUILD_MEMBER_REMOVE", 16 },
        { "GUILD_MEMBER_UPDATE", 17 },
        { "GUILD_MEMBERS_CHUNK", 18 },
        { "GUILD_ROLE_CREATE", 19 },
        { "GUILD_ROLE_UPDATE", 20 },
        { "GUILD_ROLE_DELETE", 21 },
        { "MESSAGE_CREATE", 22 },
        { "MESSAGE_UPDATE", 23 },
        { "MESSAGE_DELETE", 24 },
        { "MESSAGE_DELETE_BULK", 25 },
        { "MESSAGE_REACTION_ADD", 26 },
        { "MESSAGE_REACTION_REMOVE", 27 },
        { "MESSAGE_REACTION_REMOVE_ALL", 28 },
        { "PRESENCE_UPDATE", 29 },
        { "TYPING_START", 30 },
        { "USER_UPDATE", 31 },
        { "VOICE_STATE_UPDATE", 32 },
        { "VOICE_SERVER_UPDATE", 33 },
        { "WEBHOOKS_UPDATE", 34 }
    };

    enum events : int {
        hello,
        ready,
        resumed,
        invalid_session,
        channel_create,
        channel_update,
        channel_delete,
        channel_pins_update,
        guild_create,
        guild_update,
        guild_delete,
        guild_ban_add,
        guild_ban_remove,
        guild_emojis_update,
        guild_integrations_update,
        guild_member_add,
        guild_member_remove,
        guild_member_update,
        guild_members_chunk,
        guild_role_create,
        guild_role_update,
        guild_role_delete,
        message_create,
        message_update,
        message_delete,
        message_delete_bulk,
        message_reaction_add,
        message_reaction_remove,
        message_reaction_remove_all,
        presence_update,
        typing_start,
        user_update,
        voice_state_update,
        voice_server_update,
        webhooks_update,
        raw_message_update,
        raw_message_delete,
        raw_message_delete_bulk,
    };

    enum class AuditLogEventType : short {
        GUILD_UPDATE = 1,
        CHANNEL_CREATE = 10,
        CHANNEL_UPDATE,
        CHANNEL_DELETE,
        CHANNEL_OVERWRITE_CREATE,
        CHANNEL_OVERWRITE_UPDATE,
        CHANNEL_OVERWRITE_DELETE,
        MEMBER_KICK = 20,
        MEMBER_PRUNE,
        MEMBER_BAN_ADD,
        MEMBER_BAN_REMOVE,
        MEMBER_UPDATE,
        MEMBER_ROLE_UPDATE,
        ROLE_CREATE = 30,
        ROLE_UPDATE,
        ROLE_DELETE,
        INVITE_CREATE = 40,
        INVITE_UPDATE,
        INVITE_DELETE,
        WEBHOOK_CREATE = 50,
        WEBHOOK_UPDATE,
        WEBHOOK_DELETE,
        EMOJI_CREATE = 60,
        EMOJI_UPDATE,
        EMOJI_DELETE,
        MESSAGE_DELETE = 72
    };
}  // namespace discord