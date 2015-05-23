#include "ErrorLogManager.h"

#include "tools.h"

cException::cException( int errorNum, std::string errorDesc, std::string srcFilename, int lineNum )
{
    // write data into a specific format
    std::stringstream errss;
    errss << "error " << errorNum;
    errss << "; " << srcFilename;
    errss << ":" << lineNum;
    errss << "; " << errorDesc;


    errorText_ = errss.str();
}

const char* cException::what() const
{
    return errorText_.c_str();
}

void ErrorLogManager::CreateLogFile( std::string filename )
{
    logFile_.open( filename.c_str(), std::ios::app );
}

void ErrorLogManager::Flush()
{
    if( !logFile_.is_open() )
        return;
    logFile_ << logBuffer_.str();
    logFile_.flush();
    logBuffer_.str( "" );
}

void ErrorLogManager::Close()
{
    logFile_.close();
}

void ErrorLogManager::LogException( const cException& e )
{
    logBuffer_ << "**error** ";
    logBuffer_ << tools::GetTimeStr() << " : " << e.what();
    Flush();
}