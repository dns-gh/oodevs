#include "LogTools.h"

LogTools::LogTools( std::string base )
{
    basePath_ = base;
    currentLogger_ = -1;
}

// Register a logger with a specific type
// /!\ They can be only one logger with a given file type
// The user may want to instance multiple loggers to log into 
// different files with the same file type filter
bool LogTools::RegisterLog( unsigned int type, const std::string& filename )
{
    auto it = fileMap_.find( type );
    if( it != fileMap_.end() )
        return false;
    fileMap_[ type ] = std::unique_ptr< std::ofstream >( new std::ofstream( basePath_ + std::string( "\\" ) + filename, std::ios::app ) );
    return fileMap_[ type ]->is_open();
}

LogTools& LogTools::OOLOG( unsigned int type )
{
    auto it = fileMap_.find( type );
    if( it == fileMap_.end() )
        currentLogger_ = -1;
    else
        currentLogger_ = type;
    return *this;
}

LogTools& LogTools::operator << ( std::stringstream& msg )
{
    if( currentLogger_ >= 0 )
    {
        ( *fileMap_[ currentLogger_ ] ) << msg.str();
        fileMap_[ currentLogger_ ]->flush( );
    }
    return *this;
}

const std::string LogTools::ToString( LOGTYPE type )
{
    switch( type )
    {
    case LOG_FATALERROR:   return "FATAL_ERROR";
    case LOG_ERROR:        return "ERROR";
    case LOG_WARNING:      return "WARNING";
    case LOG_MESSAGE:      return "MESSAGE";
    default:      return "Unknown Log type";
    }
}