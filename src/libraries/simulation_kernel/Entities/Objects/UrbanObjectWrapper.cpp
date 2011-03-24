// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanObjectWrapper.h"
#include "MIL_AgentServer.h"
#include "Entities/Inhabitants/MIL_LivingArea.h"
#include "ResourceNetworkCapacity.h"
#include "MaterialAttribute.h"
#include "MedicalCapacity.h"
#include "MedicalTreatmentAttribute.h"
#include "InfrastructureCapacity.h"
#include "StructuralCapacity.h"
#include "PHY_InfrastructureType.h"
#include "PHY_MaterialCompositionType.h"
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
    builder.Build( *this );
    InitializeAttributes();
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
    if( const urban::ResourceNetworkAttribute* resource = object_->Retrieve< urban::ResourceNetworkAttribute >() )
    {
        ResourceNetworkCapacity* capacity = Retrieve< ResourceNetworkCapacity >();
        if( capacity )
            capacity->Initialize( *resource );
    }
    if( const urban::InfrastructureAttribute* infra = object_->Retrieve< urban::InfrastructureAttribute >() )
        if( const PHY_InfrastructureType* infraType = PHY_InfrastructureType::Find( infra->GetType() ) )
        {
            InfrastructureCapacity* capacity = new InfrastructureCapacity( *infraType );
            capacity->Register( *this );
            if( const PHY_InfrastructureType::MedicalProperties* medical = infraType->GetMedicalProperties() )
            {
                MedicalCapacity* capacity = new MedicalCapacity( medical->emergencyBedsRate_, medical->emergencyDoctorsRate_, medical->nightDoctorsRate_ );
                capacity->Register( *this );
            }
        }
    if( const urban::Architecture* architecture = GetArchitecture() )
        if( const PHY_MaterialCompositionType* material = PHY_MaterialCompositionType::Find( architecture->GetMaterial() ) )
            GetAttribute< MaterialAttribute >() = MaterialAttribute( *material );
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
// Name: UrbanObjectWrapper::WriteUrbanIdAttribute
// Created: JSR 2011-01-28
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::WriteUrbanIdAttribute( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", object_->GetId() );
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
    if( const urban::ColorAttribute* color = object_->Retrieve< urban::ColorAttribute >() )
    {
        message().mutable_attributes()->mutable_color()->set_red( color->Red() );
        message().mutable_attributes()->mutable_color()->set_green( color->Green() );
        message().mutable_attributes()->mutable_color()->set_blue( color->Blue() );
        message().mutable_attributes()->mutable_color()->set_alpha( color->Alpha() );
    }
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
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
// Name: UrbanObjectWrapper::Accept
// Created: JSR 2011-01-28
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Accept( urban::MotivationsVisitor_ABC& visitor ) const
{
    object_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetLivingSpace
// Created: JSR 2011-01-28
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetLivingSpace() const
{
    return object_->GetLivingSpace();
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
// Name: UrbanObjectWrapper::HasChild
// Created: LGY 2011-02-18
// -----------------------------------------------------------------------------
bool UrbanObjectWrapper::HasChild() const
{
    return object_->HasChild();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetFootprint
// Created: LGY 2011-02-18
// -----------------------------------------------------------------------------
const geometry::Polygon2f* UrbanObjectWrapper::GetFootprint() const
{
    return object_->GetFootprint();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ComputeComplexity
// Created: JSR 2011-02-18
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::ComputeComplexity() const
{
    return object_->ComputeComplexity();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetArchitecture
// Created: JSR 2011-02-18
// -----------------------------------------------------------------------------
const urban::Architecture* UrbanObjectWrapper::GetArchitecture() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        return pPhysical->GetArchitecture();
    return 0;
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
// Name: UrbanObjectWrapper::UpdateInhabitants
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateInhabitants( MIL_LivingArea& livingArea, unsigned int number )
{
    inhabitants_[ &livingArea ] = number;
    if( number == 0 )
        inhabitants_.erase( &livingArea );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetTotalInhabitants
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetTotalInhabitants() const
{
    unsigned int ret = 0;
    for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        ret += it->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdate
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes )
{
    for( int i = 0; i < attributes.size(); ++i )
    {
        const sword::MissionParameter_Value& attribute = attributes.Get( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return sword::ObjectMagicActionAck::error_invalid_specific_attributes;
        const unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        switch( actionId )
        {
        case sword::ObjectMagicAction_Attribute_alerted:
            OnReceiveSetAlerted( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_confined:
            OnReceiveSetConfined( attribute );
            break;
        case sword::ObjectMagicAction_Attribute_evacuated:
            OnReceiveSetEvacuated( attribute );
            break;
        default:
            break;
        }
    }
    return MIL_Object::OnUpdate( attributes );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnReceiveSetAlerted
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::OnReceiveSetAlerted( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool alerted = attribute.list( 1 ).booleanvalue();
        for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
            it->first->SetAlerted( alerted, this );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnReceiveSetConfined
// Created: BCI 2011-02-28
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::OnReceiveSetConfined( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool confined = attribute.list( 1 ).booleanvalue();
        for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
            it->first->SetConfined( confined, this );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnReceiveSetEvacuated
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::OnReceiveSetEvacuated( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        bool evacuated = attribute.list( 1 ).booleanvalue();
        for( IT_LivingAreas it = livingAreas_.begin(); it != livingAreas_.end(); ++it )
            ( *it )->SetEvacuated( evacuated, this );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::AddLivingArea
// Created: ABR 2011-03-24
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::AddLivingArea( MIL_LivingArea& livingArea )
{
    livingAreas_.push_back( &livingArea );
}
