// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Object.h"
#include "BurnSurfaceAttribute.h"
#include "ConstructionAttribute.h"
#include "ObstacleAttribute.h"
#include "MIL_ObjectBuilder_ABC.h"
#include "MIL_ObjectType_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( Object )

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army, const TER_Localisation* pLocation, bool reserved )
    : MIL_Object( army, builder.GetType(), xis.attribute< unsigned long >( "id" ) )
    , name_( xis.attribute< std::string >( "name", "" ) )
{
    MIL_Object_ABC::Register();
    if( pLocation )
        Initialize( *pLocation );
    builder.Build( *this );
    if( ObstacleAttribute* pObstacle = RetrieveAttribute< ObstacleAttribute >() )
        pObstacle->SetType( reserved ? sword::ObstacleType_DemolitionTargetType_reserved : sword::ObstacleType_DemolitionTargetType_preliminary );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2009-12-14
// -----------------------------------------------------------------------------
Object::Object( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army, const TER_Localisation* pLocation,
                unsigned int externalIdentifier, const std::string& name /*= std::string()*/, bool reserved /*= true*/, unsigned int forcedId /*= 0u*/ )
    : MIL_Object( army, builder.GetType(), forcedId )
    , name_              ( name )
    , externalIdentifier_( externalIdentifier )
{
    MIL_Object_ABC::Register();
    if( pLocation )
        Initialize( *pLocation );
    builder.Build( *this );
    if( ObstacleAttribute* pObstacle = RetrieveAttribute< ObstacleAttribute >() )
        pObstacle->SetType( reserved? sword::ObstacleType_DemolitionTargetType_reserved : sword::ObstacleType_DemolitionTargetType_preliminary );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object()
    : MIL_Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
Object::~Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::GetName
// Created: JCR 2010-07-07
// -----------------------------------------------------------------------------
const std::string& Object::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Object::load
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Object >( *this );
    file >> name_;
}

// -----------------------------------------------------------------------------
// Name: Object::save
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Object >( *this );
    file << name_;
}

// -----------------------------------------------------------------------------
// Name: Object::WriteODB
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "object" )
            << xml::attribute( "id"  , GetID() )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", GetType().GetName() );
    GetLocalisation().Write( xos );
    MIL_Object::WriteODB( xos );
    xos << xml::end; // object
}

// -----------------------------------------------------------------------------
// Name: Object::Update
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void Object::Update( unsigned int time )
{
    // TODO can be updated
    MIL_Object::Update( time );
    if( const ConstructionAttribute* attribute = RetrieveAttribute< ConstructionAttribute >() )
        if( attribute->NeedDestruction() )
            MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: Object::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
bool Object::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    return MIL_Object::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentMovingInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    MIL_Object::ProcessAgentMovingInside( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    MIL_Object::ProcessAgentInside( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendCreation() const
{
    client::ObjectCreation asn;
    asn().mutable_object()->set_id( GetID() );
    asn().set_name( name_ );
    asn().mutable_type()->set_id( GetType().GetName() );
    asn().mutable_party()->set_id( GetArmy() ? GetArmy()->GetID() : 0 );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand viré dans le protocole ... le message de creation ne doit PAS envoyer les attributs
    if( externalIdentifier_ != 0u )
        asn().set_external_identifier( externalIdentifier_ );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendDestruction() const
{
    client::ObjectDestruction asn;
    asn().mutable_object()->set_id( GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullState
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendFullState() const
{
    MIL_Object::SendFullState();
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void Object::UpdateState()
{
    MIL_Object::UpdateState();
    MIL_Object_ABC::UpdateState();
}
