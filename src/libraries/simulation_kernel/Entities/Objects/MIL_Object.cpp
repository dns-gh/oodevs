// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Object.h"
#include "ObjectCapacity_ABC.h"
#include "ObjectAttribute_ABC.h"
#include "ConstructionAttribute.h"
#include "MineAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "CrossingSiteAttribute.h"
#include "FloodAttribute.h"
#include "InfrastructureCapacity.h"
#include "MedicalTreatmentAttribute.h"
#include "SupplyRouteAttribute.h"
#include "StructuralCapacity.h"
#include "UndergroundAttribute.h"
#include "UniversalCapacity.h"
#include "MIL_ObjectManipulator.h"
#include "MIL_StructuralStateNotifier_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Object )

// -----------------------------------------------------------------------------
// Name: MIL_Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object::MIL_Object( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type )
    : MIL_Object_ABC( army, type )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object constructor
// Created: JSR 2011-01-19
// -----------------------------------------------------------------------------
MIL_Object::MIL_Object( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int id )
    : MIL_Object_ABC( army, type, id )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object::MIL_Object()
    : manipulator_( *new MIL_ObjectManipulator( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_Object::~MIL_Object()
{
    MIL_Object_ABC::Unregister();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::load
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MIL_Object::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    T_Capacities capacities;
    file >> capacities;
    for( T_Capacities::const_iterator it = capacities.begin(); it != capacities.end(); ++it )
        (*it)->Register( *this );
    T_Attributes attributes;
    file >> attributes;
    for( T_Attributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it )
        (*it)->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::save
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MIL_Object::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << capacities_;
    file << attributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::WriteODB
// Created: SLG 2011-01-10
// -----------------------------------------------------------------------------
void MIL_Object::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "attributes" );
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->WriteODB( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::RegisterInteractive
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MIL_Object::Register( MIL_InteractiveContainer_ABC* capacity )
{
    interactives_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::Register
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Object::Register( MIL_StructuralStateNotifier_ABC& notifier )
{
    structuralStateNotifiers_.push_back( &notifier );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::AddAttribute
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void MIL_Object::Register( ObjectAttribute_ABC* attribute )
{
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: Object::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MIL_Object::Register( ObjectCapacity_ABC* capacity )
{
    capacities_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::Instanciate
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_Object::Instanciate( MIL_Object_ABC& object ) const
{
    for( T_Capacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        (*it)->Instanciate( object );
}

// -----------------------------------------------------------------------------
// Name: Object::Finalize
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
void MIL_Object::Finalize()
{
    for( T_Capacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        (*it)->Finalize( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::CanInteractWith
// Created: LDC 2009-03-03
// -----------------------------------------------------------------------------
bool MIL_Object::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    bool canInteract = true;
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->CanInteractWith( *this, agent, canInteract );
    return canInteract && MIL_Object_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::PreprocessAgent
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void MIL_Object::PreprocessAgent( MIL_Agent_ABC& agent )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->PreprocessAgent( *this, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ProcessAgentEntering
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_Object::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->ProcessAgentEntering( *this, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ProcessAgentExiting
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_Object::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->ProcessAgentExiting( *this, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ProcessAgentMovingInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_Object::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->ProcessAgentMovingInside( *this, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ProcessAgentInside
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MIL_Object::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->ProcessAgentInside( *this, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::PreprocessPopulation
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void MIL_Object::PreprocessPopulation( MIL_PopulationElement_ABC& population )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->PreprocessPopulation( *this, population );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ProcessPopulationInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_Object::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    for( T_InteractiveCapacities::const_iterator it = interactives_.begin(); it != interactives_.end(); ++it )
        (*it)->ProcessPopulationInside( *this, population );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ApplyStructuralState
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Object::ApplyStructuralState( float structuralState ) const
{
    for( T_StructuralStateNotifiers::const_iterator it = structuralStateNotifiers_.begin(); it != structuralStateNotifiers_.end(); ++it )
        (*it)->NotifyStructuralStateChanged( structuralState, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ApplyIndirectFire
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
void MIL_Object::ApplyIndirectFire( const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation, MIL_Army_ABC& army )
{
    if( StructuralCapacity* capacity = tools::Extendable< ObjectCapacity_ABC >::Retrieve< StructuralCapacity >() )
        capacity->ApplyIndirectFire( *this, attritionSurface, dotation, &army );
    if( GetLocalisation().IsInside( attritionSurface.ComputeBarycenter() ) )
        ApplyDirectFire();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::MIL_Object::ApplyDestruction
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
void MIL_Object::ApplyDestruction( const TER_Localisation& attritionSurface, const PHY_UrbanAttritionData& attrition )
{
    if( StructuralCapacity* capacity = tools::Extendable< ObjectCapacity_ABC >::Retrieve< StructuralCapacity >() )
        capacity->ApplyDestruction( *this, attritionSurface, attrition );
    if( GetLocalisation().IsInside( attritionSurface.ComputeBarycenter() ) )
        ApplyDirectFire();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::ApplyDirectFire
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
void MIL_Object::ApplyDirectFire() const
{
    for( T_StructuralStateNotifiers::const_iterator it = structuralStateNotifiers_.begin(); it != structuralStateNotifiers_.end(); ++it )
        (*it)->NotifyFired();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::IsUniversal
// Created: JSR 2011-01-07
// -----------------------------------------------------------------------------
bool MIL_Object::IsUniversal() const
{
    return tools::Extendable< ObjectCapacity_ABC >::Retrieve< UniversalCapacity >() != 0;
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullState
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
void MIL_Object::SendFullState() const
{
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( GetID() );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    ::sword::ObjectAttributes* attributes = asn().mutable_attributes();
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->SendFullState( *attributes );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::OnUpdate
// Created: JSR 2011-02-11
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode MIL_Object::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        switch( actionId )
        {
        case sword::ObjectMagicAction::mine:
            GetAttribute< MineAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::bypass:
            GetAttribute< BypassAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::construction:
            GetAttribute< ConstructionAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::obstacle:
            GetAttribute< ObstacleAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::crossing_site:
            GetAttribute< CrossingSiteAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::supply_route:
            GetAttribute< SupplyRouteAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::medical_treatment:
            GetAttribute< MedicalTreatmentAttribute >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::structural_state:
            Get< StructuralCapacity >().OnUpdate( attribute );
            ApplyStructuralState( Get< StructuralCapacity >().GetStructuralState() );
            break;
        case sword::ObjectMagicAction::infrastructure:
            Get< InfrastructureCapacity >().OnUpdate( attribute );
            break;
        case sword::ObjectMagicAction::flood:
            GetAttribute< FloodAttribute >().GenerateFlood( true );
            break;
        case sword::ObjectMagicAction::underground:
            GetAttribute< UndergroundAttribute >().OnUpdate( attribute );
            break;
        default:
            break;
        }
    }
    return sword::ObjectMagicActionAck::no_error;
}

// -----------------------------------------------------------------------------
// Name: Object::Object::SendMsgUpdate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MIL_Object::UpdateState()
{
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( GetID() );
    sword::ObjectAttributes& attr = *asn().mutable_attributes();
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->SendUpdate( attr );
    if( xAttrToUpdate_ & eAttrUpdate_Localisation )
        NET_ASN_Tools::WriteLocation( GetLocalisation(), *asn().mutable_location() );
    if( asn().has_location() || attr.has_construction() || attr.has_obstacle() || attr.has_mine()
        || attr.has_activity_time() || attr.has_bypass() || attr.has_logistic() || attr.has_nbc()
        || attr.has_crossing_site() || attr.has_supply_route() || attr.has_toxic_cloud() || attr.has_fire()
        || attr.has_burn() || attr.has_medical_treatment() || attr.has_interaction_height() || attr.has_stock()
        || attr.has_nbc_agent() || attr.has_effect_delay() || attr.has_resource_networks() || attr.has_flood()
        || attr.has_lodging() || attr.has_altitude_modifier() || attr.has_underground() )
        asn.Send( NET_Publisher_ABC::Publisher() );
    xAttrToUpdate_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::Update
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void MIL_Object::Update( unsigned int time )
{
    for( T_Capacities::const_iterator it = capacities_.begin(); it != capacities_.end(); ++it )
        (*it)->Update( *this, time );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
const MIL_ObjectManipulator_ABC& MIL_Object::operator()() const
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
MIL_ObjectManipulator_ABC& MIL_Object::operator()()
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateLocatisation
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void MIL_Object::UpdateLocalisation( const TER_Localisation& location )
{
    xAttrToUpdate_ |= eAttrUpdate_Localisation;
    MIL_Object_ABC::UpdateLocalisation( location );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::CreateKnowledge
// Created: SLG 2011-01-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_Object::CreateKnowledge( const MIL_Army_ABC& team )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( team, *this ) );
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->Instanciate( *pKnowledge );
    return pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object::CreateKnowledge
// Created: SLG 2011-01-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > MIL_Object::CreateKnowledge( const MIL_KnowledgeGroup& group )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge( new DEC_Knowledge_Object( group, *this ) );
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        (*it)->Instanciate( *pKnowledge );
    return pKnowledge;
}
