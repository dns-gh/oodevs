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
    class Ofstream;
}

namespace sword
{
    class SimToClient;
}

namespace plugins
{
namespace logistic
{

class ConsignArchive : private boost::noncopyable
{
public:
    ConsignArchive( const tools::Path& basePath, uint32_t maxSize );
    virtual ~ConsignArchive();

    void Write( const void* data, uint32_t length );
    void Flush();

private:
    tools::Path basePath_;
    uint32_t maxSize_;
    uint32_t size_;
    uint32_t index_;
    std::unique_ptr< tools::Ofstream > output_;
};

} // namespace logistic
} // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNARCHIVE_H
