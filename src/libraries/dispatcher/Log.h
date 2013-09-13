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

#include "tools/FileLog.h"
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>

namespace tools
{
    class Path;
}

namespace dispatcher
{
// =============================================================================
/** @class  Log
    @brief  Log definition
*/
// Created: MCO 2011-06-27
// =============================================================================
class Log : public tools::FileLog
{
public:
    Log( const tools::Path& filename, bool sizeInBytes );

protected:
    virtual std::size_t Write( const std::string& line );

private:
    void Close();

    std::string GetTime() const;

private:
    tools::Ofstream s_;
    bool sizeInBytes_;
};

}

#endif // dispatcher_Log_h
