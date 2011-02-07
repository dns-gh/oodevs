// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_Agent_ABC_h
#define hla_plugin_Agent_ABC_h

#include "dispatcher/Observable.h"
#include "protocol/Protocol.h"
#include "tools/Resolver.h"
#include <geometry/Types.h>
#include <string>

namespace kernel
{
    class Karma;
}

namespace dispatcher
{
    class Equipment;
}

namespace plugins
{
namespace hla
{
    class EventListener_ABC;

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent definition
*/
// Created: SLI 2011-02-04
// =============================================================================
class Agent_ABC : public dispatcher::Observable< sword::UnitAttributes >
                , public dispatcher::Observable< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC() {}
    virtual ~Agent_ABC() {}
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener ) = 0;
    virtual void Unregister( EventListener_ABC& listener ) = 0;
    //@}

    //! @name Getters
    //@{
    virtual const tools::Resolver< dispatcher::Equipment >& GetEquipments() const = 0;
    //@}
};

}
}

#endif // hla_plugin_Agent_ABC_h
