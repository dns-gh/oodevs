// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_SINKROLEEXTENDER_H
#define SWORD_SINKROLEEXTENDER_H

#include "Entities/Agents/RoleExtender_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

#include <map>

namespace sword
{
/// =============================================================================
/// @class SinkRoleExtender
/// @brief SinkRoleExtender
///
/// Created: ahc 7 févr. 2013
/// =============================================================================
class SinkRoleExtender : public RoleExtender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SinkRoleExtender( RoleExtender_ABC* chain );
    //@}

    //! @name Operations
    //@{
    template< typename Role, typename Function >
    void AddFactory( const Function& func )
    {
        std::size_t idx = RoleExtender_ABC::Identificators::GetTypeId< typename Role::RoleInterface >();
        RoleExtender_ABC::T_RoleFactories::iterator it( roleFactories_.find( idx ) );
        if( it != roleFactories_.end() )
            throw MASA_EXCEPTION( std::string( "Extension " ) + typeid( typename Role::RoleInterface ).name() + " already attached to " + typeid( *this ).name() );
        boost::shared_ptr< RoleFactory_ABC > fact( new RoleFactory< typename Role >( func ) );
        roleFactories_.insert( std::make_pair( idx, fact) );
        indexes_.push_back( idx );
    }
    virtual void RegisterRoles( MIL_AgentPion& pion );
    //@}

private:
    //! @name Types
    //@{
    template< typename Role >
    struct RoleFactory : RoleExtender_ABC::RoleFactory_ABC
    {
        RoleFactory( std::function< Role*( MIL_AgentPion& ) > func ) : func_( func ) {}
        virtual void RegisterRole( MIL_AgentPion& pion )
        {
            pion.RegisterRole< Role >( *(func_( pion ) ) );
        }
    private:
        std::function< Role*( MIL_AgentPion& ) > func_;
    };
    //@}

private:
    //! @name Helpers
    //@{
    void RegisterRole( MIL_AgentPion& pion, std::size_t idx );
    //@}

private:
    //! @name Member data
    //@{
    RoleExtender_ABC* chain_;
    std::vector< std::size_t > indexes_;
    //@}
};

}

#endif // SWORD_SINKROLEEXTENDER_H
