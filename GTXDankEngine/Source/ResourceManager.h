#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include<memory>
#include"Model.h"
#include<string>
#include<map>

class ResourceManager
{
public:
	std::shared_ptr<Model> GetBlackBoxModel(std::string file)
	{
		auto model = modelMap.find(file);
		if(model != modelMap.end())
		{
			return model->second;
		}
		else
		{
			auto newModel = std::make_shared<Model>(file.c_str());
			modelMap.insert(std::pair< std::string, std::shared_ptr<Model>>(file, newModel));
			return newModel;
		}
	}
private:
	std::map<std::string, std::shared_ptr<Model>> modelMap;
};

#endif // !RESOURCEMANAGER_H
