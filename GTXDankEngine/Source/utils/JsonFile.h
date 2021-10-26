#pragma once
#include "pch.h"
#include "JsonFile.h"
#include "SerializationHelper.h"

class JsonFile
{
public:
    JsonFile(std::string filePath)
    {
        std::ifstream f(filePath); //taking file as inputstream
        if (f)
        {
            std::ifstream archetypeStream(filePath);
            archetypeStream >> data;
        }
    }
    ~JsonFile()
    {

    }
    ordered_json data;
    void OnLoad() {};
};
