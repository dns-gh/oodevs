// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Log_h
#define dispatcher_Log_h

#include "Log_ABC.h"
#include "tools/Log.h"

namespace dispatcher
{
    class Config;

// =============================================================================
/** @class  Log
    @brief  Log definition
*/
// Created: MCO 2011-06-27
// =============================================================================
class Log : public Log_ABC
{
public:
    explicit Log( const Config& config );
    virtual ~Log();

private:
    virtual void DoWrite( const std::string& line );

private:
    tools::Log log_;
};

}

#endif // dispatcher_Log_h
