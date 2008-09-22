// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Parameter_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/GlTooltip_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: Parameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Parameter_ABC::Parameter_ABC( const QString& name )
    : name_( name )
    , position_()
    , isSet_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Parameter_ABC::~Parameter_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString Parameter_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::GetType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
std::string Parameter_ABC::GetType() const
{
    return "undefined";
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void Parameter_ABC::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const geometry::Point2f point = GetPosition();
        it->second->Draw( point.IsZero() ? where : point, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Parameter_ABC::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !toolTip_.get() )
    {
        std::auto_ptr< kernel::GlTooltip_ABC > tooltip( tools.CreateTooltip() );
        const_cast< Parameter_ABC* >( this )->toolTip_ = tooltip;
    }
    DrawToolTip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::DrawToolTip
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Parameter_ABC::DrawToolTip( const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( GetPosition() ) )
        return;
    if( !toolTip_.get() )
    {
        std::auto_ptr< GlTooltip_ABC > tooltip( tools.CreateTooltip() );
        const_cast< Parameter_ABC* >( this )->toolTip_ = tooltip;
    }
    DisplayInToolTip( *toolTip_ );
    toolTip_->Draw( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Parameter_ABC::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "name", name_ );
    if( !isSet_ )
        xos << attribute( "set", false );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "parameter" );
        it->second->Serialize( xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::AddParameter
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Parameter_ABC::AddParameter( Parameter_ABC& parameter )
{
    Register( Count(), parameter );
    if( position_.IsZero() )
        position_ = parameter.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Display
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void Parameter_ABC::Display( Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Parameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "Parameter", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void Parameter_ABC::DisplayInToolTip( Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::GetPosition
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
geometry::Point2f Parameter_ABC::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::CommitTo
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void Parameter_ABC::CommitTo( std::string& content ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( content );  
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Parameter_ABC::CommitTo( ASN1T_MissionParameter& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( asn );    
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Parameter_ABC::Clean( ASN1T_MissionParameter& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Parameter_ABC::Accept( ParameterVisitor_ABC& visitor ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Set
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
void Parameter_ABC::Set( bool isSet )
{
    isSet_ = isSet;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::IsSet
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
bool Parameter_ABC::IsSet() const
{
    return isSet_;
}
