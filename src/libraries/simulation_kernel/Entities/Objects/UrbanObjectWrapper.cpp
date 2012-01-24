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
#include "ResourceNetworkAttribute.h"
#include "ResourceNetworkCapacity.h"
#include "MaterialAttribute.h"
#include "MedicalCapacity.h"
#include "InfrastructureCapacity.h"
#include "StructuralCapacity.h"
#include "PHY_InfrastructureType.h"
#include "PHY_MaterialCompositionType.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Entities/Inhabitants/MIL_LivingArea.h"
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

const float UrbanObjectWrapper::stretchOffset_ = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object )
    : MIL_Object( 0, builder.GetType(), 0 )
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
// Name: UrbanObjectWrapper::UpdateLocalisation
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateLocalisation( const TER_Localisation& /*location*/ )
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
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
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
    file >> inhabitants_
         >> livingAreas_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::save
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned long urbanId = object_->GetId();
    file << boost::serialization::base_object< MIL_Object >( *this );
    file << urbanId
         << inhabitants_
         << livingAreas_;
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
        sword::RgbaColor* msg = message().mutable_attributes()->mutable_color();
        msg->set_red( color->Red() );
        msg->set_green( color->Green() );
        msg->set_blue( color->Blue() );
        msg->set_alpha( color->Alpha() );
    }
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
    {
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
        {
            sword::UrbanAttributes_Architecture* msg = message().mutable_attributes()->mutable_architecture();
            msg->set_height( static_cast< float >( architecture->GetHeight() ) );
            msg->set_floor_number( architecture->GetFloorNumber() );
            msg->set_roof_shape( architecture->GetRoofShape().c_str() );
            msg->set_material( architecture->GetMaterial().c_str() );
            msg->set_occupation( architecture->GetOccupation() );
            msg->set_trafficability( architecture->GetTrafficability() );
            msg->set_parking_floors( architecture->GetParkingFloors() );
        }
        if( pPhysical->GetMotivations() )
        {
            std::map< std::string, float > motivations;
            MotivationsVisitor visitor( motivations );
            pPhysical->GetMotivations()->Accept( visitor );
            for( std::map< std::string, float >::const_iterator it = motivations.begin(); it != motivations.end(); ++it )
            {
                sword::UrbanUsage& usage = *message().mutable_attributes()->add_usages();
                usage.set_role( it->first );
                usage.set_percentage( static_cast< unsigned int >( it->second * 100 + 0.5f ) );
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
    size_t notSendableAttributes = RetrieveAttribute< MaterialAttribute >() ? 1 : 0;
    if( CountAttributes() > notSendableAttributes )
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
    if( message().attributes().has_structure() || message().attributes().has_infrastructures() )
        message.Send( NET_Publisher_ABC::Publisher() );
    size_t notSendableAttributes = RetrieveAttribute< MaterialAttribute >() ? 1 : 0;
    if( CountAttributes() > notSendableAttributes )
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
// Name: UrbanObjectWrapper::ComputeLocalisationsInsideBlock
// Created: MIA 2011-06-13
// -----------------------------------------------------------------------------
const std::vector< boost::shared_ptr< MT_Vector2D > >& UrbanObjectWrapper::ComputeLocalisationsInsideBlock()
{
    if( strechedArea_.size() == 0 )
    {
        const UrbanObjectWrapper& constThis = *this;
        const T_PointVector& points = constThis.GetLocalisation().GetPoints();
        const MT_Vector2D barycenter = constThis.GetLocalisation().ComputeBarycenter();
        for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
            strechedArea_.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *it + MT_Vector2D( *it - barycenter ).Normalize() * stretchOffset_ ) ) );
    }
    return strechedArea_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Is
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
bool UrbanObjectWrapper::Is( const urban::TerrainObject_ABC& object ) const
{
    return object_ == &object;
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
// Name: UrbanObjectWrapper::ComputeComplexity
// Created: JSR 2011-02-18
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::ComputeComplexity() const
{
    return object_->ComputeComplexity();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetHeight
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetHeight() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
            return static_cast< float >( architecture->GetHeight() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetStructuralHeight
// Created: MMC 2012-01-09
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetStructuralHeight() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
        {
            if( const StructuralCapacity* structuralCapacity = Retrieve< StructuralCapacity >() )
                return float( architecture->GetHeight() ) * structuralCapacity->GetStructuralState();
            else
                return float( architecture->GetHeight() );
        }
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetStructuralState
// Created: MMC 2012-01-23
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetStructuralState() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
            if ( const StructuralCapacity* structuralCapacity = Retrieve< StructuralCapacity >() )
                return structuralCapacity->GetStructuralState();
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetOccupation
// Created: JSR 2011-05-26
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetOccupation() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
            return architecture->GetOccupation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetTrafficability
// Created: JSR 2011-05-26
// -----------------------------------------------------------------------------
float UrbanObjectWrapper::GetTrafficability() const
{
    if( const urban::PhysicalAttribute* pPhysical = object_->Retrieve< urban::PhysicalAttribute >() )
        if( const urban::Architecture* architecture = pPhysical->GetArchitecture() )
            return architecture->GetTrafficability();
    return std::numeric_limits< float >::max();
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
void UrbanObjectWrapper::UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number )
{
    inhabitants_[ &livingArea ][ motivation ] = number;
    if( number == 0 )
    {
        inhabitants_[ &livingArea ].erase( motivation );
        unsigned int total = 0;
        for( CIT_Motivations it = inhabitants_[ &livingArea ].begin(); it != inhabitants_[ &livingArea ].end(); ++it )
            total += it->second;
        if( total == 0 )
            inhabitants_.erase( &livingArea );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetTotalInhabitants
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetTotalInhabitants() const
{
    unsigned int ret = 0;
    for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( CIT_Motivations it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            ret += it2->second;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetTotalInhabitantsForMotivation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetTotalInhabitantsForMotivation( const std::string& motivation ) const
{
    unsigned int ret = 0;
    for( CIT_Inhabitants it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        for( CIT_Motivations it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if( it2->first == motivation )
                ret += it2->second;
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
        case sword::ObjectMagicAction::alerted:
            OnReceiveSetAlerted( attribute );
            break;
        case sword::ObjectMagicAction::confined:
            OnReceiveSetConfined( attribute );
            break;
        case sword::ObjectMagicAction::evacuated:
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
