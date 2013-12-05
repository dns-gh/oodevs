// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTICPLUGIN_CONSIGNARCHIVE_H
#define LOGISTICPLUGIN_CONSIGNARCHIVE_H

#include <tools/Path.h>
#include <boost/noncopyable.hpp>
#include <memory>
#include <cstdint>

namespace tools
{
    class Fstream;
}

namespace sword
{
    class SimToClient;
}

namespace plugins
{
namespace logistic
{

struct ConsignOffset
{
    uint32_t file;
    uint32_t offset;
};

class ConsignArchive : private boost::noncopyable
{
public:
    ConsignArchive( const tools::Path& basePath, uint32_t maxSize );
    virtual ~ConsignArchive();

    ConsignOffset Write( const void* data, uint32_t length );
    // Calls "callback" with bytes read for each valid entry in offsets.
    // Sequence order is preserved but the caller should try to have offsets
    // grouped by files by ascending offsets to avoid seeking back and forth.
    void ReadMany( const std::vector< ConsignOffset >& offsets,
            const std::function< void( std::vector< uint8_t >& )>& callback ) const;
    void Flush();

private:
    tools::Path GetFilename( uint32_t index ) const;
    boost::shared_ptr< tools::Fstream > GetFile( uint32_t index ) const;

private:
    tools::Path basePath_;
    uint32_t maxSize_;
    uint32_t size_;
    uint32_t index_;
    std::unique_ptr< tools::Fstream > output_;
};

} // namespace logistic
} // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNARCHIVE_H
