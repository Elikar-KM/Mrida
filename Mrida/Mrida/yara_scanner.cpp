// SWAMI KARUPPASWAMI THUNNAI

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include "yara_scanner.h"



yara_scanner::yara_scanner()
{
	for (auto file : std::experimental::filesystem::recursive_directory_iterator("signatures"))
	{
		std::string yara_file = file.path().u8string();
		this->yara.addRuleFile(yara_file);
	}
}

std::vector<threat_info> yara_scanner::scan_file(std::string file_location)
{
	std::vector<threat_info> detected_signatures;
	if (this->yara.analyze(file_location))
	{
		// Get matched signatures
		for (yaracpp::YaraRule rule : yara.getDetectedRules())
		{
			std::string threat_name = rule.getName();
			std::string description = "UNKNOWN DESCRIPTION";
			std::string author = "UNKNOWN AUTHOR";
			std::vector<yaracpp::YaraMeta> meta_data = rule.getMetas();
			for (yaracpp::YaraMeta meta : meta_data)
			{
				if (meta.getId() == "description")
				{
					description = meta.getStringValue();
				}
				else if (meta.getId() == "author")
				{
					author = meta.getStringValue();
				}
			}
			threat_info signature;
			signature.set_threat_name(threat_name);
			signature.set_signature_author(author);
			signature.set_threat_description(description);
			detected_signatures.push_back(signature);
		}
	}
	return detected_signatures;
}