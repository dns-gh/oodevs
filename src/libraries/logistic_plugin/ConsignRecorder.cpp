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
#include "protocol/MessageParameters.h"
#include "protocol/Simulation.h"
#include <tools/Exception.h>
#include <tools/Helpers.h>
#include <tools/Path.h>
#include <boost/make_shared.hpp>
#include <algorithm>
#include <unordered_set>

using namespace plugins::logistic;

class ConsignRecorder::ConsignHistory
{
public:
    ConsignHistory( uint32_t i )
        : id_( i )
        , destroyed_( false )
    {
    }

    std::vector< ConsignOffset > records_;
    // List of entities referenced by this consigns. They mirror the entries
    // in entityConsigns_.
    std::vector< uint32_t > entities_;
    uint32_t id_;
    bool destroyed_;
};

ConsignRecorder::ConsignRecorder( const tools::Path& archivePath,
        uint32_t maxSize, uint32_t maxConsigns, uint32_t maxHistory )
    : archive_( new ConsignArchive( archivePath, maxSize ))
    , maxConsigns_( maxConsigns )
    , maxHistory_( maxHistory )
{
}

ConsignRecorder::ConsignRecorder( const tools::Path& archivePath,
        uint32_t maxConsigns, uint32_t maxHistory )
    : archive_( new ConsignArchive( archivePath ))
    , maxConsigns_( maxConsigns )
    , maxHistory_( maxHistory )
{
    std::vector< uint32_t > entities;
    archive_->ReadAll( [&]( ConsignOffset offset, const std::vector< uint8_t >& output )
    {
        sword::LogHistoryEntry entry;
        if( !entry.ParseFromArray( &output[0], static_cast< int >( output.size() )))
            return;
        const auto id = GetConsignId( entry );
        entities.clear();
        AppendConsignEntities( entry, entities );
        IndexEntry( id, IsConsignDestroyed( entry ), offset, entry, entities );
    });
}

ConsignRecorder::~ConsignRecorder()
{
}

void ConsignRecorder::Flush()
{
    archive_->Flush();
}

void ConsignRecorder::WriteEntry( uint32_t requestId, bool destroyed,
        const sword::LogHistoryEntry& entry, std::vector< uint32_t >& entities )
{
    if( !entry.IsInitialized() )
        throw MASA_EXCEPTION( "input entry is not initialized: " + entry.ShortDebugString() );
    std::vector< uint8_t > buffer( entry.ByteSize() );
    if( !entry.SerializeWithCachedSizesToArray( &buffer[0] ) )
        throw MASA_EXCEPTION( "could not serialize input message: " + entry.ShortDebugString() );
    const auto offset = archive_->Write( &buffer[0], static_cast< uint32_t >( buffer.size() ));

    IndexEntry( requestId, destroyed, offset, entry, entities );
}

void ConsignRecorder::IndexEntry( uint32_t requestId, bool destroyed,
        const ConsignOffset& offset, const sword::LogHistoryEntry& entry,
        std::vector< uint32_t >& entities )
{
    if( !entry.IsInitialized() )
        throw MASA_EXCEPTION( "input entry is not initialized: " + entry.ShortDebugString() );
    UpdateRequestIndex( requestId, offset, destroyed, entities );
    UpdateHistoryIndex( requestId, entry.tick(), offset );
}

