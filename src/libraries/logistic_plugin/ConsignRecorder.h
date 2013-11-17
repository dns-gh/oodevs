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
class ConsignResolver_ABC;

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

    void WriteEntry( uint32_t requestId, bool destroyed, const sword::LogHistoryEntry& entry );
    void GetHistory( uint32_t requestId, boost::ptr_vector< sword::LogHistoryEntry >& entries ) const;
    size_t GetHistorySize() const;

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
};

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNRECORDER_H
