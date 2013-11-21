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
#pragma warning( pop )
#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <cstdint>

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
    ConsignRecorder( const tools::Path& archivePath, uint32_t maxSize );
    virtual ~ConsignRecorder();

    void AddLogger( int kind, const tools::Path& path, const std::string header );
    bool HasLogger( int kind ) const;
    void Write( int kind, const std::string& data, const boost::gregorian::date& today );
    void Flush();
    void SetMaxLinesInFile( int maxLines );

    void WriteEntry( const sword::LogHistoryEntry& entry );

private:
    boost::ptr_map< int, ConsignResolver_ABC > loggers_;
    std::unique_ptr< ConsignArchive > archive_;
};

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNRECORDER_H
