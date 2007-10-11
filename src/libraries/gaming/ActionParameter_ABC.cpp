// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameter_ABC.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/GlTooltip_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::ActionParameter_ABC( const QString& name )
    : name_( name )
    , position_()
    , isSet_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::~ActionParameter_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString ActionParameter_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
QString ActionParameter_ABC::GetType() const
{
    return "undefined";
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const geometry::Point2f point = GetPosition();
        it->second->Draw( point.IsZero() ? where : point, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void ActionParameter_ABC::DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !toolTip_.get() )
    {
        std::auto_ptr< kernel::GlTooltip_ABC > tooltip( tools.CreateTooltip() );
        const_cast< ActionParameter_ABC* >( this )->toolTip_ = tooltip;
    }
    DrawToolTip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::DrawToolTip
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void ActionParameter_ABC::DrawToolTip( const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( GetPosition() ) )
        return;
    if( !toolTip_.get() )
    {
        std::auto_ptr< GlTooltip_ABC > tooltip( tools.CreateTooltip() );
        const_cast< ActionParameter_ABC* >( this )->toolTip_ = tooltip;
    }
    DisplayInToolTip( *toolTip_ );
    toolTip_->Draw( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "name", name_ );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "parameter" );
        it->second->Serialize( xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::AddParameter
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ActionParameter_ABC::AddParameter( ActionParameter_ABC& parameter )
{
    Register( Count(), parameter );
    if( position_.IsZero() )
        position_ = parameter.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::IsContext
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ActionParameter_ABC::IsContext() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Display
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Display( Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "ActionParameter", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::DisplayInToolTip
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ActionParameter_ABC::DisplayInToolTip( Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetPosition
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
geometry::Point2f ActionParameter_ABC::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::CommitTo
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void ActionParameter_ABC::CommitTo( std::string& content ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( content );  
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameter_ABC::CommitTo( ASN1T_MissionParameter& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( asn );    
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameter_ABC::CommitTo( ASN1T_OrderContext& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Clean( ASN1T_MissionParameter& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Clean( ASN1T_OrderContext& asn ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Set
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Set( bool isSet )
{
    isSet_ = isSet;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::IsSet
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
bool ActionParameter_ABC::IsSet() const
{
    return isSet_;
}
