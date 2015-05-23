#ifndef _LOGTOOLS_H
#define _LOGTOOLS_H

#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <iostream>

#include "tools.h"

enum FILETYPE
{
    FILE_RESOURCE = 0,
    FILE_INFOS,
    FILE_GAMELOOP,    
};

enum LOGTYPE
{
    LOG_FATALERROR = 0,
    LOG_ERROR,
    LOG_WARNING,
    LOG_MESSAGE,
};

#define OOSTREAM( type, msg ) static_cast< std::stringstream& >( std::stringstream() << \
    LogTools::ToString( type ) << " " << tools::GetTimeStr() << ": " << msg << std::endl )

class LogTools
{
public:
    LogTools( std::string basePath );
    LogTools& OOLOG( unsigned int type );
    bool RegisterLog( unsigned int type, const std::string& filename );
    LogTools& operator<< ( std::stringstream& msg );

    static const std::string ToString( LOGTYPE type );

private:
    std::map< unsigned int, std::unique_ptr< std::ofstream > > fileMap_;
    std::string basePath_;
    int currentLogger_;
};

#endif