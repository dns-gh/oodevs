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

#include "Entities/MIL_Army.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"

#include "game_asn/ASN_Delete.h"

#include "ConstructionAttribute.h"
#include "MineAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "CrossingSiteAttribute.h"
#include "SupplyRouteAttribute.h"

#include "Knowledge/DEC_Knowledge_Object.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

// HLA
#include "HLA/HLA_Object_ABC.h"
#include "HLA/HLA_UpdateFunctor.h"

#include <hla/Deserializer.h>
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_GUID( Object, "Object" )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object( uint id, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, const std::string& name, bool reserved, const std::vector< double >& parameters )
    : MIL_Object_ABC( army, builder.GetType() )
    , id_       ( id )
    , name_     ( name )
    , pView_    ( 0 )
    , manipulator_ ( new MIL_ObjectManipulator( *this ) )
{
    if( pLocation )
        Initialize( *pLocation );
    builder.Build( *this );
    ObstacleAttribute* pObstacle = RetrieveAttribute< ObstacleAttribute >();
    if( pObstacle )
        pObstacle->SetType( reserved? ASN1T_EnumDemolitionTargetType::reserved : ASN1T_EnumDemolitionTargetType::preliminary );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
Object::Object()
    : MIL_Object_ABC()
    , id_       ( 0 )
    , name_     ( )
    , pView_    ( 0 )
    , manipulator_ ( new MIL_ObjectManipulator( *this ) )
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
// Name: Object::GetID
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
uint Object::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Object::load
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> name_
         >> id_;
    
    T_Capacities capacities;
    file >> capacities;
    std::for_each( capacities.begin(), capacities.end(), boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( *this ) ) );

    file >> attributes_;
}
    
// -----------------------------------------------------------------------------
// Name: Object::save
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << name_
         << id_
         << capacities_
         << attributes_;
}

// -----------------------------------------------------------------------------
// Name: Object::WriteODB
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "object" )
            << xml::attribute( "id"  , id_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", GetType().GetName() );
    
    GetLocalisation().Write( xos );

    xos << xml::start( "attributes" );
    {
        std::for_each( attributes_.begin(), attributes_.end(), 
            boost::bind( &ObjectAttribute_ABC::WriteODB, _1, boost::ref( xos ) ) );
    }
    xos << xml::end();
    xos << xml::end(); // object
}

