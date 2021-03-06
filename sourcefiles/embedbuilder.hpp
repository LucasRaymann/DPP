#pragma once

#include "discord.hpp"


discord::EmbedBuilder::EmbedBuilder(nlohmann::json event)
    : embed{ event } {
}

discord::EmbedBuilder::EmbedBuilder()
    : embed{ nlohmann::json({}) } {
}

discord::EmbedBuilder &discord::EmbedBuilder::set_title(std::string const &title) {
    embed["title"] = title;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_description(std::string const &desc) {
    embed["description"] = desc;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_url(std::string const &url) {
    embed["url"] = url;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_timestamp(std::string const &iso_8601) {
    embed["timestamp"] = iso_8601;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_color(Color c) {
    embed["color"] = c.raw_int;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_footer(std::string const &text, std::string const &icon_url) {
    embed["footer"] = nlohmann::json({});
    if (icon_url != "") {
        embed["footer"]["icon_url"] = icon_url;
    }
    embed["footer"]["text"] = text;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_image(std::string const &url, int w, int h) {
    embed["image"] = nlohmann::json({ { "url", url } });
    if (w != -1) {
        embed["image"]["width"] = w;
    }
    if (h != -1) {
        embed["image"]["height"] = h;
    }
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_thumbnail(std::string const &url, int w, int h) {
    embed["thumbnail"] = nlohmann::json({ { "url", url } });
    if (w != -1) {
        embed["thumbnail"]["width"] = w;
    }
    if (h != -1) {
        embed["thumbnail"]["height"] = h;
    }

    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_video(std::string const &url, int w, int h) {
    embed["video"] = nlohmann::json({});
    if (w != -1) {
        embed["video"]["width"] = w;
    }
    if (h != -1) {
        embed["video"]["height"] = h;
    }

    embed["video"]["url"] = url;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::set_author(std::string const &name, std::string const &url, std::string const &icon_url) {
    embed["author"] = nlohmann::json({});
    if (!icon_url.empty()) {
        embed["author"]["icon_url"] = icon_url;
    }
    if (url != "") {
        embed["author"]["url"] = url;
    }

    embed["author"]["name"] = name;
    return *this;
}

discord::EmbedBuilder &discord::EmbedBuilder::add_field(std::string const &name, std::string const &value, bool in_line) {
    if (!embed.contains("fields")) {
        embed["fields"] = nlohmann::json::array();
    }
    embed["fields"].push_back(nlohmann::json({ { "name", name },
                                               { "value", value },
                                               { "inline", in_line } }));
    return *this;
}

nlohmann::json discord::EmbedBuilder::to_json() const {
    return embed;
}

discord::EmbedBuilder::operator nlohmann::json() {
    return embed;
}