// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanObjectWrapper.h"
#include "MIL_AgentServer.h"
#include "ResourceNetworkCapacity.h"
#include "MedicalCapacity.h"
#include "MedicalTreatmentAttribute.h"
#include "InfrastructureCapacity.h"
#include "StructuralCapacity.h"
#include "UrbanType.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <urban/PhysicalAttribute.h>
#include <urban/ColorAttribute.h>
#include <urban/GeometryAttribute.h>
#include <urban/Model.h>
#include <urban/InfrastructureAttribute.h>
#include <urban/ResourceNetworkAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/MotivationsVisitor_ABC.h>
#include <urban/StaticModel.h>
#include <urban/InfrastructureType.h>
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanObjectWrapper )

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object )
    : MIL_Object( 0, builder.GetType() )
    , object_( &object )
{
    InitializeAttributes();
    builder.Build( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper()
    : MIL_Object()
    , object_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper destructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::~UrbanObjectWrapper()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::InitializeAttributes
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::InitializeAttributes()
{
    // Position
    std::vector< MT_Vector2D > vector;
    const urban::GeometryAttribute* geometry = object_->Retrieve< urban::GeometryAttribute >();
    if( geometry )
    {
        geometry::Polygon2f::T_Vertices vertices = geometry->Geometry().Vertices();
        for( geometry::Polygon2f::CIT_Vertices it = vertices.begin(); it != vertices.end(); ++it )
            vector.push_back( MT_Vector2D( it->X(), it->Y() ) );
    }
    Initialize( TER_Localisation( TER_Localisation::ePolygon , vector ) );
    // resource network
    const urban::ResourceNetworkAttribute* resource = object_->Retrieve< urban::ResourceNetworkAttribute >();
    if( resource )
    {
        ResourceNetworkCapacity* capacity = new ResourceNetworkCapacity( *resource );
        capacity->Register( *this );
    }
    const urban::InfrastructureAttribute* infra = object_->Retrieve< urban::InfrastructureAttribute >();
    if( infra )
    {
        urban::InfrastructureType* infraType = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::InfrastructureType >( infra->GetType() );
        if( infraType )
        {
            InfrastructureCapacity* capacity = new InfrastructureCapacity( *infraType );
            capacity->Register( *this );
            if( infraType->medical_.hasMedicalCapacity )
            {
                MedicalCapacity* capacity = new MedicalCapacity( infraType->medical_.emergencyBedsRate_, infraType->medical_.emergencyDoctorsRate_, infraType->medical_.nightDoctorsRate_ );
                capacity->Register( *this );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::load
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned long urbanId;
    file >> boost::serialization::base_object< MIL_Object >( *this );
    file >> urbanId;
    object_ = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( urbanId );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::save
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned long urbanId = object_->GetId();
    file << boost::serialization::base_object< MIL_Object >( *this );
    file << urbanId;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::WriteODB
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::WriteODB( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::CanInteractWith
// Created: SLG 2010-01-05
// -----------------------------------------------------------------------------
bool UrbanObjectWrapper::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    return MIL_Object_ABC::CanInteractWith( agent );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_Army_ABC& /*team*/ )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_KnowledgeGroup& /*group*/ )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdate
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        if ( actionId == sword::ObjectMagicAction_Attribute_medical_treatment )
            GetAttribute< MedicalTreatmentAttribute >().OnUpdate( attribute );
    }
    return sword::ObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnRequest
// Created: BCI 2011-01-10
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& /*attributes*/ )
{
    return sword::ObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendCapacity
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
template < typename T >
void UrbanObjectWrapper::SendCapacity( sword::UrbanAttributes& msg ) const
{
    const T* capacity = Retrieve< T >();
    if( capacity )
        capacity->SendState( msg );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendFullStateCapacity
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
template < typename T >
void UrbanObjectWrapper::SendFullStateCapacity( sword::UrbanAttributes& msg ) const
{
    const T* capacity = Retrieve< T >();
    if( capacity )
        capacity->SendFullState( msg );
}

namespace
{
    class MotivationsVisitor : public urban::MotivationsVisitor_ABC
    {
    public:
        explicit MotivationsVisitor( std::map< std::string, float >& motivations )
            : motivations_( motivations )
        {
            // NOTHING
        }
        virtual void Visit( const std::string& motivation, float proportion )
        {
            motivations_[ motivation ] = proportion;
        }
        std::map< std::string, float >& motivations_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendCreation
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendCreation() const
{
    if( object_->HasChild() )
        return;
    client::UrbanCreation message;
    message().mutable_object()->set_id( GetID() );
    message().set_name( object_->GetName() );
    NET_ASN_Tools::WriteLocation( GetLocalisation(), *message().mutable_location() );

    const urban::ColorAttribute* color = object_->Retrieve< urban::ColorAttribute >();
    if( color != 0 )
    {
        message().mutable_attributes()->mutable_color()->set_red( color->Red() );
        message().mutable_attributes()->mutable_color()->set_green( color->Green() );
        message().mutable_attributes()->mutable_color()->set_blue( color->Blue() );
        message().mutable_attributes()->mutable_color()->set_alpha( color->Alpha() );
    }

    const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >();
    if( pPhysical )
    {
        if( pPhysical->GetArchitecture() )
        {
            message().mutable_attributes()->mutable_architecture()->set_height( pPhysical->GetArchitecture()->GetHeight() );
            message().mutable_attributes()->mutable_architecture()->set_floor_number( pPhysical->GetArchitecture()->GetFloorNumber() );
            message().mutable_attributes()->mutable_architecture()->set_roof_shape( pPhysical->GetArchitecture()->GetRoofShape().c_str() );
            message().mutable_attributes()->mutable_architecture()->set_material( pPhysical->GetArchitecture()->GetMaterial().c_str() );
            message().mutable_attributes()->mutable_architecture()->set_occupation( pPhysical->GetArchitecture()->GetOccupation() );
            message().mutable_attributes()->mutable_architecture()->set_trafficability( pPhysical->GetArchitecture()->GetTrafficability() );
            // TODO parking
            message().mutable_attributes()->mutable_architecture()->set_parking_available( false );
        }
        if( pPhysical->GetMotivations() )
        {
            T_Motivations motivations;
            MotivationsVisitor visitor( motivations );
            pPhysical->GetMotivations()->Accept( visitor );
            BOOST_FOREACH( const T_Motivations::value_type& motivation, motivations )
            {
                sword::UrbanUsage& usage = *message().mutable_attributes()->add_usages();
                usage.set_role( motivation.first );
                usage.set_percentage( static_cast< unsigned int >( motivation.second * 100 ) );
            }
        }
    }
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendDestruction
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendDestruction() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendFullState
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendFullState() const
{
    if( object_->HasChild() )
        return;
    client::UrbanUpdate message;
    message().mutable_object()->set_id( GetID() );
    SendFullStateCapacity< StructuralCapacity >( *message().mutable_attributes() );
    SendFullStateCapacity< ResourceNetworkCapacity >( *message().mutable_attributes() );
    SendFullStateCapacity< InfrastructureCapacity >( *message().mutable_attributes() );
    message.Send( NET_Publisher_ABC::Publisher() );
    MIL_Object::SendFullState();
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateState()
{
    if( object_->HasChild() )
        return;
    client::UrbanUpdate message;
    message().mutable_object()->set_id( GetID() );
    SendCapacity< StructuralCapacity >( *message().mutable_attributes() );
    SendCapacity< ResourceNetworkCapacity >( *message().mutable_attributes() );
    SendCapacity< InfrastructureCapacity >( *message().mutable_attributes() );
    if ( message().attributes().has_structure() || message().attributes().has_infrastructures() )
        message.Send( NET_Publisher_ABC::Publisher() );
    MIL_Object::UpdateState();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetID
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetUrbanId() const
{
    return object_->GetId();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetName
// Created: JCR 2010-10-22
// -----------------------------------------------------------------------------
const std::string& UrbanObjectWrapper::GetName() const
{
    return object_->GetName();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetObject
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC& UrbanObjectWrapper::GetObject()
{
    return *object_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetObject
// Created: SLG 2010-06-24
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC& UrbanObjectWrapper::GetObject() const
{
    return *object_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdateStructuralState
// Created: SLG 2010-12-22
// -----------------------------------------------------------------------------
sword::UrbanMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdateStructuralState( int state )
{
    StructuralCapacity* capacity = Retrieve< StructuralCapacity >();
    if( !capacity )
        return sword::UrbanMagicActionAck::error_invalid_urban_block;
    capacity->SetStructuralState( state );
    ApplyStructuralState( state );
    return sword::UrbanMagicActionAck::no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::UrbanObjectWrapper::OnUpdateStructuralState
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
sword::UrbanMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdateInfrastructure( const sword::UrbanMagicAction_Infrastructure& msg )
{
    InfrastructureCapacity* capacity = Retrieve< InfrastructureCapacity >();
    if( !capacity )
        return sword::UrbanMagicActionAck::error_invalid_urban_block;
    if( msg.has_active() )
        capacity->SetEnabled( msg.active() );
    if( msg.has_threshold() )
        capacity->SetThreshold( msg.threshold() );
    return sword::UrbanMagicActionAck::no_error;
}
