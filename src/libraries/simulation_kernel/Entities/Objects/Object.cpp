// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "CrossingSiteAttribute.h"
#include "SupplyRouteAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "SpawnCapacity.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
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
    : MIL_Object_ABC( &army, builder.GetType() )
    , id_         ( xis.attribute< unsigned long >( "id" ) )
    , name_       ( xis.attribute< std::string >( "name", "" ) )
    , pView_      ( 0 )
    , manipulator_( new MIL_ObjectManipulator( *this ) )
{
    idManager_.Lock( id_ );
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
    : MIL_Object_ABC( &army, builder.GetType() )
    , id_         ( idManager_.GetFreeId() )
    , name_       ( name )
    , pView_      ( 0 )
    , manipulator_( new MIL_ObjectManipulator( *this ) )
{
    if( GetType().GetCapacity< SpawnCapacity >() )
        idManager_.GetFreeId(); // we need to skip one ID for dynamic created object.

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
    : MIL_Object_ABC()
    , id_         ( 0 )
    , pView_      ( 0 )
    , manipulator_( new MIL_ObjectManipulator( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
Object::~Object()
{
    MIL_Object_ABC::Unregister();
}

// -----------------------------------------------------------------------------
// Name: Object::GetID
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
unsigned int Object::GetID() const
{
    return id_;
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
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> name_
         >> id_;
    T_Capacities capacities;
    file >> capacities;
    std::for_each( capacities.begin(), capacities.end(), boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( *this ) ) );
    T_Attributes attributes;
    file >> attributes;
    std::for_each( attributes.begin(), attributes.end(), boost::bind( &ObjectAttribute_ABC::Register, _1, boost::ref( *this ) ) );
    idManager_.Lock( id_ );
    //MIL_Object_ABC::Register();
}

// -----------------------------------------------------------------------------
// Name: Object::save
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << name_;
    file << id_;
    file << capacities_;
    file << attributes_;
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
    xos     << xml::start( "attributes" );
    std::for_each( attributes_.begin(), attributes_.end(), boost::bind( &ObjectAttribute_ABC::WriteODB, _1, boost::ref( xos ) ) );
    xos     << xml::end;
    xos << xml::end; // object
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
void Object::Instanciate( MIL_Object_ABC& object ) const
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
void Object::Update( unsigned int time )
{
    // TODO can be updated
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Update, _1, boost::ref( *this ), time ) );
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
    bool canInteract = true;
    std::for_each( interactives_.begin(), interactives_.end(), boost::bind( &MIL_InteractiveContainer_ABC::CanInteractWith, _1, boost::ref( *this ), boost::ref( agent ), boost::ref( canInteract ) ) );
    return canInteract && MIL_Object_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: Object::PreprocessAgent
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void Object::PreprocessAgent( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
                   boost::bind( &MIL_InteractiveContainer_ABC::PreprocessAgent, _1, boost::ref( *this ), boost::ref( agent ) ) );
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
// Name: Object::PreprocessPopulation
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void Object::PreprocessPopulation( MIL_PopulationElement_ABC& population )
{
    std::for_each( interactives_.begin(), interactives_.end(),
                   boost::bind( &MIL_InteractiveContainer_ABC::PreprocessPopulation, _1, boost::ref( *this ), boost::ref( population ) ) );
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
sword::ObjectMagicActionAck_ErrorCode Object::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes;
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
    return sword::ObjectMagicActionAck_ErrorCode_no_error;
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
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand vir� dans le protocole ... le message de creation ne doit PAS envoyer les attributs
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
    if( pView_ && pView_->HideObject() )
        return;
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( id_ );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::SendFullState, _1, boost::ref( *asn().mutable_attributes() ) ) );        
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: Object::Object::SendMsgUpdate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void Object::SendMsgUpdate() const
{
    if( pView_ && pView_->HideObject() )
        return;
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( id_ );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::SendUpdate, _1, boost::ref( *asn().mutable_attributes() ) ) );
    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
        NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    sword::ObjectAttributes& attr = *asn().mutable_attributes();
    if( asn().has_location() || attr.has_construction() || attr.has_obstacle()
        || attr.has_mine() || attr.has_activity_time() || attr.has_bypass()
        || attr.has_logistic() || attr.has_nbc() || attr.has_crossing_site()
        || attr.has_supply_route() || attr.has_toxic_cloud() || attr.has_fire() || attr.has_burn()
        || attr.has_medical_treatment() || attr.has_interaction_height() || attr.has_stock()
        || attr.has_nbc_agent() || attr.has_effect_delay() || attr.has_resource_networks() )
        asn.Send( NET_Publisher_ABC::Publisher() );
    xAttrToUpdate_ = 0;
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
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Deserialize, _1, boost::cref( attributeID ), deserializer ) );
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
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Serialize, _1, boost::ref( functor ) ) );
    xAttrToUpdateForHLA_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Object::CreateKnowledge
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > Object::CreateKnowledge( const MIL_Army_ABC& team )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( team, *this ) );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Instanciate, _1, boost::ref( *pKnowledge ) ) );
    return pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: Object::CreateKnowledge
// Created: LDC 2010-04-15
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > Object::CreateKnowledge( const MIL_KnowledgeGroup& group )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( group, *this ) );
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Instanciate, _1, boost::ref( *pKnowledge ) ) );
    return pKnowledge;
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
