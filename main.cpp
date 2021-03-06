#include <fstream>
#include <iostream>
#include <thread>

#define __DPP_DEBUG

#include "activity.hpp"
#include "embedbuilder.hpp"
#include "message.hpp"
#include "sourcefiles/bot.hpp"
#include "utility.hpp"
#include "webhook.hpp"


int main() {
    std::ifstream file("token.txt");
    std::string token;
    std::getline(file, token);
    discord::Bot bot{ token, ">" };

    bot.register_callback<discord::events::ready>([&bot]() { std::cout << "Ready!" << std::endl
                                                                       << "Logged in as: " << bot.username << "#" << bot.discriminator
                                                                       << std::endl
                                                                       << "ID: " << bot.id << std::endl
                                                                       << "-----------------------------" << std::endl; });


    bot.register_command("test", [](discord::Context const&) {
        std::cout << discord::Object{ 500 } << std::endl;
    });

    bot.register_command("image", [](discord::Context const& ctx) {
        ctx.send(discord::EmbedBuilder()
                     .set_description("Test")
                     .set_image(ctx.message.author->avatar.url));
    });

    bot.register_command("file", [](discord::Context const& ctx) {
        ctx.channel->send("Here is a file!", { { "test.txt", "todo.txt", true } }, false);
    });

    return bot.run();
}
