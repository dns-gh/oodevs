// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ResourceNetwork.h"
#include "ParameterVisitor_ABC.h"
#include "Resource.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "protocol/Protocol.h"
#include <windows.h>
#include <gl/gl.h>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< kernel::Object_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( const kernel::OrderParameter& parameter, const kernel::Object_ABC& object, const std::string& resource, kernel::Controller& controller )
    : Entity< kernel::Object_ABC >( parameter, &object, controller )
{
    AddResourceParameter( resource );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( const kernel::OrderParameter& parameter, const sword::ResourceNetworkElement& resourceNetwork, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< kernel::Object_ABC >( parameter, controller )
{
    unsigned long id = resourceNetwork.object().id();
    const kernel::Object_ABC* object = resolver.FindUrbanObject( id );
    if( !object )
        object = resolver.FindObject( id );
    SetValue( object );
    AddResourceParameter( resourceNetwork.resource().name() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< kernel::Object_ABC >( parameter, controller )
{
    if( xis.has_attribute( "value" ) )
    {
        unsigned long id = xis.attribute< unsigned long >( "value" );
        const kernel::Object_ABC* object = resolver.FindUrbanObject( id );
        if( !object )
            object = resolver.FindObject( id );
        SetValue( object );
        xis >> xml::list( "parameter", *this, &ResourceNetwork::ReadParameter );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork destructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
ResourceNetwork::~ResourceNetwork()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Accept
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetwork::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::DisplayInToolTip
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetwork::DisplayInToolTip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( "", GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Draw
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& tools ) const
{
    const bool selected = tools.ShouldDisplay();
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
// Name: ResourceNetwork::GetPosition
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
geometry::Point2f ResourceNetwork::GetPosition() const
{
    if( IsSet() )
        if( const kernel::UrbanPositions_ABC* positions = GetValue()->Retrieve< kernel::UrbanPositions_ABC >() )
            return positions->Barycenter();
    return Entity< kernel::Object_ABC >::GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetwork::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.mutable_value()->Add()->mutable_resourcenetwork() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetwork::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        CommitTo( *message.mutable_resourcenetwork() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::AddResourceParameter
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetwork::AddResourceParameter( const std::string& resource )
{
    AddParameter( *new Resource( kernel::OrderParameter( tools::translate( "Parameter", "Resource" ).ascii(), "resource", false ), resource ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::ReadParameter
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
void ResourceNetwork::ReadParameter( xml::xistream& xis )
{
    if( xis.attribute< std::string >( "type" ) == "resource" )
        AddResourceParameter( xis.attribute< std::string >( "value" ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::CommitTo
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void ResourceNetwork::CommitTo( sword::ResourceNetworkElement& resourceNetwork ) const
{
    Entity< kernel::Object_ABC >::CommitTo( *resourceNetwork.mutable_object() );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetType() == "resource" )
            static_cast< const Resource* >( it->second )->CommitTo( *resourceNetwork.mutable_resource()->mutable_name() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ResourceNetwork::SerializeType() const
{
    return "resourcenetwork";
}
