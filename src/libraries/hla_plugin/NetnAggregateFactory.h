// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnAggregateFactory_h
#define plugins_hla_NetnAggregateFactory_h

#include "AggregateFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnAggregateFactory
    @brief  NETN aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnAggregateFactory : public AggregateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnAggregateFactory( const AggregateFactory_ABC& factory );
    virtual ~NetnAggregateFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< Aggregate_ABC > Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    const AggregateFactory_ABC& factory_;
    //@}
};

}
}

#endif // plugins_hla_NetnAggregateFactory_h
