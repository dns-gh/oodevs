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
#include <tools/Helpers.h>
#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace bg = boost::gregorian;
using namespace plugins::logistic;

class ConsignRecorder::ConsignHistory
{
public:
    ConsignHistory( uint32_t i )
        : id_( i )
        , destroyed_( false )
    {
    }

    std::vector< ConsignArchive::Offset > records_;
    uint32_t id_;
    bool destroyed_;
};

ConsignRecorder::ConsignRecorder( const tools::Path& archivePath,
        uint32_t maxSize, uint32_t maxConsigns )
    : archive_( new ConsignArchive( archivePath, maxSize ))
    , maxConsigns_( maxConsigns )
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

void ConsignRecorder::WriteEntry( uint32_t requestId, bool destroyed,
        const sword::LogHistoryEntry& entry )
{
    if( !entry.IsInitialized() )
        throw MASA_EXCEPTION( "input entry is not initialized: " + entry.ShortDebugString() );
    std::vector< uint8_t > buffer( entry.ByteSize() );
    if( !entry.SerializeWithCachedSizesToArray( &buffer[0] ) )
        throw MASA_EXCEPTION( "could not serialize input message: " + entry.ShortDebugString() );
    const auto offset = archive_->Write( &buffer[0], static_cast< uint32_t >( buffer.size() ));

    // Now, get the record file/offset/length and add it to the history,
    // possibly evicting entries, from oldest deleted to newest alive. 

    // Push a new record and update the LRU
    auto ic = consigns_.find( requestId );
    if( ic == consigns_.end() )
    {
        alive_.push_back( ConsignHistory( requestId ) );
        ic = consigns_.insert( std::make_pair( requestId, std::prev( alive_.end() ) )).first;
    }
    auto& source = ic->second->destroyed_ ? destroyed_ : alive_;
    ic->second->records_.push_back( offset );
    if( destroyed )
        ic->second->destroyed_ = true;
    auto& dest = ic->second->destroyed_ ? destroyed_ : alive_;
    dest.splice( dest.end() , source, ic->second );

    // Remove extra entries
    T_LRU* const lists[] = { &destroyed_, &alive_ };
    for( int i = 0; i != COUNT_OF( lists ); ++i )
    {
        T_LRU& list = *lists[i];
        while( consigns_.size() > maxConsigns_ && !list.empty() )
        {
            consigns_.erase( list.front().id_ );
            list.pop_front();
        }
    }
}

void ConsignRecorder::GetHistory( uint32_t requestId,
        boost::ptr_vector< sword::LogHistoryEntry >& entries ) const
{
    entries.clear();
    const auto ic = consigns_.find( requestId );
    if( ic == consigns_.end() )
        return;
    const ConsignHistory& c = *ic->second;
    std::vector< uint8_t > buffer;
    for( auto it = c.records_.cbegin(); it != c.records_.cend(); ++it )
    {
        if( !archive_->Read( it->file, it->offset, buffer ) )
            continue;
        std::auto_ptr< sword::LogHistoryEntry > entry( new sword::LogHistoryEntry() );
        if( !entry->ParseFromArray( &buffer[0], static_cast< int >( buffer.size() )))
            continue;
        entries.push_back( entry );
    }
}

size_t ConsignRecorder::GetHistorySize() const
{
    return consigns_.size();
}

