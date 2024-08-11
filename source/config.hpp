#include "json.hpp"
using json = nlohmann::ordered_json;
#include <io/file_stream.h>
#include <QString>
class Config
{
public :
    json config;
    std::filesystem::path path = "config.json";

    json basecfg = {{"language","none"},{"ActualVersion",0},{"MinVersion",0}};
    Config() {
        if(!std::filesystem::exists(path)){
            config = basecfg;
            update();
        }else{
            try{
                std::ifstream cfile(path);
                config = json::parse(cfile);
                for (auto& [key, value] : basecfg.items()) {
                    if(!config.contains(key)) config[key] = value;
                }
            } catch(...){
                config = basecfg;
            }
            update();
        }
    };
    // std::string lang(){
    //     return config["language"];
    // }
    int* versions(){
        static int v[2] = {config["ActualVersion"],config["MinVersion"]};
        return v;
    }
    void versions(int ActualVersion,int MinVersion){
        config["ActualVersion"] = ActualVersion;
        config["MinVersion"] = MinVersion;
        update();
    }
    QString lang(){
        return QString::fromStdString(config["language"]);
    }
    void lang(QString lan){
        config["language"] = lan.toStdString();
        update();
    }
    void update(){
        std::ofstream o(path);
        o << config.dump(4) << std::endl;
    }
} ;
