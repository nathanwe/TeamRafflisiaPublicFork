#include "pch.h"

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "System.h"


#include "../utils/Log.h"


enum class ResourceState
{
	UNLOADED = 0,
	LOADED_BUT_NOT_UPDATED = 1,
	LOADED = 2
};



template <typename HandleResourceType>
class ResourceHandle
{
public:
	ResourceHandle(HandleResourceType* defaultOrRealResource, std::string inFilepath, ResourceState startSate);

	void onLoad(HandleResourceType* loadedResource);

	//it can pretend to be a pointer
	HandleResourceType& operator* ();
	HandleResourceType* operator-> ();
	HandleResourceType* GetPointer();

	ResourceState state;
	std::string filepath;
	HandleResourceType* resource;
	HandleResourceType* resourceToUpdateTo;
};

//.cpp stuff in .h because template
template<typename HandleResourceType>
inline ResourceHandle<HandleResourceType>::ResourceHandle
(HandleResourceType* defaultOrRealResource, std::string inFilepath, ResourceState startSate)
{
	resource = defaultOrRealResource;
	filepath = inFilepath;
	state = startSate;
	resourceToUpdateTo = nullptr;
	
	
}

template<typename HandleResourceType>
inline void ResourceHandle<HandleResourceType>::onLoad(HandleResourceType* loadedResource)
{
	resourceToUpdateTo = loadedResource;
	state = ResourceState::LOADED_BUT_NOT_UPDATED;
}

template<typename HandleResourceType>
inline HandleResourceType& ResourceHandle<HandleResourceType>::operator*()
{
	return *resource;
}

template<typename HandleResourceType>
inline HandleResourceType* ResourceHandle<HandleResourceType>::operator->()
{
	return resource;
}

template<typename HandleResourceType>
inline HandleResourceType* ResourceHandle<HandleResourceType>::GetPointer()
{
	return resource;
}




//back to .h stuf
template <class ResourceType>
class ResourceManager : public System
{
public:
	ResourceManager();
	ResourceHandle<ResourceType>* GetResourceHandle(std::string filepath);
	ResourceHandle<ResourceType>* GetResourceHandleNoThread(std::string filepath);


	 ~ResourceManager();

	bool Init();
	bool Init(std::string);
	void Update(float dt);
	bool Destroy();

private:
	void ThreadlyUpdateHandles();


	std::unordered_map<std::string, ResourceHandle<ResourceType>> resources;
	std::list<std::string> updatingResources;
	ResourceType* defaultResource;
	std::string defaultResourcePath;
	std::thread threadName;
	std::condition_variable cv;
	std::mutex mutex;
	bool threadRunning = true;
};

//.cpp stuff
template<typename ResourceType>
inline ResourceManager<ResourceType>::ResourceManager()
{
	defaultResource = nullptr;
}
template<class ResourceType>
inline ResourceHandle<ResourceType>* ResourceManager<ResourceType>::GetResourceHandle(std::string filepath)
{
	auto handle = resources.find(filepath);
	if (handle != resources.end())
	{
		return &(handle->second);
	}
	else
	{
		if (std::filesystem::exists(filepath))
		{
			resources.insert(std::pair<std::string,
				ResourceHandle<ResourceType>>(filepath,
					ResourceHandle<ResourceType>(defaultResource, filepath, ResourceState::UNLOADED)));

			updatingResources.push_back(filepath);
			auto handle = resources.find(filepath);
			if (handle != resources.end())
			{
				return &(handle->second);
			}
			else
			{
				LOG_ERROR("cant find just added resource");
				return &(handle->second);
			}
		}
		else
		{
			LOG_ERROR("file {} does not exist", filepath);
			return GetResourceHandle(defaultResourcePath);
		}
	}
}

template<class ResourceType>
inline ResourceHandle<ResourceType>* ResourceManager<ResourceType>::GetResourceHandleNoThread(std::string filepath)
{
	auto handle = resources.find(filepath);
	if (handle != resources.end())
	{
		while (handle->second.state != ResourceState::LOADED)
		{
			Sleep(1);
			Update(0);
		}
		return &(handle->second);
	}
	else
	{
		ResourceType* resource = new ResourceType(filepath); //this might take a while
		resource->OnLoad();
		resources.insert(std::pair<std::string,
			ResourceHandle<ResourceType>>(filepath,
				ResourceHandle<ResourceType>(resource, filepath, ResourceState::LOADED)));
		handle = resources.find(filepath);
		if (handle != resources.end())
		{
			return &(handle->second);
		}
		else
		{
			LOG_ERROR("cant find just added resource");
			return &(handle->second);
		}
	}
}

template<class ResourceType>
inline ResourceManager<ResourceType>::~ResourceManager()
{	
	threadRunning = false;
	std::unique_lock<std::mutex> inputUpdateLock(mutex);
	cv.notify_one();
	threadName.join();
}

template<class ResourceType>
inline bool ResourceManager<ResourceType>::Init()
{
	LOG_ERROR("cant init w/out string, use other init");
	return false;
}

template<class ResourceType>
inline bool ResourceManager<ResourceType>::Init(std::string _defaultResourcePath)
{
	defaultResource = new ResourceType(_defaultResourcePath);
	defaultResourcePath = _defaultResourcePath;
	defaultResource->OnLoad();
	resources.insert(std::pair<std::string,
		ResourceHandle<ResourceType>>(_defaultResourcePath,
			ResourceHandle<ResourceType>(defaultResource, _defaultResourcePath, ResourceState::LOADED)));
	threadName = std::thread(&ResourceManager::ThreadlyUpdateHandles, this);
	return false;
}

template<class ResourceType>
inline void ResourceManager<ResourceType>::Update(float dt)
{
	for (auto& handle : resources)
	{
		if (handle.second.state == ResourceState::LOADED_BUT_NOT_UPDATED)
		{
			handle.second.resource = handle.second.resourceToUpdateTo;
			handle.second.resource->OnLoad();
			handle.second.state = ResourceState::LOADED;
		}
	}
	cv.notify_one();
}

template<class ResourceType>
inline bool ResourceManager<ResourceType>::Destroy()
{
	/*
	for (auto& handle : resources)
	{
		delete(handle.second.resource);
	}
	*/
	threadRunning = false;
	std::unique_lock<std::mutex> inputUpdateLock(mutex);
	cv.notify_one();
//	threadName.join();
	
	return true;
	
}

template<class ResourceType>
inline void ResourceManager<ResourceType>::ThreadlyUpdateHandles()
{
	while (threadRunning)
	{
		if (!updatingResources.empty())
		{
			std::string filepath = *updatingResources.begin();
			ResourceType* resource = new ResourceType(filepath); //this might take a while
			auto handle = resources.find(filepath);
			if (handle != resources.end())
			{
				handle->second.onLoad(resource);
			}
			else
			{
				LOG_ERROR("cant find updating resource");
			}
			updatingResources.pop_front();
		}
		std::unique_lock<std::mutex> resourceUpdateLock(mutex);
		cv.wait(resourceUpdateLock);
	}
}
#endif