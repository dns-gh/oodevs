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
#include "ObjectPrototype.h"
#include "ObjectCapacity_ABC.h"
#include "ObjectAttribute_ABC.h"
#include "MIL_ObjectBuilder_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include "MIL_ObjectManipulator.h"
#include "ConstructionAttribute.h"
#include "MineAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "CrossingSiteAttribute.h"
#include "SupplyRouteAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "protocol/ClientSenders.h"
#include "hla/HLA_Object_ABC.h"
#include "hla/HLA_UpdateFunctor.h"
#include <hla/Deserializer.h>
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( Object )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, bool reserved )
    : MIL_Object( &army, builder.GetType(), xis.attribute< unsigned long >( "id" ) )
    , name_       ( xis.attribute< std::string >( "name", "" ) )
    , pView_      ( 0 )
{
    MIL_Object_ABC::Register();
    if( pLocation )
        Initialize( *pLocation );
    builder.Build( *this );
    ObstacleAttribute* pObstacle = RetrieveAttribute< ObstacleAttribute >();
    if( pObstacle )
        pObstacle->SetType( reserved ? sword::ObstacleType_DemolitionTargetType_reserved : sword::ObstacleType_DemolitionTargetType_preliminary );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2009-12-14
// -----------------------------------------------------------------------------
Object::Object( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, const std::string& name /*= std::string()*/, bool reserved /*= true*/ )
    : MIL_Object( &army, builder.GetType() )
    , name_       ( name )
    , pView_      ( 0 )
{
    MIL_Object_ABC::Register();
    if( pLocation )
        Initialize( *pLocation );
    builder.Build( *this );
    ObstacleAttribute* pObstacle = RetrieveAttribute< ObstacleAttribute >();
    if( pObstacle )
        pObstacle->SetType( reserved? sword::ObstacleType_DemolitionTargetType_reserved : sword::ObstacleType_DemolitionTargetType_preliminary );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object()
    : MIL_Object()
    , pView_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
Object::~Object()
{
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
    const ConstructionAttribute* attribute = RetrieveAttribute< ConstructionAttribute >();
    if( attribute && attribute->NeedDestruction() )
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
// Name: Object::OnUpdate
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode Object::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        switch( actionId )
        {
        case sword::ObjectMagicAction_Attribute_mine:
            GetAttribute< MineAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_bypass:
            GetAttribute< BypassAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_construction:
            GetAttribute< ConstructionAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_obstacle:
            GetAttribute< ObstacleAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_crossing_site:
            GetAttribute< CrossingSiteAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_supply_route:
            GetAttribute< SupplyRouteAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_medical_treatment:
            GetAttribute< MedicalTreatmentAttribute >().OnUpdate( attribute );
            break;
        default:
            break;
        }
    }
    return sword::ObjectMagicActionAck::no_error;
}

// -----------------------------------------------------------------------------
// Name: Object::OnRequest
// Created: BCI 2011-01-10
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode Object::OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        switch( actionId )
        {
        case sword::ObjectMagicAction_Attribute_burn_surface:
            GetAttribute< BurnSurfaceAttribute >().OnRequest( attribute );
            break;
        default:
            break;
        }
    }
    return sword::ObjectMagicActionAck::no_error;
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendCreation() const
{
    if( pView_ && pView_->HideObject() )
        return;
    client::ObjectCreation asn;
    asn().mutable_object()->set_id( GetID() );
    asn().set_name( name_ );
    asn().mutable_type()->set_id( GetType().GetName() );
    asn().mutable_party()->set_id( GetArmy()->GetID() );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand viré dans le protocole ... le message de creation ne doit PAS envoyer les attributs
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendDestruction() const
{
    if( pView_ && pView_->HideObject() )
        return;
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
    if( pView_ && pView_->HideObject() )
        return;
    MIL_Object::SendFullState();
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void Object::UpdateState()
{
    if( pView_ && pView_->HideObject() )
        return;
    MIL_Object::UpdateState();
    MIL_Object_ABC::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: Object::GetHLAView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_Object_ABC* Object::GetHLAView() const
{
    return pView_;
}

// -----------------------------------------------------------------------------
// Name: Object::SetView
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::SetHLAView( HLA_Object_ABC& view )
{
    delete pView_;
    pView_ = &view;
}

// -----------------------------------------------------------------------------
// Name: Object::Deserialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "coordonnees" )
    {
        TER_Localisation newLocalisation;
        deserializer >> newLocalisation;
        UpdateLocalisation( newLocalisation );
    }
    MIL_Object::Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: Object::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy()->GetName() );
    functor.Serialize( "type", false, GetType().GetName() );
    functor.Serialize( "nom", false, name_ );
    functor.Serialize( "coordonnees", ( xAttrToUpdateForHLA_ & eAttrUpdate_Localisation ) != 0, GetLocalisation() );
    MIL_Object::Serialize( functor );
    xAttrToUpdateForHLA_ = 0;
}
