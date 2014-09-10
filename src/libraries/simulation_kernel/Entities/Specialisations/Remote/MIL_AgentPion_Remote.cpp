// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPion_Remote.h"

#include "Adapters/RoleAdapterInterface.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Network/NET_RoleInterface_Dotations.h"

#include "MissionController_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPion_Remote )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPion_Remote* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller= &pion->GetController();
    archive << nTypeID;
    archive << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPion_Remote* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    std::string name;
    archive >> nTypeID;
    archive >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPion_Remote( *pType, *controller );
}

MIL_AgentPion_Remote::MIL_AgentPion_Remote( const MIL_AgentTypePion& type,
                                            MissionController_ABC& controller,
                                            MIL_Automate& automate,
                                            xml::xistream& xis )
    : MIL_AgentPion( type, controller, automate, xis )
{
    // NOTHING
}

MIL_AgentPion_Remote::MIL_AgentPion_Remote( const MIL_AgentTypePion& type,
                                            MissionController_ABC& controller )
    : MIL_AgentPion( type, controller )
{
    // NOTHING
}

MIL_AgentPion_Remote::~MIL_AgentPion_Remote()
{
    // NOTHING
}

void MIL_AgentPion_Remote::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "unit" );
    MIL_Entity_ABC::WriteODB( xos ) ;
    xos << xml::attribute( "id", GetID() )
        << xml::attribute( "type", GetType().GetName() )
        << xml::attribute( "command-post", IsPC() )
        << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
    if( !GetLevel().empty() )
        xos << xml::attribute( "level", GetLevel() );
    if( !GetSymbol().empty() )
        xos << xml::attribute( "overridden-symbol", true )
            << xml::attribute( "nature", GetSymbol() );
    GetColor()->WriteODB( xos );
    CallRole( &PHY_RolePion_Composantes::WriteODB, xos );
    GetExtensions()->WriteODB( xos );
    xos << xml::end;// unit
}

namespace
{
    template< typename R >
    R& LoadRole( MIL_CheckPointInArchive& archive, tools::RoleContainer& container )
    {
        R* role;
        archive >> role;
        if( !role )
            throw MASA_EXCEPTION( "Failed to load role " + std::string( typeid( role ).name() ) );
        container.RegisterRole( *role );
        return *role;
    }

    template< typename R >
    void SaveRole( const MIL_AgentPion& pion, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & pion.GetRole< R >();
        file << role;
    }
}

void MIL_AgentPion_Remote::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_Automate* pAutomate = 0;
    DEC_KnowledgeBlackBoard_AgentPion* blackboard = 0;
    MIL_AffinitiesMap* pAffinities;
    MIL_DictionaryExtensions* pExtensions = 0;
    MIL_Color* pColor;
    std::string level;
    std::string symbol;
    file >> boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file >> pAutomate;
    file >> blackboard;
    file >> pAffinities;
    file >> pExtensions;
    file >> pColor;
    file >> symbol;
    file >> level;
    SetAutomate( pAutomate );
    SetKnowledge( blackboard );
    SetAffinities( std::unique_ptr< MIL_AffinitiesMap >( pAffinities ) );
    SetLevel( level );
    SetSymbol( symbol );
    LoadRole< NET_RoleInterface_Dotations >( file, *this );
    LoadRole< PHY_RoleInterface_Location >( file, *this );
    LoadRole< PHY_RoleInterface_UrbanLocation >( file, *this );
    LoadRole< PHY_RoleInterface_Composantes >( file, *this );
    SetColor( std::unique_ptr< MIL_Color >( pColor ) );
    SetExtensions( *pExtensions );
    LoadRole< sword::RoleAdapterInterface >( file, *this );
}

void MIL_AgentPion_Remote::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_Automate* const pAutomate = &GetAutomate();
    const MIL_AffinitiesMap* const pAffinities = GetAffinities();
    const MIL_DictionaryExtensions* const pExtensions = GetExtensions();
    const DEC_KnowledgeBlackBoard_AgentPion* const pKnowledge = &GetKnowledge();
    const MIL_Color* const pColor = GetColor();
    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << pAutomate;
    file << pKnowledge;
    file << pAffinities;
    file << pExtensions;
    file << pColor;
    file << GetSymbol();
    file << GetLevel();
    SaveRole< NET_RoleInterface_Dotations >( *this, file );
    SaveRole< PHY_RoleInterface_Location >( *this, file );
    SaveRole< PHY_RoleInterface_UrbanLocation >( *this, file );
    SaveRole< PHY_RoleInterface_Composantes >( *this, file );
    SaveRole< sword::RoleAdapterInterface >( *this, file );
}
