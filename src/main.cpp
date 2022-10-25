#include <fstream>
#include <chrono>
#include <map>

#include <spdlog/spdlog.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>

#include "../handler/builder.h"
#include "../handler/btnHandler.h"

using json = nlohmann::json;

int main()
{
	json reader;
	{
		std::ifstream reading("config.json");

		if (!reading)
		{
			std::cout << "No configuration file found! Please add it with the name \"config.json\"!";
			std::cin.get();

			exit(0);
		}
		
		reading >> reader;
		reading.close();
	}

	const std::string token = reader["token"];
	dpp::cluster client(token, dpp::i_all_intents);

	client.on_ready([&client](const dpp::ready_t& event) 
		{
			client.set_presence(
				dpp::presence(dpp::ps_dnd, dpp::at_game, "Genshin Impact")
			);
			
			SlashCommandCreate(client);

			fmt::print("[{} - STARTED]: {} is online!\n", dpp::utility::current_date_time(), client.me.username);
			fmt::print("[{} - REGISTERED]: Successfully registered slash commands!\n", dpp::utility::current_date_time());
		});

	client.on_slashcommand([&client](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction commandData = event.command.get_command_interaction();
			auto commandFilter                   = commands.find(commandData.name);

			if (commandFilter != commands.end())
			{
				commandFilter->second.function(client, event);
			}
		});

	client.on_button_click([](const dpp::button_click_t& event) 
		{
			ButtonHandle(event);
		});
	
	client.start(false);
	return 0;
}
