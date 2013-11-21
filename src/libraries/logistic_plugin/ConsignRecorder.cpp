// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ConsignRecorder.h"
#include "ConsignArchive.h"
#include "ConsignResolver_ABC.h"
#include "protocol/Simulation.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace bg = boost::gregorian;

namespace plugins
{
namespace logistic
{

ConsignRecorder::ConsignRecorder( const tools::Path& archivePath, uint32_t maxSize )
    : archive_( new ConsignArchive( archivePath, maxSize ))
{
}

ConsignRecorder::~ConsignRecorder()
{
}

void ConsignRecorder::AddLogger( int kind, const tools::Path& path, const std::string header )
{
    loggers_.insert( kind, std::auto_ptr< ConsignResolver_ABC >(
        new ConsignResolver_ABC( path, header )));
}

bool ConsignRecorder::HasLogger( int kind ) const
{
    return loggers_.find( kind ) != loggers_.end();
}

void ConsignRecorder::Write( int kind, const std::string& data, const bg::date& today )
{
    auto it = loggers_.find( kind );
    if( it == loggers_.end() )
        return;
    it->second->Write( data, today );
}

void ConsignRecorder::Flush()
{
    archive_->Flush();
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->Flush();
}

void ConsignRecorder::SetMaxLinesInFile( int maxLines )
{
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->SetMaxLinesInFile( maxLines );
}

void ConsignRecorder::WriteEntry( const sword::LogHistoryEntry& entry )
{
    if( !entry.IsInitialized() )
        throw MASA_EXCEPTION( "input entry is not initialized: " + entry.ShortDebugString() );
    std::vector< uint8_t > buffer( entry.ByteSize() );
    if( !entry.SerializeWithCachedSizesToArray( &buffer[0] ) )
        throw MASA_EXCEPTION( "could not serialize input message: " + entry.ShortDebugString() );
    archive_->Write( &buffer[0], static_cast< uint32_t >( buffer.size() ));
}

}  // namespace logistic
}  // namespace plugins
