#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "plugin_manager.h"
//#include "utilities.h"

using namespace std;
namespace fs = boost::filesystem;

vector<string> list_files(string path, string extension=".def")
{
    fs::path files_path(path);
    vector<string> ret;
    if (fs::exists(files_path) && fs::is_directory(files_path))
    {
        for(fs::directory_iterator end, iter(files_path);
            iter != end; ++iter)
        {
            if(fs::is_directory(iter->status()))
            {
                vector<string> temp = list_files(iter->path().c_str());
                ret.insert(ret.end(), temp.begin(), temp.end());
            }

            if(fs::is_regular_file(iter->path()) &&
               (iter->path().extension() == extension))
            {
                ret.push_back(iter->path().c_str());
                cout << iter->path() << endl;      
            }
        }
    }
    return ret;
}

void PluginManager::scan_plugins(const string path)
{   
    vector<string> f = list_files(path);
    for(auto file : f)
        parse_plugin_def(file);
}

void PluginManager::list_plugins()
{
	for(auto p : plugins) {
		std::cout << p.first << ": " << p.second.name << std::endl;
		std::cout << "\t" << p.second.description << " - " << p.second.usage << std::endl;
	}
}

bool PluginManager::has_plugin(const string &plugin)
{
	return plugins.count(plugin);
}

PluginInfo PluginManager::get_plugin(const string &plugin)
{
	if(has_plugin(plugin))
		return plugins[plugin];

	return PluginInfo {};
}

void PluginManager::parse_plugin_def(const string &def_file)
{
    std::ifstream in(def_file);

    string name;
    getline(in, name);

    string executable;
    getline(in, executable);

    string parameters;
    getline(in, parameters);

    boost::filesystem::path p(def_file);
    boost::filesystem::path dir = p.parent_path();

#ifdef __linux__
    executable = "./" + executable;
#endif

    string value = dir.c_str() + executable + " " + parameters;

    PluginInfo plug = { name, parameters, "", dir.c_str() };
    plugins[name] = plug;
}
