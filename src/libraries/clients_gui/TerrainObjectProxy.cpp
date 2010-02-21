// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainObjectProxy.h"
#include "Tools.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "protocol/SimulationSenders.h"
#include <urban/Architecture.h>
#include <urban/Soil.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/Vegetation.h>


using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy(const MsgsSimToClient::MsgUrbanCreation& message, kernel::Controller& controller, urban::TerrainObject_ABC& object )
    : EntityImplementation< kernel::Entity_ABC >( controller, message.oid(), QString( message.name().c_str() ) )
    , object_( &object )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::~TerrainObjectProxy()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy operator==
// Created: MGD 2009-11-2
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::operator==( const TerrainObjectProxy& object ) const
{
        return object_ == object.object_;
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy GetName
// Created: SLG 2009-11-2
// -----------------------------------------------------------------------------
QString TerrainObjectProxy::GetName() const 
{ 
    return QString( object_->GetName().c_str() ); 
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy GetId
// Created: SLG 2009-11-2
// -----------------------------------------------------------------------------
unsigned long TerrainObjectProxy::GetId() const
{ 
    return object_->GetId(); 
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::CreateDictionary
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
void TerrainObjectProxy::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary& dictionary = *new kernel::PropertiesDictionary( controller );
    EntityImplementation< kernel::Entity_ABC >::Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "Info/Identifier" ), EntityImplementation< kernel::Entity_ABC >::id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "Info/Name" ), EntityImplementation< kernel::Entity_ABC >::name_ );

    AddDictionaryForArchitecture( dictionary );
    AddDictionaryForVegetation( dictionary );
    AddDictionaryForSoil( dictionary );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForArchitecture
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary )
{
    urban::Architecture* architecture = object_->RetrievePhysicalFeature< urban::Architecture >();
    if( architecture )
    {
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" )               , architecture->GetHeight() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" )          , architecture->GetFloorNumber() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/basementLevelNumber" )  , architecture->GetBasement() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" )            , architecture->GetRoofShape() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/material" )             , architecture->GetMaterial() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/innerCluttering" )      , architecture->GetInnerCluttering() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/facadeOpacity" )        , architecture->GetFacadeOpacity() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForVegetation
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForVegetation( kernel::PropertiesDictionary& dictionary )
{
    urban::Vegetation* vegetation = object_->RetrievePhysicalFeature< urban::Vegetation >();
    if ( vegetation )
    {
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Vegetation/type" )   , vegetation->GetType() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Vegetation/height" ) , vegetation->GetHeight() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Vegetation/density" ), vegetation->GetDensity() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForSoil
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForSoil( kernel::PropertiesDictionary& dictionary )
{
    urban::Soil* soil = object_->RetrievePhysicalFeature< urban::Soil >();
    if ( soil )
    {
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Soil/occupation" )      , soil->GetOccupation() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Soil/trafficability" )  , soil->GetTrafficability() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Soil/isMultiple" )      , soil->GetMultiplicity() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Soil/compoundClearing" ), soil->GetCompoundClearing() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}


// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::SetSelected
///** @param  selected 
//*/
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
void TerrainObjectProxy::SetSelected( bool selected ) const
{
    object_->SetSelected( selected );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Draw
///** @param  drawer 
//*/
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Draw( urban::Drawer_ABC& drawer ) const
{
    object_->Draw( drawer );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::IsInside
///** @param  point 
//    @return 
//*/
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::IsInside ( const geometry::Point2f& point ) const
{
    return object_->GetFootprint()->IsInside( point );
}