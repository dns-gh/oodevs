// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTICPLUGIN_CONSIGNSTATE_H
#define LOGISTICPLUGIN_CONSIGNSTATE_H

#include <cstdint>

namespace plugins
{
namespace logistic
{

class ConsignState
{
public:
     ConsignState()
        : id_( 0 )
        , startTick_( -1 )
        , endTick_( -1 )
        , status_( -1 )
        , handlerId_( 0 )
    {
    }
    ~ConsignState()
    {
    }

    uint32_t id_;
    int startTick_;
    int endTick_;
    int status_;
    unsigned int handlerId_;

};

}  // namespace logistic
}  // namespace plugins

#endif // LOGISTICPLUGIN_CONSIGNSTATE_H
