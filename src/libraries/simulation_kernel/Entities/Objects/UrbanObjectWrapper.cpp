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
#include "StructuralCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_ObjectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_ObjectLoader.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_Localisation.h"
#include "geometry/Types.h"
#include "UrbanType.h"
#include <Urban/ColorRGBA.h>
#include <Urban/PhysicalFeature_ABC.h>
#include <Urban/Architecture.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/StaticModel.h>
#include <urban/MaterialCompositionType.h>
#include <boost/bind.hpp>

// HLA
#include "HLA/HLA_Object_ABC.h"
#include "HLA/HLA_UpdateFunctor.h"

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper constructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::UrbanObjectWrapper( const MIL_ObjectBuilder_ABC& builder, const urban::TerrainObject_ABC& object )
    : MIL_Object_ABC( 0, builder.GetType() )
    , object_( object )
    , id_( idManager_.GetFreeId() )
    , manipulator_( *new MIL_ObjectManipulator( *this ) )
{
    std::string name = object.GetName(); 
    geometry::Polygon2f::T_Vertices vertices = object.GetFootprint()->Vertices();
    std::vector< MT_Vector2D > vector; 
    for( geometry::Polygon2f::CIT_Vertices it = vertices.begin(); it != vertices.end(); ++it )
        vector.push_back( MT_Vector2D( it->X(), it->Y() ) );
    Initialize( TER_Localisation( TER_Localisation::ePolygon , vector ) );
    builder.Build( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper destructor
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper::~UrbanObjectWrapper() 
{
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::WriteODB
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::WriteODB( xml::xostream& xos ) const
{
    //NOTHING
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
// Name: UrbanObjectWrapper::ReadBlockCapacity
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateCapacities( const std::string& capacity, xml::xistream& xis )
{
    if( capacity == "structural" )
    {
        StructuralCapacity* structural = Retrieve< StructuralCapacity >();
        if( structural )
            structural->Load( xis );
    }
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
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_Army_ABC& team )
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > UrbanObjectWrapper::CreateKnowledge( const MIL_KnowledgeGroup& group )
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
void UrbanObjectWrapper::Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Serialize
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Serialize( HLA_UpdateFunctor& functor ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::OnUpdate
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode UrbanObjectWrapper::OnUpdate( const Common::MsgMissionParameter_Value& asn )
{
    return MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendCreation
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendCreation() const
{
    if( object_.HasChild() )
        return;
    client::UrbanCreation message;
    message().set_oid( object_.GetId() );
    message().set_name( object_.GetName() );
    message().mutable_location()->set_type( Common::MsgLocation_Geometry_polygon );
    const geometry::Polygon2f::T_Vertices& points = object_.GetFootprint()->Vertices();
    for( geometry::Polygon2f::CIT_Vertices it = points.begin(); it != points.end(); ++it )
    {
        Common::MsgCoordLatLong* point = message().mutable_location()->mutable_coordinates()->add_elem();
        point->set_latitude( it->X() );
        point->set_longitude( it->Y() );

    }

    const ColorRGBA* color = object_.GetColor();
    if ( color != 0 )
    {
        message().mutable_attributes()->mutable_color()->set_red( color->Red() );
        message().mutable_attributes()->mutable_color()->set_green( color->Green() );
        message().mutable_attributes()->mutable_color()->set_blue( color->Blue() );
        message().mutable_attributes()->mutable_color()->set_alpha( color->Alpha() );
    }

    const urban::Architecture* architecture = object_.RetrievePhysicalFeature< urban::Architecture >();
    if ( architecture != 0 )
    {       
        message().mutable_attributes()->mutable_architecture()->set_height( architecture->GetHeight() );
        message().mutable_attributes()->mutable_architecture()->set_floor_number( architecture->GetFloorNumber() );
        message().mutable_attributes()->mutable_architecture()->set_roof_shape( architecture->GetRoofShape().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_material( architecture->GetMaterial().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_occupation( architecture->GetOccupation() );
        message().mutable_attributes()->mutable_architecture()->set_trafficability( architecture->GetTrafficability() );
    }
    //TODO : faire une boucle sur toutes les capacités
    const StructuralCapacity* structural = Retrieve< StructuralCapacity >();
    if( structural )
        structural->SendState( *message().mutable_attributes() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendDestruction
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendDestruction() const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::SendFullState
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::SendFullState() const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::UpdateState
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::UpdateState()
{
    if( object_.HasChild() )
        return;
    client::UrbanUpdate message;
    message().set_oid( object_.GetId() );
    Retrieve< StructuralCapacity >()->SendState( *message().mutable_attributes() );
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
// Name: UrbanObjectWrapper::GetObject
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC& UrbanObjectWrapper::GetObject()
{
    return object_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetObject
// Created: SLG 2010-06-24
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC& UrbanObjectWrapper::GetObject() const
{
    return object_;
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
void UrbanObjectWrapper::Register( ObjectAttribute_ABC* attribute )
{

}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::Register
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanObjectWrapper::Register( ObjectCapacity_ABC* capacity )
{
    capacities_.push_back( capacity );
}

// -----------------------------------------------------------------------------
// Name: UrbanObjectWrapper::GetMaterial
// Created: SLG 2010-06-24
// -----------------------------------------------------------------------------
unsigned int UrbanObjectWrapper::GetMaterial() const
{
    const urban::Architecture* architecture = object_.RetrievePhysicalFeature< urban::Architecture >();
    if( architecture )
        return UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId();
    return std::numeric_limits< unsigned int >::max();
}