// -----------------------------------------------------------------------------
// Name: Object::AddAttribute
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::Register( ObjectAttribute_ABC* attribute )
{    
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: Object::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::Register( ObjectCapacity_ABC* capacity )
{
    capacities_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: Object::RegisterInteractive
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::Register( MIL_InteractiveContainer_ABC* capacity )
{
    interactives_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: Object::Instanciate
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
void Object::Instanciate( Object& object ) const
{    
    std::for_each( capacities_.begin(), capacities_.end(), 
                   boost::bind( &ObjectCapacity_ABC::Instanciate, _1, boost::ref( object ) ) );
//    std::for_each( object.capacities_.begin(), object.capacities_.end(), 
//                   boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( object ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Finalize
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
void Object::Finalize()
{    
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Finalize, _1, boost::ref( *this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Update
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void Object::Update( uint time )
{
    // TODO can be updated
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Update, _1, boost::ref( *this ), time ) );
    const ConstructionAttribute* attribute = RetrieveAttribute< ConstructionAttribute >();
    if( attribute && attribute->GetState() == 0.f )
        MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: Object::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
bool Object::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    bool canInteract = true;
    std::for_each( interactives_.begin(), interactives_.end(), boost::bind( &MIL_InteractiveContainer_ABC::CanInteractWith, _1, boost::ref( *this ), boost::ref( agent ), boost::ref( canInteract ) ) );
    return canInteract && MIL_Object_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentEntering
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentEntering, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentExiting
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentExiting, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentMovingInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentMovingInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: Object::ProcessAgentInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void Object::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyTerrainObjectCollision( *this );
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::ProcessPopulationInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void Object::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    std::for_each( interactives_.begin(), interactives_.end(), 
                   boost::bind( &MIL_InteractiveContainer_ABC::ProcessPopulationInside, _1, boost::ref( *this ), boost::ref( population ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::operator()
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator_ABC& Object::operator()()
{
    return *manipulator_;
}

// -----------------------------------------------------------------------------
// Name: Object::operator()
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
const MIL_ObjectManipulator_ABC& Object::operator()() const
{
    return *manipulator_;
}

// -----------------------------------------------------------------------------
// Name: Object::OnUpdate
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode Object::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    if( asn.m.minePresent ) GetAttribute< MineAttribute >().OnUpdate( asn );
    if( asn.m.bypassPresent ) GetAttribute< BypassAttribute >().OnUpdate( asn );
    if( asn.m.constructionPresent ) GetAttribute< ConstructionAttribute >().OnUpdate( asn );
    if( asn.m.obstaclePresent ) GetAttribute< ObstacleAttribute >().OnUpdate( asn );
    if( asn.m.crossing_sitePresent ) GetAttribute< CrossingSiteAttribute >().OnUpdate( asn );
    if( asn.m.supply_routePresent ) GetAttribute< SupplyRouteAttribute >().OnUpdate( asn );

    return EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendCreation() const
{
    if( pView_ && pView_->HideObject() )
        return;

    NET_ASN_MsgObjectCreation asn;

    asn().oid  = GetID();
    asn().name = name_.c_str();
    asn().type = GetType().GetName().c_str();
    asn().team = GetArmy().GetID();

    NET_ASN_Tools::WriteLocation( GetLocalisation(), asn().location );
    std::for_each( attributes_.begin(), attributes_.end(), 
                    boost::bind( &ObjectAttribute_ABC::SendFullState, _1, boost::ref( asn().attributes ) ) );
    asn.Send();
    ASN_Delete::Delete( asn().location );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendDestruction() const
{
    if( pView_ && pView_->HideObject() )
        return;

    NET_ASN_MsgObjectDestruction asn;
    asn() = GetID();
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void Object::UpdateState()
{
    SendMsgUpdate();
    MIL_Object_ABC::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullState
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void Object::SendFullState() const
{
//    NotifyAttributeUpdated( eAttrUpdate_All );
    xAttrToUpdate_ |= eAttrUpdate_Localisation;
    SendMsgUpdate();
}

// -----------------------------------------------------------------------------
// Name: Object::Object::SendMsgUpdate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::SendMsgUpdate() const
{
    if( pView_ && pView_->HideObject() )
        return;

    NET_ASN_MsgObjectUpdate asn;
    asn().oid = id_;

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::SendUpdate, _1, boost::ref( asn().attributes ) ) );

    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
    {
        asn().m.locationPresent = 1;
        NET_ASN_Tools::WriteLocation( GetLocalisation(), asn().location );
    }
        
    unsigned int xAttr = *reinterpret_cast< unsigned int * >( &asn().attributes.m );
    xAttr <<= 19; // $$$$ 32bits - 1bit * #attributes

    if ( xAttr != 0 || asn().m.locationPresent )
        asn.Send();
    
    xAttrToUpdate_ = 0;

    if( asn().m.locationPresent )
        ASN_Delete::Delete( asn().location );
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
    tools::Extendable< ObjectAttribute_ABC >& extensions = *this;

    if( attributeID == "coordonnees" )
    {
        TER_Localisation newLocalisation;
        deserializer >> newLocalisation;
        UpdateLocalisation( newLocalisation );
    }
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Deserialize, _1, boost::cref( attributeID ), deserializer ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void Object::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "armee", false, GetArmy().GetName() );
    functor.Serialize( "type",  false, GetType().GetName() );
    functor.Serialize( "coordonnees",   ( xAttrToUpdateForHLA_ & eAttrUpdate_Localisation ) != 0,           GetLocalisation() );
    
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Serialize, _1, boost::ref( functor ) ) );

    xAttrToUpdateForHLA_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Object::CreateKnowledge
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& Object::CreateKnowledge( const MIL_Army_ABC& team )
{
    DEC_Knowledge_Object& knowledge = *new DEC_Knowledge_Object( team, *this );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Instanciate, _1, boost::ref( knowledge ) ) );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateLocatisation
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void Object::UpdateLocalisation( const TER_Localisation& location )
{
    xAttrToUpdate_ |= eAttrUpdate_Localisation;    
    MIL_Object_ABC::UpdateLocalisation( location );
}
