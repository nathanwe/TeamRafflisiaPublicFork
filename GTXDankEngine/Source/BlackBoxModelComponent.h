#ifndef BLACKBOXMODELCOMPONENT_H
#define BLACKBOXMODELCOMPONENT_H

#include <chrono>
#include "Model.h"
#include <string>
#include "Enums.h"
#include "Shader.h"
#include "Camera.h"
#include "AbstractComponent.h"
#include <memory>



class BlackBoxModelComponent : public AbstractComponent
{
public:
	BlackBoxModelComponent(std::shared_ptr<Model> _model);
	~BlackBoxModelComponent();
	void Update(std::chrono::milliseconds dt);
	void HandleEvent(Event& event);
	ComponentType GetType();
	void Draw(Shader shaderProgram, Camera camera);

private:
	std::shared_ptr<Model> model;
};


#endif