#pragma once 

#include "Filestate.h"

#ifdef WIN32
#include <Windows.h>
#endif

namespace glpipe
{
	Filestate::Filestate()
	{
		versionHash = 0;
		stream = 0;
		filesource_name = "";
	}

	Filestate::Filestate(const char* s)
	{
		versionHash = 0;
		stream = 0;
		filesource_name = s;
	}

	FILE* Filestate::openStream()
	{
		if (stream)
		{
			throw 2;
		}
		else
		{
			return stream = fopen(filesource_name.c_str(),"r");
		}
	}

	void Filestate::closeStream()
	{
		if (stream)
		{
			fclose(stream);
			stream = 0;
		}
		else
		{
			throw 3;
		}
	}

#ifdef WIN32
	unsigned long Filestate::getFilestateHash()
	{
		WIN32_FILE_ATTRIBUTE_DATA info;
		if (GetFileAttributesExA(filesource_name.c_str(),GetFileExInfoStandard,&info))
		{
			return info.ftLastWriteTime.dwHighDateTime ^ info.ftLastWriteTime.dwLowDateTime;
		}
		else
		{
			return this->versionHash;
		}
	}
#endif
		
	bool Filestate::isFilestate()
	{
		return filesource_name.size();
	}

	bool Filestate::FilestateChanged()
	{
		return versionHash != getFilestateHash();
	}

	void Filestate::FilestateUpdate()
	{
		versionHash = getFilestateHash();
	}

	void Filestate::setFilestateSource(const char* s)
	{
		if (stream)
		{
			throw "Can't change source while stream open!";
		}
		else
		{
			filesource_name = s;
		}
	}

	const char* Filestate::getFilestateSource()
	{
		return filesource_name.c_str();
	}

};

