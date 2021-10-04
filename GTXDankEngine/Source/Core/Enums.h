#ifndef ENUMS_H
#define ENUMS_H

enum class ComponentType
{
	ABSTRACT = 0,
	BLACK_BOX_MODEL = 1
};


enum class ErrorEnum
{
	SUCCESS = 0,
	FAIL_BECAUSE_COMPONENT_ALREADY_EXISTS = 1,
	FAIL_BECAUSE_NO_MORE_MEMORY = 2
};



//TODO real events
struct Event
{

};
#endif // !ENUMS_H
