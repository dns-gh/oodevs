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
#include <boost/shared_ptr.hpp>
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
    ConsignArchive( const tools::Path& baseDir, uint32_t maxSize );
    // Read-only mode.
    ConsignArchive( const tools::Path& baseDir );
    virtual ~ConsignArchive();

    ConsignOffset Write( const void* data, uint32_t length );
    // Calls "callback" with bytes read for each valid entry in offsets.
    // Sequence order is preserved but the caller should try to have offsets
    // grouped by files by ascending offsets to avoid seeking back and forth.
    void ReadMany( const std::vector< ConsignOffset >& offsets,
        const std::function< void( const std::vector< uint8_t >& )>& callback ) const;
    void ReadAll( const std::function<
        void( ConsignOffset, const std::vector< uint8_t > )>& callback ) const;
    void ReadRange( ConsignOffset start, ConsignOffset end,
        const std::function< void( ConsignOffset, const std::vector< uint8_t > )>& callback ) const;
    void Flush();
    // Reads the offset file and returns its content. Returned file index is not
    // zero on success.
    ConsignOffset ReadOffsetFile() const;

private:
    tools::Path GetFilename( uint32_t index ) const;
    boost::shared_ptr< tools::Fstream > GetFile( uint32_t index ) const;

private:
    tools::Path baseDir_;
    tools::Path basePath_;
    uint32_t maxSize_;
    uint32_t size_;
    uint32_t index_;
    boost::shared_ptr< tools::Fstream > output_;
    bool readOnly_;
};

// Writes a file containing the file index and last valid byte offset within
// this file as atomically as possible. Since rename() has no atomicity
// guarantee under windows, we unlink the target and move a temporary file at
// its place instead. This leaves a window where no offset file exists but this
// is better than writing an invalid one.
bool WriteOffsetFile( const tools::Path& path, uint32_t file, uint32_t offset );

// Reads an offset file and return the file and data offset on success.
bool ReadOffsetFile( const tools::Path& path, uint32_t& file, uint32_t& offset );

} // namespace logistic
} // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNARCHIVE_H
