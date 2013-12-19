// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ConsignCsvLogger.h"
#include "ConsignResolver_ABC.h"
#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bg = boost::gregorian;
using namespace plugins::logistic;

ConsignCsvLogger::ConsignCsvLogger()
{
}

ConsignCsvLogger::~ConsignCsvLogger()
{
}

void ConsignCsvLogger::AddLogger( int kind, const tools::Path& path,
        const std::string header )
{
    loggers_.insert( kind, std::auto_ptr< ConsignResolver_ABC >(
        new ConsignResolver_ABC( path, header )));
}

bool ConsignCsvLogger::HasLogger( int kind ) const
{
    return loggers_.find( kind ) != loggers_.end();
}

void ConsignCsvLogger::Write( int kind, const std::string& data,
        const boost::gregorian::date& today )
{
    auto it = loggers_.find( kind );
    if( it == loggers_.end() )
        return;
    it->second->Write( data, today );
}

void ConsignCsvLogger::Flush()
{
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->Flush();
}

void ConsignCsvLogger::SetMaxLinesInFile( int maxLines )
{
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->SetMaxLinesInFile( maxLines );
}
