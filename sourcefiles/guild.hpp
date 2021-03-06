#pragma once
#include <nlohmann/json.hpp>
#include "discord.hpp"

#include "utility.hpp"

#include "audit.hpp"
#include "channel.hpp"
#include "emoji.hpp"
#include "member.hpp"
#include "role.hpp"

#include <cpr/cpr.h>

discord::Guild::Guild(snowflake id)
    : discord::Object(id) {
    auto g = discord::utils::get(discord::detail::bot_instance->guilds, [id](auto const& guild) {
        return guild->id == id;
    });
    if (g) {
        *this = *g;
    }
}

discord::Guild::Guild(nlohmann::json const guild)
    : splash{ get_value(guild, "splash", 0) },
      mfa_level{ get_value(guild, "mfa_level", 0) },
      afk_timeout{ get_value(guild, "afk_timeout", 0) },
      member_count{ get_value(guild, "member_count", 0) },
      verification_level{ get_value(guild, "verification_level", 0) },
      explicit_content_filter{ get_value(guild, "explicit_content_filter", 0) },
      large{ get_value(guild, "large", true) },
      unavailable{ get_value(guild, "unavailable", false) },
      id{ to_sf(guild["id"]) },
      application_id{ to_sf(get_value(guild, "application_id", "0")) },
      name{ get_value(guild, "name", "") },
      region{ get_value(guild, "region", "") },
      created_at{ time_from_discord_string(get_value(guild, "joined_at", "")) },
      vanity_url_code{ get_value(guild, "vanity_url_code", "") },
      roles{ from_json_array<discord::Role>(guild, "roles", discord::utils::get(discord::detail::bot_instance->guilds, [this](auto const& g) { return this->id == g->id; })) },
      emojis{ from_json_array<discord::Emoji>(guild, "emojis") } {

    if (guild.contains("members")) {
        for (auto& each : guild["members"]) {
            discord::Member member{
                each,
                discord::User(each["user"]),
                this
            };
            members.emplace_back(std::make_shared<discord::Member>(member));
            if (each["user"]["id"] == guild["owner_id"]) {
                owner = member;
            }
        }
    }

    if (guild.contains("icon")) {
        if (!guild["icon"].is_null()) {
            std::string av_hash = guild["icon"];
            icon = Asset{
                av_hash, guild_icon, av_hash[0] == 'a' && av_hash[1] == '_', id
            };
        } else {
            icon = Asset{};
        }
    }

    if (guild.contains("banner")) {
        if (!guild["banner"].is_null()) {
            std::string av_hash = guild["banner"];
            icon = Asset{
                av_hash, guild_banner, av_hash[0] == 'a' && av_hash[1] == '_', id
            };
        } else {
            icon = Asset{};
        }
    }

    if (guild.contains("channels") && channels.empty()) {
        for (auto const& each : guild["channels"]) {
            channels.emplace_back(std::make_shared<discord::Channel>(each, id));
        }
    }
}

discord::Guild& discord::Guild::update(nlohmann::json const data) {
    update_object_bulk(data,
                       "splash", splash,
                       "mfa_level", mfa_level,
                       "afk_timeout", afk_timeout,
                       "verification_level", verification_level,
                       "explicit_content_filter", explicit_content_filter,
                       "unavailable", unavailable,
                       "application_id", application_id,
                       "name", name,
                       "region", region,
                       "vanity_url_code", vanity_url_code);

    if (data.contains("roles")) {
        roles = from_json_array_special<discord::Role>(data["roles"], discord::utils::get(discord::detail::bot_instance->guilds, [this](auto const& g) { return this->id == g->id; }));
    }

    if (data.contains("emojis")) {
        emojis = from_json_array<discord::Emoji>(data["emojis"]);
    }

    if (data.contains("icon")) {
        if (!data["icon"].is_null()) {
            std::string av_hash = data["icon"];
            icon = Asset{
                av_hash, guild_icon, av_hash[0] == 'a' && av_hash[1] == '_', id
            };
        } else {
            icon = Asset{};
        }
    }

    if (data.contains("banner")) {
        if (!data["banner"].is_null()) {
            std::string av_hash = data["banner"];
            icon = Asset{
                av_hash, guild_banner, av_hash[0] == 'a' && av_hash[1] == '_', id
            };
        } else {
            icon = Asset{};
        }
    }
    return *this;
}

std::vector<discord::Webhook> discord::Guild::get_webhooks() {
    return from_json_array<discord::Webhook>(
        send_request<request_method::Get>(nlohmann::json(),
                                          get_default_headers(),
                                          endpoint("/guilds/%/webhooks", id)));
}

void discord::Guild::leave() {
    send_request<request_method::Delete>(nlohmann::json({}), get_default_headers(), endpoint("/users/@me/guilds/%", id));
}

