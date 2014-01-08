// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTICPLUGIN_CONSIGNRECORDER_H
#define LOGISTICPLUGIN_CONSIGNRECORDER_H

#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_vector.hpp>
#pragma warning( pop )
#include <boost/container/deque.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace sword
{
    class LogHistoryEntry;
}

namespace boost
{
    namespace gregorian
    {
        class date;
    }
}

namespace tools
{
    class Path;
}

namespace plugins
{
namespace logistic
{

class ConsignArchive;
struct ConsignOffset;
class ConsignResolver_ABC;

struct ConsignRecord
{
    int32_t tick;
    uint32_t requestId;
    uint32_t file;
    uint32_t offset;
};

// Indexes and persists the modification history of logistic requests. History
// entries can be retrieved from requests or referenced entities identifiers.
class ConsignRecorder: private boost::noncopyable
{
public:
    ConsignRecorder( const tools::Path& archiveDir, uint32_t maxSize, uint32_t maxConsigns,
           uint32_t maxHistory );
    // Create recorder in indexer mode only, inputs will not be recorded to disk,
    // and WriteEntry will throw an exception.
    ConsignRecorder( const tools::Path& archiveDir, uint32_t maxConsigns, uint32_t maxHistory );
    virtual ~ConsignRecorder();

    void Flush();
    // In read-only mode, checks the underlying archive for new entries and loads
    // them. The last loaded entry offset is remembered as a starting point for
    // the next call. Do nothing in write mode.
    void ReadNewEntries();

    void WriteEntry( uint32_t requestId, bool destroyed,
            const sword::LogHistoryEntry& entry, std::vector< uint32_t >& entities );
    // Returns the list of requests referencing supplied entities.
    void GetRequestIdsFromEntities( const std::set< uint32_t >& entities,
            std::set< uint32_t >& requests ) const;
    // Returns top maxCount entries of recently updated requests.
    void GetRequests( const std::set< uint32_t >& requestIds, int32_t currentTick,
            size_t maxCount, boost::ptr_vector< sword::LogHistoryEntry>& entries ) const;
    // Returns all entries of a given request.
    void GetHistory( uint32_t requestId, boost::ptr_vector< sword::LogHistoryEntry >& entries ) const;
    size_t GetHistorySize() const;

private:
    // Append entries referenced by supplied offsets to "entries", in the same
    // order.
    void AppendEntries( const std::vector< ConsignOffset >& offsets, 
        boost::ptr_vector< sword::LogHistoryEntry >& entries ) const;

    void IndexEntry( uint32_t requestId, bool destroyed, const ConsignOffset& offset,
            const sword::LogHistoryEntry& entry, std::vector< uint32_t >& entities );
    void UpdateRequestIndex( uint32_t requestId, const ConsignOffset& offset,
           bool destroyed, const std::vector< uint32_t >& entities );
    void UpdateHistoryIndex( uint32_t requestId, int32_t tick, const ConsignOffset& offset );

private:
    std::unique_ptr< ConsignArchive > archive_;

    // Maintain a list of most recently update consigns history. "destroyed_",
    // and "alive_" list most recently updated destroyed/alive consigns, while
    // consigns_ maps into either one depending on ConsignHistory::destroyed
    // field.
    class ConsignHistory;
    typedef std::list< ConsignHistory > T_LRU;
    T_LRU alive_;
    T_LRU destroyed_;
    std::unordered_map< uint32_t, T_LRU::iterator > consigns_;
    const size_t maxConsigns_;

    // Map entity (unit, formation, whatever) identifiers to requests
    std::multimap< uint32_t, uint32_t > entityConsigns_;

    // A bounded FIFO queue of request states sorted by descending ticks. Note
    // that history_ and consign_ referenced states are overlapping but not equal.
    boost::container::deque< ConsignRecord > history_;
    const size_t maxHistory_;

    // In read-only mode, the offset of the most recently loaded entry. NULL
    // in write mode.
    std::unique_ptr< ConsignOffset > reloadedEnd_;
};

void GetRequestsFromEntities( const ConsignRecorder& rec, const std::set< uint32_t >& entities,
    int32_t currentTick, size_t maxCount, boost::ptr_vector< sword::LogHistoryEntry >& entries );

uint32_t GetConsignId( const sword::LogHistoryEntry& entry );
bool IsConsignDestroyed( const sword::LogHistoryEntry& entry );
void AppendConsignEntities( const sword::LogHistoryEntry& entry,
        std::vector< uint32_t >& entities );

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNRECORDER_H
