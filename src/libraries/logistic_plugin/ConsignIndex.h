// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTICPLUGIN_CONSIGNINDEX_H
#define LOGISTICPLUGIN_CONSIGNINDEX_H

#include "LogisticPlugin.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <map>
#include <vector>

namespace sword
{
    class LogHistoryEntry;
    class SimToClient;
}

namespace plugins
{
namespace logistic
{

enum E_ConsignEvent
{
    eConsignUnknown,
    eConsignCreation,
    eConsignUpdate,
    eConsignDestruction,
};

struct ConsignEvent
{
    ConsignEvent( LogisticPlugin::E_LogisticType t, E_ConsignEvent a, int i );

    LogisticPlugin::E_LogisticType type;
    E_ConsignEvent action;
    int id;
    boost::shared_ptr< const sword::LogHistoryEntry > entry;
};

class ConsignIndex: private boost::noncopyable
{
public:
    ConsignIndex();
    virtual ~ConsignIndex();

    ConsignEvent Update( const sword::SimToClient& message, int tick );

private:
    std::map< int, boost::shared_ptr< sword::LogHistoryEntry > > consigns_;
};

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNINDEX_H