void ConsignRecorder::UpdateRequestIndex( uint32_t requestId, const ConsignOffset& offset,
       bool destroyed, const std::vector< uint32_t >& entities )
{
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
    // Despite the standard saying splice does not invalidate iterators in the
    // 3 arguments version, MSVC believes it does in debug mode. Refresh it
    // to avoid spurious assertions.
    ic->second = std::prev( dest.end() );

    // Record and index new entities
    auto& known = ic->second->entities_;
    for( auto it = entities.cbegin(); it != entities.cend(); ++it )
    {
        // Entity lists are small, linear search is OK here
        if( std::find( known.begin(), known.end(), *it ) != known.end() )
            continue;
        known.push_back( *it );
        entityConsigns_.insert( std::make_pair( *it, requestId ));
    }

    // Remove extra entries
    T_LRU* const lists[] = { &destroyed_, &alive_ };
    for( int i = 0; i != COUNT_OF( lists ); ++i )
    {
        T_LRU& list = *lists[i];
        while( consigns_.size() > maxConsigns_ && !list.empty() )
        {
            const auto& e = list.front();
            consigns_.erase( e.id_ );
            // Clean entities -> request index
            for( auto it = e.entities_.cbegin(); it != e.entities_.cend(); ++it )
            {
                auto values = entityConsigns_.equal_range( *it );
                for( auto ic = values.first; ic != values.second; ++ic )
                    if( ic->second == e.id_ )
                        ic = entityConsigns_.erase( ic );
                    else
                        ++ic;
            }
            list.pop_front();
        }
    }
}

void ConsignRecorder::UpdateHistoryIndex( uint32_t requestId, int32_t tick,
        const ConsignOffset& offset )
{
    if( !history_.empty() && history_.front().tick > tick )
        throw MASA_EXCEPTION( "consign history is moving backward in time" );

    ConsignRecord rec;
    rec.tick = tick;
    rec.requestId = requestId;
    rec.file = offset.file;
    rec.offset = offset.offset;

    history_.push_front( rec );
    while( history_.size() > maxHistory_ )
        history_.pop_back();
}

void ConsignRecorder::GetRequestIdsFromEntities( const std::set< uint32_t >& entities,
        std::set< uint32_t >& requests ) const
{
    for( auto it = entities.cbegin(); it != entities.cend(); ++it )
    {
        const auto values = entityConsigns_.equal_range( *it );
        for( auto ic = values.first; ic != values.second; ++ic )
            requests.insert( ic->second );
    }
}

namespace
{

bool CompareTick( const ConsignRecord& rec, int32_t tick )
{
    return rec.tick > tick;
}

}  // namespace

void ConsignRecorder::GetRequests( const std::set< uint32_t >& requestIds, int32_t currentTick,
        size_t maxCount, boost::ptr_vector< sword::LogHistoryEntry >& entries ) const
{
    entries.clear();
    std::unordered_set< uint32_t > seen;
    std::vector< ConsignOffset > offsets;
    offsets.reserve( maxCount );

    auto ih = history_.cbegin();
    if( currentTick >= 0 )
        ih = std::lower_bound( history_.cbegin(), history_.cend(), currentTick, CompareTick );
    for( ; ih != history_.end(); ++ih )
    {
        // Keep only the latest state of a given request
        if( !seen.insert( ih->requestId ).second )
            continue;
        if( requestIds.find( ih->requestId ) == requestIds.end() )
            continue;
        ConsignOffset offset;
        offset.file = ih->file;
        offset.offset = ih->offset;
        offsets.push_back( offset );
        if( offsets.size() >= maxCount )
            break;
    }

    // Make sure to seek in increasing offset order.
    std::reverse( offsets.begin(), offsets.end() );
    AppendEntries( offsets, entries );
    std::reverse( entries.base().begin(), entries.base().end() );
}

void ConsignRecorder::GetHistory( uint32_t requestId,
        boost::ptr_vector< sword::LogHistoryEntry >& entries ) const
{
    entries.clear();
    const auto ic = consigns_.find( requestId );
    if( ic == consigns_.end() )
        return;
    // records_ is naturally sorted by ascending (file, offset)
    AppendEntries( ic->second->records_, entries );
}

void ConsignRecorder::AppendEntries( const std::vector< ConsignOffset >& offsets, 
    boost::ptr_vector< sword::LogHistoryEntry >& entries ) const
{
    archive_->ReadMany( offsets, [&]( const std::vector< uint8_t >& buffer )
    {
        std::auto_ptr< sword::LogHistoryEntry > entry( new sword::LogHistoryEntry() );
        if( entry->ParseFromArray( &buffer[0], static_cast< int >( buffer.size() )))
            entries.push_back( entry );
    });
}

