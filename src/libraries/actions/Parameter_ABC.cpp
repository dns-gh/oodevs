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
#include "ParameterVisitor_ABC.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/GlTooltip_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: Parameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Parameter_ABC::Parameter_ABC( const QString& name )
    : name_ ( name )
    , isSet_( false )
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
// Name: Parameter_ABC::SetName
// Created: ABR 2014-03-07
// -----------------------------------------------------------------------------
void Parameter_ABC::SetName( const QString& name )
{
    name_ = name;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
QString Parameter_ABC::GetDisplayName( kernel::DisplayExtractor_ABC& /*extractor*/ ) const
{
    return "";
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
// Name: Parameter_ABC::SetKeyName
// Created: LDC 2012-09-05
// -----------------------------------------------------------------------------
void Parameter_ABC::SetKeyName( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::IsOptional
// Created: LDC 2012-09-05
// -----------------------------------------------------------------------------
bool Parameter_ABC::IsOptional() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::IsContext
// Created: SLI 2014-04-04
// -----------------------------------------------------------------------------
bool Parameter_ABC::IsContext() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void Parameter_ABC::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    geometry::Point2f point = GetPosition();
    point = point.IsZero() ? where : point;
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( point, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::DisplayTooltip
// Created: AGE 2007-07-10
// -----------------------------------------------------------------------------
void Parameter_ABC::DisplayTooltip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    if( !toolTip_ )
        const_cast< Parameter_ABC* >( this )->toolTip_ = tools.CreateTooltip();
    DrawToolTip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::DrawToolTip
// Created: SBO 2007-06-25
// -----------------------------------------------------------------------------
void Parameter_ABC::DrawToolTip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( GetPosition() ) )
        return;
    if( !toolTip_ )
        const_cast< Parameter_ABC* >( this )->toolTip_ = tools.CreateTooltip();
    DisplayInToolTip( *toolTip_ );
    toolTip_->Draw( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Parameter_ABC::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_.toStdString() );
    std::string id = GetKeyName();
    if( !id.empty() )
        xos << xml::attribute( "identifier", id.c_str() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "parameter" );
        it->second->Serialize( xos );
        xos << xml::end;
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
    displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
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
// Name: Parameter_ABC::GetKeyName
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
std::string Parameter_ABC::GetKeyName() const
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::CommitTo
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void Parameter_ABC::CommitTo( std::string& content ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->CommitTo( content );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Parameter_ABC::CommitTo( sword::MissionParameter& message ) const
{
    ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >* list =message.mutable_value();
    message.set_null_value( elements_.size() == 0 );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        if( !it->second->IsSet() )
        {
            message.Clear();
            message.set_null_value( true );
            return;
        }
        sword::MissionParameter_Value* elementValue = list->Add();
        it->second->CommitTo( *elementValue );
    }
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Parameter_ABC::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    if( visitor.IsRecursive() )
        for( auto it = elements_.begin(); it != elements_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: Parameter::CommitTo
// Created: MGD 2010-11-10
// -----------------------------------------------------------------------------
void Parameter_ABC::CommitTo( sword::MissionParameter_Value& /*message*/ ) const
{
    throw( "No specific implementation avalaible" );
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool Parameter_ABC::CheckKnowledgeValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Parameter_ABC::RetrieveKnowledgeIfPossible
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void Parameter_ABC::RetrieveKnowledgeIfPossible()
{
    // Nothing
}
