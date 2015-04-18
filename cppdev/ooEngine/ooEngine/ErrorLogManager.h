#ifndef _ERRORLOGMANAGER_H
#define _ERRORLOGMANAGER_H

#include "EngineObject.h"
#include "SingletonTemplate.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <time.h>

// ------------------
// Exception handling
// -------------------
class cException : public std::exception
{
public:
    cException( int errorNum, std::string errorDesc, std::string srcFilename, int lineNum );

    // override std::exception
    const char* what( ) const;

private:
    int errorNumber_;
    std::string errorDescription_;
    std::string srcFilename_;
    int lineNumber_;
    std::string errorText_;
};

// Define a throw macro for the cException class/struct
#ifndef OOTHROW
#define OOTHROW( errorNum, errorDesc ) throw cException( errorNum, errorDesc, __FILE__, __LINE__ );
#endif
// -------------------

// Manager for error and logging
class ErrorLogManager : public EngineObject
					  , public SingletonTemplate< ErrorLogManager >
{
	friend ErrorLogManager* SingletonTemplate< ErrorLogManager >::GetInstance();
	friend void SingletonTemplate< ErrorLogManager >::Kill();
public:
    std::stringstream logBuffer_;
    std::ofstream logFile_;

    void CreateLogFile( std::string filename );
    void Flush();
    void Close();
    void LogException( const cException& e );
    std::string GetTimeStr();

private:
	ErrorLogManager(const ErrorLogManager&){};
	ErrorLogManager& operator=(const ErrorLogManager&){};
	ErrorLogManager(){};
	virtual ~ErrorLogManager(){};
};

#endif // ErrorLogManager.h