// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AggregateFactory_h
#define plugins_hla_AggregateFactory_h

#include "AggregateFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AggregateFactory
    @brief  Aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class AggregateFactory : public AggregateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateFactory();
    virtual ~AggregateFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< Aggregate_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const;
    //@}
};

}
}

#endif // plugins_hla_AggregateFactory_h
