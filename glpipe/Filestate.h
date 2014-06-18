
#pragma once

#include <string>

namespace glpipe
{
	class Filestate
	{
		std::string filesource_name;
		unsigned long versionHash;
		unsigned long getFilestateHash();
		FILE* stream;

	public:
		
		Filestate();
		Filestate(const char* s);
		bool isFilestate();
		bool FilestateChanged();
		void FilestateUpdate();
		FILE* openStream();
		void closeStream();
		void setFilestateSource(const char* s);
		const char* getFilestateSource();

	};


};
