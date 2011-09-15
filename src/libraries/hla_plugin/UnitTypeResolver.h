// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_UnitTypeResolver_h
#define plugins_hla_UnitTypeResolver_h

#include "UnitTypeResolver_ABC.h"

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace tools
{
    template< typename T, typename I > class Resolver_ABC;
}

namespace kernel
{
    class AgentType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  UnitTypeResolver
    @brief  Unit type resolver
*/
// Created: SLI 2011-09-15
// =============================================================================
class UnitTypeResolver : public UnitTypeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< kernel::AgentType, std::string >& agentTypeResolver );
    virtual ~UnitTypeResolver();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long Resolve( const rpr::EntityType& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    const rpr::EntityTypeResolver_ABC& entityTypeResolver_;
    const tools::Resolver_ABC< kernel::AgentType, std::string >& agentTypeResolver_;
    const unsigned long defaultType_;
    //@}
};

}
}

#endif // plugins_hla_UnitTypeResolver_h
