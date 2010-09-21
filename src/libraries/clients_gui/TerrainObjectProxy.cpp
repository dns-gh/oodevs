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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <urban/Architecture.h>
#include <urban/TerrainObject_ABC.h>

using namespace gui;

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object,
                                        unsigned int id, const QString& name )
    : EntityImplementation< kernel::Entity_ABC >( controller, id, name )
    , Creatable< TerrainObjectProxy >( controller, this )
    , object_( &object )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy constructor
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
TerrainObjectProxy::TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object )
    : EntityImplementation< kernel::Entity_ABC >( controller, object.GetId(), QString( object.GetName().c_str() ) )
    , Creatable< TerrainObjectProxy >( controller, this )
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
    Destroy();
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
// Name: TerrainObjectProxy::Update
// Created: SLG 2010-06-22
// -----------------------------------------------------------------------------
void TerrainObjectProxy::DoUpdate( const MsgsSimToClient::MsgUrbanUpdate& /*msg*/ )
{
    Touch();
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
    // $$$$ _RC_ JSR 2010-08-20: la méthode n'est "quasiment" pas appelée, mais ça peut causer des conflits d'id si on appelle
    // la méthode en tant que Entity_ABC. L'id sera sûrement le même qu'un agent ou un objet. 
    return object_->GetId();
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Select
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
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
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::AddDictionaryForArchitecture
// Created: SLG 2009-12-09
// -----------------------------------------------------------------------------
void TerrainObjectProxy::AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary )
{
    urban::Architecture* architecture = object_->Retrieve< urban::Architecture >();
    if( architecture )
    {
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" )               , architecture->GetHeight() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" )          , architecture->GetFloorNumber() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" )            , architecture->GetRoofShape() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/material" )             , architecture->GetMaterial() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" )           , architecture->GetOccupation() );
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" )       , architecture->GetTrafficability() );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::SetSelected
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
void TerrainObjectProxy::SetSelected( bool selected ) const
{
    object_->SetSelected( selected );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Draw
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
void TerrainObjectProxy::Draw( urban::Drawer_ABC& drawer, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& /*tools*/ ) const
{
    object_->Draw( drawer );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::IsInside
// Created: FDS 2010-01-15
// -----------------------------------------------------------------------------
bool TerrainObjectProxy::IsInside( const geometry::Point2f& point ) const
{
    return object_->GetFootprint()->IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: TerrainObjectProxy::Barycenter
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
geometry::Point2f TerrainObjectProxy::Barycenter() const
{
    return object_->GetFootprint()->Barycenter();
}
