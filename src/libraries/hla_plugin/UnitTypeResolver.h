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
#include "dispatcher/Logger_ABC.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include <tools/Resolver_ABC.h>

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
template< typename T >
class UnitTypeResolver : public UnitTypeResolver_ABC
{
public:
    typedef T UnitType;
    //! @name Constructors/Destructor
    //@{
             UnitTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< UnitType, std::string >& agentTypeResolver,
                               dispatcher::Logger_ABC& logger );
    virtual ~UnitTypeResolver();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long Resolve( const rpr::EntityType& type ) const;
    virtual unsigned long Resolve( const std::string& typeName ) const;
    //@}

private:
    //! @name Member data
    //@{
    const rpr::EntityTypeResolver_ABC& entityTypeResolver_;
    const tools::Resolver_ABC< UnitType, std::string >& agentTypeResolver_;
    dispatcher::Logger_ABC& logger_;
    const unsigned long defaultType_;
    //@}
};

namespace details
{
// -----------------------------------------------------------------------------
// Name: UnitTypeResolver destructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
template< typename T >
unsigned long GetDefault( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< T, std::string >& agentTypeResolver )
{
    std::string defaultType;
    entityTypeResolver.Resolve( rpr::EntityType(), defaultType );
    const T* agentType = agentTypeResolver.Find( defaultType );
    if( agentType == 0 )
        throw MASA_EXCEPTION( "Agent type identifier '" + defaultType + "' not found, please check your physical model." );
    return agentType->GetId();
}
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver constructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
template< typename T >
UnitTypeResolver< T >::UnitTypeResolver( const rpr::EntityTypeResolver_ABC& entityTypeResolver, const tools::Resolver_ABC< UnitType, std::string >& agentTypeResolver,
                                dispatcher::Logger_ABC& logger )
    : entityTypeResolver_( entityTypeResolver )
    , agentTypeResolver_ ( agentTypeResolver )
    , defaultType_       ( details::GetDefault( entityTypeResolver, agentTypeResolver ) )
    , logger_            ( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver destructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
template< typename T >
UnitTypeResolver< T >::~UnitTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTypeResolver::Resolve
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
template< typename T >
unsigned long UnitTypeResolver< T >::Resolve( const rpr::EntityType& type ) const
{
    std::string defaultType;
    if( !entityTypeResolver_.Resolve( type, defaultType ) )
        logger_.LogWarning( std::string( "Could not find Unit for EntityType ") + type.str() );
    const UnitType* agentType = agentTypeResolver_.Find( defaultType );
    if( agentType == 0 )
        return defaultType_;
    return agentType->GetId();
}

// -----------------------------------------------------------------------------
// Name: AutomatTypeResolver::Resolve
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
template< typename T >
unsigned long UnitTypeResolver< T >::Resolve( const std::string& typeName ) const
{
    UnitType* type = agentTypeResolver_.Find( typeName );
    if( type == 0 )
        return 0;
    return type->GetId();
}

}
}

#endif // plugins_hla_UnitTypeResolver_h
