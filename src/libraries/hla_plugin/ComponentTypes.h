// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ComponentTypes_h
#define plugins_hla_ComponentTypes_h

#include "ComponentTypes_ABC.h"

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace kernel
{
    class AgentType;
    class BreakdownType;
    class EquipmentType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ComponentTypes
    @brief  Component types
*/
// Created: SLI 2011-09-30
// =============================================================================
class ComponentTypes : public ComponentTypes_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ComponentTypes( const tools::Resolver_ABC< kernel::AgentType, unsigned long >& agentTypes,
            const tools::Resolver_ABC< kernel::EquipmentType, unsigned long >& equipmentTypes,
            const tools::Resolver_ABC< kernel::BreakdownType, std::string >& breakdownsTypes );
    virtual ~ComponentTypes();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( unsigned int agentTypeIdentifier, ComponentTypeVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::AgentType, unsigned long >& agentTypes_;
    const tools::Resolver_ABC< kernel::EquipmentType, unsigned long >& equipmentTypes_;
    const tools::Resolver_ABC< kernel::BreakdownType, std::string >& breakdownsTypes_;
    //@}
};

}
}

#endif // plugins_hla_ComponentTypes_h