size_t ConsignRecorder::GetHistorySize() const
{
    return consigns_.size();
}

void plugins::logistic::GetRequestsFromEntities( const ConsignRecorder& rec,
        const std::set< uint32_t >& entities, int32_t currentTick,
        size_t maxCount, boost::ptr_vector< sword::LogHistoryEntry >& entries )
{
    // Resolve request identifiers
    std::set< uint32_t > requestIds;
    rec.GetRequestIdsFromEntities( entities, requestIds );
    // Fetch most recent state of each request
    rec.GetRequests( requestIds, currentTick, maxCount, entries );
}

uint32_t plugins::logistic::GetConsignId( const sword::LogHistoryEntry& entry )
{
    if( entry.has_funeral() && entry.funeral().has_creation() )
        return entry.funeral().creation().request().id();
    if( entry.has_maintenance() && entry.maintenance().has_creation() )
        return entry.maintenance().creation().request().id();
    if( entry.has_medical() && entry.medical().has_creation() )
        return entry.medical().creation().request().id();
    if( entry.has_supply() && entry.supply().has_creation() )
        return entry.supply().creation().request().id();
    return 0;
}

bool plugins::logistic::IsConsignDestroyed( const sword::LogHistoryEntry& entry )
{
    return ( entry.has_funeral() && entry.funeral().has_destruction() ) ||
           ( entry.has_maintenance() && entry.maintenance().has_destruction() ) ||
           ( entry.has_medical() && entry.medical().has_destruction() ) ||
           ( entry.has_supply() && entry.supply().has_destruction() );
}

void plugins::logistic::AppendConsignEntities( const sword::LogHistoryEntry& msg,
        std::vector< uint32_t >& entities )
{
    if( msg.has_funeral() )
    {
        if( msg.funeral().has_creation() )
            entities.push_back( msg.funeral().creation().unit().id() );
        if( msg.funeral().has_update() )
        {
            const auto& sub = msg.funeral().update();
            if( sub.has_handling_unit() )
                entities.push_back( protocol::GetParentEntityId( sub.handling_unit() ));
            if( sub.has_convoying_unit() )
                entities.push_back( sub.convoying_unit().id() );
        }
    }
    else if( msg.has_maintenance() )
    {
        if( msg.maintenance().has_creation() )
            entities.push_back( msg.maintenance().creation().unit().id() );
        if( msg.maintenance().has_update() )
        {
            const auto& sub = msg.maintenance().update();
            entities.push_back( sub.unit().id() );
            entities.push_back( sub.provider().id() );
        }
    }
    else if( msg.has_medical() )
    {
        if( msg.medical().has_creation() )
            entities.push_back( msg.medical().creation().unit().id() );
        if( msg.medical().has_update() )
        {
            const auto& sub = msg.medical().update();
            entities.push_back( sub.unit().id() );
            if( sub.has_provider() )
                entities.push_back( sub.provider().id() );
        }
    }
    else if( msg.has_supply() )
    {
        if( msg.supply().has_creation() )
        {
            const auto& sub = msg.supply().creation();
            entities.push_back( protocol::GetParentEntityId( sub.supplier() ));
            entities.push_back( protocol::GetParentEntityId( sub.transporters_provider() ));
        }
        if( msg.supply().has_update() )
        {
            const auto& sub = msg.supply().update();
            if( sub.has_convoyer() )
                entities.push_back( sub.convoyer().id() );
            if( sub.has_requests() )
            {
                const int count = sub.requests().requests().size();
                for( int i = 0; i != count; ++i )
                    entities.push_back( sub.requests().requests( i ).recipient().id() );
            }
        }
    }
}
