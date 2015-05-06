#include "LogTools.h"

LogTools::LogTools( std::string base )
{
    basePath_ = base;
    currentLogger_ = -1;
}

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