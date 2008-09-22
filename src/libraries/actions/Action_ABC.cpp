// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Action_ABC.h"
#include "Parameter_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/ActionController.h"
#include <xeumeuleu/xml.h>

using namespace actions;

unsigned long Action_ABC::idManager_ = 0; // $$$$ SBO 2007-03-12: real id manager maybe
const QString Action_ABC::typeName_ = "action";

// -----------------------------------------------------------------------------
// Name: Action_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::Action_ABC( kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target )
    : controller_( controller )
    , type_( type )
    , target_( target )
    , id_( ++idManager_ )
    , name_( type_.GetName().c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC constructor
// Created: SBO 2008-05-28
// -----------------------------------------------------------------------------
Action_ABC::Action_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target )
    : controller_( controller )
    , type_( type )
    , target_( target )
    , id_( ++idManager_ )
    , name_( xml::attribute< std::string >( xis, "name", type_.GetName() ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC::~Action_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetTypeName
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
QString Action_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetId
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
unsigned long Action_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
QString Action_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Rename
// Created: SBO 2008-05-15
// -----------------------------------------------------------------------------
void Action_ABC::Rename( const QString& name )
{
    name_ = name;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetType
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const kernel::OrderType& Action_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::GetEntity
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& Action_ABC::GetEntity() const
{
    return target_;
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::AddParameter
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void Action_ABC::AddParameter( Parameter_ABC& parameter )
{
    ParameterContainer_ABC::Register( Count(), parameter );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void Action_ABC::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( where, viewport, tools );

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Display
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void Action_ABC::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Action", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "Action", "Value" ) ).Display( "" );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void Action_ABC::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", type_.GetId() )
        << xml::attribute( "name", name_ )
        << xml::attribute( "target", target_.GetId() );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "parameter" );
        it->second->Serialize( xos );
        xos << xml::end();
    }
    Interface().Apply( &kernel::Serializable_ABC::SerializeAttributes, xos );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Action_ABC::CommitTo( ASN1T_MissionParameters& asn ) const
{
    asn.n = Count();
    if( !asn.n )
        return;
    asn.elem = new ASN1T_MissionParameter[asn.n];
    unsigned int i = 0;
    for( CIT_Elements it = elements_.begin(); it != elements_.end() && i < asn.n; ++it )
        it->second->CommitTo( asn.elem[i++] );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Action_ABC::Clean( ASN1T_MissionParameters& asn ) const
{
    unsigned int i = 0;
    for( CIT_Elements it = elements_.begin(); it != elements_.end() && i < asn.n; ++it )
        it->second->Clean( asn.elem[i++] );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Select
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::ContextMenu
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::ContextMenu( kernel::ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}

// -----------------------------------------------------------------------------
// Name: Action_ABC::Activate
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void Action_ABC::Activate( kernel::ActionController& controller ) const
{
    controller.Activate( *this );
}
