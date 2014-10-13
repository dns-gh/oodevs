// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ResourceNetworkNode.h"
#include "ParameterVisitor_ABC.h"
#include "Resource.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "protocol/Protocol.h"
#include <windows.h>
#include <gl/gl.h>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
ResourceNetworkNode::ResourceNetworkNode( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< kernel::Entity_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetworkNode::ResourceNetworkNode( const kernel::OrderParameter& parameter, const kernel::Entity_ABC& object, const std::string& resource, kernel::Controller& controller )
    : Entity< kernel::Entity_ABC >( parameter, &object, controller )
{
    AddResourceParameter( resource );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetworkNode::ResourceNetworkNode( const kernel::OrderParameter& parameter, const sword::ResourceNetworkElement& resourceNetwork, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< kernel::Entity_ABC >( parameter, controller )
{
    unsigned long id = resourceNetwork.object().id();
    const kernel::Entity_ABC* object = resolver.FindUrbanObject( id );
    if( !object )
        object = resolver.FindObject( id );
    SetValue( object );
    AddResourceParameter( resourceNetwork.resource().name() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode destructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetworkNode::~ResourceNetworkNode()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::Accept
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetworkNode::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::DisplayInToolTip
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetworkNode::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::Draw
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetworkNode::Draw( const geometry::Point2f&, const gui::Viewport_ABC&, gui::GLView_ABC& tools ) const
{
    const bool selected = tools.GetOptions().IsDrawingSelection();
    GLfloat color[4];
    glGetFloatv( GL_CURRENT_COLOR, color );
    glPushAttrib( GL_LINE_BIT );
    if( selected )
    {
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, color[3] * 0.5f );
        glLineWidth( 6.f );
        tools.DrawCross( GetPosition(), GL_CROSSSIZE );
        glPopAttrib();
    }
    glLineWidth( 2.f );
    tools.DrawCross( GetPosition(), GL_CROSSSIZE );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::GetPosition
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
geometry::Point2f ResourceNetworkNode::GetPosition() const
{
    if( IsSet() )
        if( const kernel::UrbanPositions_ABC* positions = GetValue()->Retrieve< kernel::UrbanPositions_ABC >() )
            return positions->Barycenter();
    return Entity< kernel::Entity_ABC >::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetworkNode::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.add_value()->mutable_resourcenetworknode() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetworkNode::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_resourcenetworknode() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::AddResourceParameter
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetworkNode::AddResourceParameter( const std::string& resource )
{
    AddParameter( *new Resource( kernel::OrderParameter( tools::translate( "Parameter", "Resource" ).toStdString(), "resource", false ), resource ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetworkNode::CommitTo( sword::ResourceNetworkElement& resourceNetwork ) const
{
    Entity< kernel::Entity_ABC >::CommitTo( *resourceNetwork.mutable_object() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetType() == "resource" )
            static_cast< const Resource* >( it->second )->CommitTo( *resourceNetwork.mutable_resource()->mutable_name() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkNode::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ResourceNetworkNode::SerializeType() const
{
    return "resourcenetworknode";
}

