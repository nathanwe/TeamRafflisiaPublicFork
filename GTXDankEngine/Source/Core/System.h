#ifndef SYSTEM_H
#define SYSTEM_H



// abstract base class for all systems
class System
{
public:
	virtual ~System() {};

	virtual bool Init() = 0;
	virtual void Update(float timeStamp = 0) = 0;
	virtual bool Destroy() = 0;
};
#endif // !SYSTEM_H
