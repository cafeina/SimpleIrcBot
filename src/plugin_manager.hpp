#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <string>
#include <map>

struct PluginInfo
{
	std::string name;
	std::string usage;
	std::string description;
	std::string path;
};

class PluginManager
{
public:
	PluginManager() {}
	void scan_plugins(const std::string="plugins/");
	void list_plugins();
	bool has_plugin(const std::string&);
	PluginInfo get_plugin(const std::string&);

private:
	void parse_plugin_def(const std::string&);

	std::map<std::string, struct PluginInfo> plugins {};
};

#endif //PLUGIN_MANAGER_H
