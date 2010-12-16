// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectManipulator.h"
#include "MIL_AgentServer.h"
#include "ResourceNetworkCapacity.h"
#include "StructuralCapacity.h"
#include "UrbanObjectWrapper.h"
#include "UrbanType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_ObjectLoader.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_Localisation.h"
#include "hla/HLA_Object_ABC.h"
#include "hla/HLA_UpdateFunctor.h"
#include <urban/ColorRGBA.h>
#include <urban/Architecture.h>
#include <urban/ColorAttribute.h>
#include <urban/GeometryAttribute.h>
#include <urban/Model.h>
#include <urban/ResourceNetworkAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/StaticModel.h>
#include <urban/MaterialCompositionType.h>
#include <geometry/Types.h>
#include <boost/serialization/vector.hpp>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanObjectWrapper )

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object )
    : MIL_Object_ABC( 0, builder.GetType() )
    , object_( &object )
    , id_( idManager_.GetFreeId() )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
    , pView_( 0 )
{
    InitializeAttributes();
    builder.Build( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper()
    : MIL_Object_ABC()
    , object_( 0 )
    , id_( 0 )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
    , pView_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper destructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::~UrbanObjectWrapper()
{
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
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::load
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned long urbanId;
    file >> boost::serialization::base_object< MIL_Object_ABC >( *this );
    file >> id_
         >> urbanId;
    object_ = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( urbanId );
    T_Capacities capacities;
    file >> capacities;
    std::for_each( capacities.begin(), capacities.end(), boost::bind( &ObjectCapacity_ABC::Register, _1, boost::ref( *this ) ) );
    idManager_.Lock( id_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::save
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned long urbanId = object_->GetId();
    file << boost::serialization::base_object< MIL_Object_ABC >( *this );
    file << id_;
    file << urbanId;
    file << capacities_;
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
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( MIL_InteractiveContainer_ABC* capacity )
{
    interactives_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::PreprocessAgent
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::PreprocessAgent( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::PreprocessAgent, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentEntering
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentEntering, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentExiting
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentExiting, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentMovingInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentMovingInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessAgentInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::ProcessAgentInside, _1, boost::ref( *this ), boost::ref( agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::PreprocessPopulation
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::PreprocessPopulation( MIL_PopulationElement_ABC& population )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::PreprocessPopulation, _1, boost::ref( *this ), boost::ref( population ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::ProcessPopulationInside
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::ProcessPopulationInside( MIL_PopulationElement_ABC& population )
{
    std::for_each( interactives_.begin(), interactives_.end(),
        boost::bind( &MIL_InteractiveContainer_ABC::ProcessPopulationInside, _1, boost::ref( *this ), boost::ref( population ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Instanciate
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Instanciate( MIL_Object_ABC& object ) const
{
    std::for_each( capacities_.begin(), capacities_.end(),
        boost::bind( &ObjectCapacity_ABC::Instanciate, _1, boost::ref( object ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Finalize
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Finalize()
{
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Finalize, _1, boost::ref( *this ) ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_Army_ABC& /*team*/ )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_KnowledgeGroup& /*group*/ )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
const MIL_ObjectManipulator_ABC& UrbanObjectWrapper::operator()() const
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::operator()
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
MIL_ObjectManipulator_ABC& UrbanObjectWrapper::operator()()
{
    return manipulator_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetHLAView
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
HLA_Object_ABC* UrbanObjectWrapper::GetHLAView() const
{
    return pView_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SetHLAView
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SetHLAView( HLA_Object_ABC& view )
{
    delete pView_;
    pView_ = &view;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Deserialize
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Deserialize( const hla::AttributeIdentifier& /*attributeID*/, hla::Deserializer /*deserializer*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Serialize
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Serialize( HLA_UpdateFunctor& /*functor*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdate
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
sword::ObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& /*attributes*/ )
{
    return sword::ObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendCapacity
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
template < typename T >
void UrbanObjectWrapper::SendCapacity( sword::UrbanAttributes& msg )
{
    T* capacity = Retrieve< T >();
    if( capacity )
        capacity->SendState( msg );
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
    message().mutable_urban_object()->set_id( object_->GetId() );
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

    const urban::Architecture* architecture = object_->Retrieve< urban::Architecture >();
    if( architecture != 0 )
    {
        message().mutable_attributes()->mutable_architecture()->set_height( architecture->GetHeight() );
        message().mutable_attributes()->mutable_architecture()->set_floor_number( architecture->GetFloorNumber() );
        message().mutable_attributes()->mutable_architecture()->set_roof_shape( architecture->GetRoofShape().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_material( architecture->GetMaterial().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_occupation( architecture->GetOccupation() );
        message().mutable_attributes()->mutable_architecture()->set_trafficability( architecture->GetTrafficability() );
        // TODO parking
        message().mutable_attributes()->mutable_architecture()->set_parking_available( false );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateState()
{
    if( object_->HasChild() )
        return;
    if( pView_ && pView_->HideObject() )
        return;
    client::UrbanUpdate message;
    message().mutable_urban_object()->set_id( object_->GetId() );
    SendCapacity< StructuralCapacity >( *message().mutable_attributes() );
    SendCapacity< ResourceNetworkCapacity >( *message().mutable_attributes() );
    if ( message().attributes().has_structure() || message().attributes().has_infrastructures() )
        message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetID
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetID() const
{
    return id_;
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
// Name: UrbanObjectWrapper::Update
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Update( unsigned int time )
{
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Update, _1, boost::ref( *this ), time ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( ObjectAttribute_ABC* /*attribute*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( ObjectCapacity_ABC* capacity )
{
    capacities_.push_back( capacity );
}