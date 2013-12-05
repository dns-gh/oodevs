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
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>
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

// Indexes and persists the modification history of logistic requests. History
// entries can be retrieved from requests or referenced entities identifiers.
class ConsignRecorder: private boost::noncopyable
{
public:
    ConsignRecorder( const tools::Path& archivePath, uint32_t maxSize, uint32_t maxConsigns );
    virtual ~ConsignRecorder();

    void AddLogger( int kind, const tools::Path& path, const std::string header );
    bool HasLogger( int kind ) const;
    void Write( int kind, const std::string& data, const boost::gregorian::date& today );
    void Flush();
    void SetMaxLinesInFile( int maxLines );

    void WriteEntry( uint32_t requestId, bool destroyed, const sword::LogHistoryEntry& entry,
           std::vector< uint32_t >& entities );
    // Returns the list of requests referencing supplied entities.
    void GetRequestIdsFromEntities( const std::set< uint32_t >& entities,
            std::set< uint32_t >& requests ) const;
    // Returns top maxCount entries of recently updated requests.
    void GetRequests( const std::set< uint32_t >& requestIds, size_t maxCount,
            boost::ptr_vector< sword::LogHistoryEntry>& entries ) const;
    // Returns all entries of a given request.
    void GetHistory( uint32_t requestId, boost::ptr_vector< sword::LogHistoryEntry >& entries ) const;
    size_t GetHistorySize() const;

private:
    // Append entries referenced by supplied offsets to "entries", in the same
    // order.
    void AppendEntries( const std::vector< ConsignOffset >& offsets, 
        boost::ptr_vector< sword::LogHistoryEntry >& entries ) const;

private:
    boost::ptr_map< int, ConsignResolver_ABC > loggers_;
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
    size_t maxConsigns_;

    // Map entity (unit, formation, whatever) identifiers to requests
    std::multimap< uint32_t, uint32_t > entityConsigns_;
};

void GetRequestsFromEntities( const ConsignRecorder& rec, const std::set< uint32_t >& entities,
        size_t maxCount, boost::ptr_vector< sword::LogHistoryEntry >& entries );

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNRECORDER_H
