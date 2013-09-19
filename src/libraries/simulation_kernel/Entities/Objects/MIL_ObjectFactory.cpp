// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectFactory.h"
#include "AttributeFactory.h"
#include "BuildableCapacity.h"
#include "CapacityFactory.h"
#include "MIL_ObjectManipulator_ABC.h"
#include "Object.h"
#include "ObjectPrototype.h"
#include "ObstacleAttribute.h"
#include "Urban/MIL_UrbanObject.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/AvoidanceCapacity.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory constructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::MIL_ObjectFactory()
    : factory_   ( new legacy::CapacityFactory() )
    , attributes_( new AttributeFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::~MIL_ObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::FindType
// Created: LGY 2012-09-17
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectFactory::FindType( const std::string& type ) const
{
    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw MASA_EXCEPTION( "Unknown object type: " + type );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::Initialize
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
            >> xml::list( "object", *this, &MIL_ObjectFactory::ReadObjectPrototype )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::ReadObjectPrototype( xml::xistream& xis )
{
    const std::string type( xis.attribute< std::string >( "type" ) );
    const std::string name( xis.attribute< std::string >( "name", "" ) );
    boost::shared_ptr< ObjectPrototype >& prototype = prototypes_[ type ];
    if( prototype.get() )
        throw MASA_EXCEPTION( "Object type redefinition: " + type );
    double pointSize = 250.;
    xis >> xml::optional >> xml::attribute( "point-size", pointSize );
    prototype.reset( new ObjectPrototype( name, type, static_cast< unsigned int >( prototypes_.size() ), pointSize ) );
    xis >> xml::list( *this, &MIL_ObjectFactory::ReadCapacity, *prototype );
    factory_->FinalizeCreate( *prototype );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::ReadCapacity
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::ReadCapacity( const std::string& capacity, xml::xistream& xis, ObjectPrototype& prototype )
{
    factory_->Create( prototype, capacity, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::CreateObject( sword::Sink_ABC& sink, const std::string& name, const std::string& type, MIL_Army_ABC* army, const TER_Localisation& location,
                                                 bool activated, unsigned int externalIdentifier, unsigned int forcedId, double density ) const
{
    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw MASA_EXCEPTION( "Unknown object type: " + type );
    const MIL_ObjectBuilder_ABC& builder = *it->second;
    Object* object = new Object( builder.GetType(), army, &location, externalIdentifier, name, forcedId );
    builder.Build( *object, sink );
    attributes_->Initialize( *object );
    if( ObstacleAttribute* pObstacle = object->RetrieveAttribute< ObstacleAttribute >() )
    {
        if( activated )
            pObstacle->Activate();
        else
            pObstacle->Deactivate();
    }
    if( density )
    {
        BuildableCapacity* capacity = object->Retrieve< BuildableCapacity >();
        if( capacity )
            capacity->SetDensity( density );
        ConstructionAttribute* attribute = object->RetrieveAttribute< ConstructionAttribute >();
        if( attribute )
            attribute->SetDensity( density );
    }
    object->Finalize();
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::ReadObject
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::CreateObject( sword::Sink_ABC& sink, xml::xistream& xis, MIL_Army_ABC* army ) const
{
    const std::string type( xis.attribute< std::string >( "type" ) );

    CIT_Prototypes it = prototypes_.find( type );
    if( it == prototypes_.end() )
        throw MASA_EXCEPTION( "Unknown object prototype: " + type );
    TER_Localisation location;
    location.Read( xis );
    const MIL_ObjectBuilder_ABC& builder = *it->second;
    // $$$$ SBO 2009-06-08: Check geometry constraint
    std::auto_ptr< Object > object( new Object( xis, builder.GetType(), army, &location ) );
    builder.Build( *object, sink );
    xis >> xml::optional
        >> xml::start( "attributes" )
            >> xml::list( *this, &MIL_ObjectFactory::ReadAttributes, *object )
        >> xml::end;
    object->Finalize();
    return object.release();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::CreateObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::CreateObject( sword::Sink_ABC& sink, const sword::MissionParameters& message, MIL_Army_ABC* army,
                                                 sword::ObjectMagicActionAck_ErrorCode& value ) const
{
    CIT_Prototypes it = prototypes_.find( message.elem( 0 ).value( 0 ).acharstr() );
    if( it == prototypes_.end() )
    {
        value = sword::ObjectMagicActionAck::error_invalid_object;
        return 0;
    }
    TER_Localisation location;
    double rPointSize = it->second->GetPointSize();
    if( ! NET_ASN_Tools::ReadLocation( message.elem( 1 ).value( 0 ).location(), location, rPointSize ) )
        return 0;
    const MIL_ObjectBuilder_ABC& builder = *it->second;
    Object* pObject = new Object( builder.GetType(), army, &location, 0u, message.elem( 2 ).value( 0 ).acharstr() );
    builder.Build( *pObject, sink );
    try
    {
        if( message.elem_size() < 5 )
            attributes_->Initialize( *pObject );
        else
            attributes_->Create( *pObject, message.elem( 4 ) );
    }
    catch( const std::exception& )
    {
        value = sword::ObjectMagicActionAck::error_invalid_object;
        return 0;
    }

    pObject->Finalize();
    MIL_ObjectManipulator_ABC& obj = pObject->operator()();
    obj.Construct();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::CreateObject( sword::Sink_ABC& sink, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army ) const
{
    CIT_Prototypes it = prototypes_.find( builder.GetType().GetName() );
    if( it == prototypes_.end() )
        return 0;
    const MIL_ObjectBuilder_ABC& objectBuilder = *it->second;
    Object* pObject = new Object( objectBuilder.GetType(), army, 0, 0u );
    objectBuilder.Build( *pObject, sink );
    builder.Build( *pObject, sink );
    pObject->Finalize();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::CreateUrbanObject
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
MIL_UrbanObject_ABC* MIL_ObjectFactory::CreateUrbanObject( sword::Sink_ABC& sink, xml::xistream& xis, MIL_UrbanObject_ABC* parent ) const
{
    CIT_Prototypes it = prototypes_.find( "urban block" );
    if( it == prototypes_.end() )
        return 0;
    const MIL_ObjectBuilder_ABC& builder = *it->second;
    MIL_UrbanObject* pObject = new MIL_UrbanObject( xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ),
                                                    builder.GetType(), parent );
    builder.Build( *pObject, sink );
    pObject->ReadData( xis );
    pObject->Finalize();
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::ReadAttributes
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::ReadAttributes( const std::string& attribute, xml::xistream& xis, Object& object ) const
{
    attributes_->Create( object, attribute, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::GetDangerousObjects
// Created: CMA 2011-11-28
// -----------------------------------------------------------------------------
std::vector< unsigned int > MIL_ObjectFactory::GetDangerousObjects() const
{
    std::vector< unsigned int > dangerousIDs;
    for( auto it = prototypes_.begin(); it != prototypes_.end(); ++it )
        if( IsDangerousObject( *it->second ))
            dangerousIDs.push_back( it->second->GetID() );
    return dangerousIDs;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::GetMaxAvoidanceDistance
// Created: CMA 2012-04-25
// -----------------------------------------------------------------------------
double MIL_ObjectFactory::GetMaxAvoidanceDistance() const
{
    double maxDistance = 0.;
    for( auto it = prototypes_.begin(); it != prototypes_.end(); ++it )
        if( const AvoidanceCapacity* avoidance = it->second->GetType().GetCapacity< AvoidanceCapacity >() )
            maxDistance = std::max( maxDistance, avoidance->GetDistance() );
    return maxDistance;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::Update
// Created: LGY 2012-08-31
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Update( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object ) const
{
    factory_->Update( object, capacity, xis );
}
