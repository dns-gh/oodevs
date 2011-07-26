// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AggregateFactory_ABC_h
#define plugins_hla_AggregateFactory_ABC_h

#include "rpr/ForceIdentifier.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <memory>

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class Aggregate_ABC;
    class Agent_ABC;

// =============================================================================
/** @class  AggregateFactory_ABC
    @brief  Aggregate factory definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class AggregateFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateFactory_ABC() {}
    virtual ~AggregateFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< Aggregate_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const = 0;
    //@}
};

}
}

#endif // __AggregateFactory_ABC_h_