std::vector<discord::Emoji> discord::Guild::list_emojis() {
    return from_json_array<discord::Emoji>(
        send_request<request_method::Get>(
            nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/emojis", id)));
}

discord::Emoji discord::Guild::get_emoji(discord::Emoji const& e) {
    return discord::Emoji{
        send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/emojis/%", id, e.id))
    };
}

discord::Emoji discord::Guild::edit_emoji(discord::Emoji const& emote, std::string name, std::vector<discord::Role> roles) {
    nlohmann::json data(
        { { "name", name },
          { "roles", {} } });
    for (auto const& each : roles) {
        data["roles"].push_back(each.id);
    }
    return discord::Emoji{
        send_request<request_method::Patch>(
            data,
            get_default_headers(),
            endpoint("/guilds/%/emojis/%", id, emote.id))
    };
}

void discord::Guild::edit(std::string const& name, std::string const& rg, int verif_level, int default_message_notif, int explicit_cont_filt, snowflake afk_chan_id, int afk_timeout, std::string const& icon, snowflake owner_id, std::string const& splash, snowflake system_channel_id) {
    nlohmann::json data{
        { "name", name }
    };
    if (rg != "") data["region"] = rg;
    if (verif_level != -1) data["verification_level"] = verif_level;
    if (default_message_notif != -1) data["default_message_notifications"] = default_message_notif;
    if (explicit_cont_filt != -1) data["explicit_content_filter"] = explicit_cont_filt;
    if (afk_chan_id != -1) data["afk_channel_id"] = afk_chan_id;
    if (afk_timeout != -1) data["afk_timeout"] = afk_timeout;
    if (icon != "") data["icon"] = icon;
    if (owner_id != -1) data["owner_id"] = owner_id;
    if (splash != "") data["splash"] = splash;
    if (system_channel_id != -1) data["system_channel_id"] = system_channel_id;

    send_request<request_method::Patch>(
        data,
        get_default_headers(),
        endpoint("/guilds/%", id));
}

void discord::Guild::remove() {
    send_request<request_method::Delete>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%", id));
}

std::vector<discord::Channel> discord::Guild::get_channels() {
    return from_json_array_special<discord::Channel>(
        send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/channels", id)), id);
}

discord::Channel discord::Guild::create_channel(std::string const& name, bool nsfw, int type, std::string const& topic, int bitrate, int user_limit, int rate_limit_per_user, int position, std::vector<discord::PermissionOverwrites> const& permission_overwrites, snowflake parent_id) {
    nlohmann::json data = nlohmann::json({ { "name", name },
                                           { "nsfw", nsfw },
                                           { "permission_overwrites", nlohmann::json::array() } });
    if (type != -1) data["type"] = type;
    if (topic != "") data["topic"] = topic;
    if (bitrate != -1) data["bitrate"] = bitrate;
    if (user_limit != -1) data["user_limit"] = user_limit;
    if (rate_limit_per_user != -1) data["rate_limit_per_user"];
    if (position != -1) data["position"] = position;

    for (auto const& each : permission_overwrites) {
        data["permission_overwrites"].push_back(each.to_json());
    }

    if (parent_id != -1) data["parent_id"] = parent_id;

    return discord::Channel{
        send_request<request_method::Post>(data, get_default_headers(), endpoint("/guilds/%/channels", id))
    };
}


void discord::Guild::remove_emoji(discord::Emoji const& emote) {
    send_request<request_method::Delete>(
        nlohmann::json({}),
        get_default_headers(),
        endpoint("/guilds/%/emojis/%", id, emote.id));
}

discord::Member discord::Guild::get_member(snowflake m_id) {
    auto req_data = send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/members/%", this->id, m_id));
    return discord::Member{
        req_data,
        req_data["user"],
        this
    };
}

std::vector<discord::Member> discord::Guild::get_members(int limit, snowflake after) {
    return from_json_array<discord::Member>(
        send_request<request_method::Get>(
            nlohmann::json({ { "limit", limit }, { "after", after } }),
            get_default_headers(),
            endpoint("/guilds/%/members", this->id)));
}

std::vector<std::pair<std::string, discord::User>> discord::Guild::get_bans() {
    auto response = send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/bans", id));
    std::vector<std::pair<std::string, discord::User>> ret_vec;
    for (auto const& each : response) {
        ret_vec.push_back({ each["reason"], discord::User{ each["user"] } });
    }
    return ret_vec;
}

std::pair<std::string, discord::User> discord::Guild::get_ban(discord::Object const& banned_obj) {
    auto response = send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/bans/%", id, banned_obj.id));
    return { response["reason"], discord::User{ response["user"] } };
}


void discord::Guild::add_member(nlohmann::json const& data, snowflake user_id) {
    send_request<request_method::Put>(data, get_default_headers(), endpoint("/guilds/%/members/%", this->id, user_id));
}

void discord::Guild::edit_bot_username(std::string const& new_nick) {
    send_request<request_method::Patch>(nlohmann::json({ { "nick", new_nick } }), get_default_headers(), endpoint("/guilds/%/members/@me/nick", discord::detail::bot_instance->id));
}

void discord::Guild::unban(discord::Object const& obj) {
    send_request<request_method::Delete>(
        nlohmann::json({}),
        get_default_headers(),
        endpoint("/guilds/%/bans/%", id, obj.id));
}

std::vector<discord::Role> discord::Guild::get_roles() {
    return from_json_array<discord::Role>(
        send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/roles", id)));
}

discord::Role discord::Guild::create_role(std::string const& _name, PermissionOverwrites& _perms, discord::Color _color, bool _hoist, bool _mention) {
    return discord::Role{
        send_request<request_method::Post>(
            nlohmann::json({ { "name", _name },
                             { "permissions", _perms.base_permissions },
                             { "color", _color.raw_int },
                             { "hoist", _hoist },
                             { "mentionable", _mention } }),
            get_default_headers(),
            endpoint("/guilds/%/roles", id)),
        discord::utils::get(discord::detail::bot_instance->guilds, [this](auto const& g) { return g->id == this->id; })
    };
}


int discord::Guild::get_prune_count(int days) {
    return send_request<request_method::Get>(
        nlohmann::json({ { "days", days } }), get_default_headers(), endpoint("/guilds/%/prune", id))["pruned"];
}

int discord::Guild::begin_prune(int days, bool compute_prune_count) {
    return get_value(send_request<request_method::Post>(
                         nlohmann::json({ { "days", days },
                                          { "compute_prune_count", compute_prune_count } }),
                         get_default_headers(),
                         endpoint("/guilds/%/prune", id)),
                     "pruned",
                     0);
}


std::vector<discord::VoiceRegion> discord::Guild::get_voice_regions() {
    auto resp = send_request<request_method::Get>(nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/regions", id));
    std::vector<VoiceRegion> ret_val;
    for (auto const& each : resp) {
        ret_val.push_back({ each["id"],
                            each["name"],
                            each["vip"],
                            each["optimal"],
                            each["deprecated"],
                            each["custom"] });
    }
    return ret_val;
}

std::vector<discord::Invite> discord::Guild::get_invites() {
    return from_json_array<discord::Invite>(
        send_request<request_method::Get>(
            nlohmann::json({}),
            get_default_headers(),
            endpoint("/guilds/%/invites", id)));
}

discord::snowflake discord::Guild::get_embed() {
    return get_value(send_request<request_method::Get>(
                         nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/embed", id)),
                     "channel_id",
                     0);
}

discord::snowflake discord::Guild::edit_embed(snowflake c_id) {
    return get_value(send_request<request_method::Patch>(
                         nlohmann::json({ { "enabled", c_id != -1 }, { "channel_id", c_id } }),
                         get_default_headers(),
                         endpoint("/guilds/%/embed", id)),
                     "channel_id",
                     0);
}

std::string discord::Guild::get_vanity_invite_url() {
    return get_value(send_request<request_method::Get>(
                         nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/vanity-url", id)),
                     "code",
                     "");
}


discord::Asset discord::Guild::get_widget_image(std::string const& style) {
    discord::Asset a{};
    a.url = endpoint("/guilds/%/widget.png?style=%", id, style);
    return a;
}


std::vector<discord::Integration> discord::Guild::get_integrations() {
    return from_json_array<discord::Integration>(
        send_request<request_method::Get>(
            nlohmann::json({}), get_default_headers(), endpoint("/guilds/%/integrations", id)));
}


void discord::Guild::create_integration(discord::Integration const& integr) {
    send_request<request_method::Post>(
        nlohmann::json({ { "type", integr.type }, { "id", integr.id } }),
        get_default_headers(),
        endpoint("/guilds/%/integrations", id));
}

void discord::Guild::edit_integration(discord::Integration const& integr, int expire_behavior, int expire_grace_period, bool enable_emotes) {
    send_request<request_method::Patch>(
        nlohmann::json({ { "expire_behavior", expire_behavior },
                         { "expire_grace_period", expire_grace_period },
                         { "enable_emoticons", enable_emotes } }),
        get_default_headers(),
        endpoint("/guilds/%/integrations/%", id, integr.id));
}


void discord::Guild::remove_integration(discord::Integration const& integr) {
    send_request<request_method::Delete>(
        nlohmann::json({}),
        get_default_headers(),
        endpoint("/guilds/%/integrations/%", id, integr.id));
}

void discord::Guild::sync_integration(discord::Integration const& integr) {
    send_request<request_method::Post>(
        nlohmann::json({}),
        get_default_headers(),
        endpoint("/guilds/%/integrations/%/sync", id, integr.id));
}

discord::Emoji discord::Guild::create_emoji(std::string const& name, discord::Emoji& emote_data, std::vector<discord::Role> roles) {
    nlohmann::json data({ { "name", name },
                          { "image", encode64(emote_data.image.read()) },
                          { "roles", nlohmann::json::array() } });
    for (auto const& each : roles) {
        data["roles"].push_back(each.id);
    }
    return discord::Emoji{
        send_request<request_method::Post>(
            data,
            get_default_headers(),
            endpoint("/guilds/%/emojis", id))
    };
}

discord::AuditLogs discord::Guild::get_audit_logs() {
    return discord::AuditLogs{
        send_request<request_method::Get>(
            nlohmann::json({}),
            get_default_headers(),
            endpoint("/guilds/%/audit-logs", id))
    };
}
