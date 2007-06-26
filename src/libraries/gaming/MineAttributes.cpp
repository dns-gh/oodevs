// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MineAttributes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MineAttributes constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttributes::MineAttributes( kernel::Controller& controller, bool minefield /* = false */ )
    : controller_( controller )
    , minefield_( minefield )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MineAttributes destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttributes::~MineAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::Display( kernel::Displayer_ABC& displayer ) const
{
    if( !activityTime_ && !minefield_ )
        return;
    Displayer_ABC& group = displayer.Group( tools::translate( "Object", "Mine parameters" ) );
    if( activityTime_ )
        group.Display( tools::translate( "Object", "Activity time:" ), activityTime_ * Units::hours );
    if( minefield_ )
        group.Display( tools::translate( "Object", "Density:" ), density_ * Units::minesPerMeter );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void MineAttributes::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}
   
// -----------------------------------------------------------------------------
// Name: MineAttributes::UpdateData
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
template< typename T >
void MineAttributes::UpdateData( const T& message )
{
    if( !message.m.specific_attributesPresent )
        return;
    if( message.specific_attributes.t == T_ObjectAttributesSpecific_linear_mine_area )
    {
        activityTime_ = message.specific_attributes.u.linear_mine_area->activity_time;
        density_  = message.specific_attributes.u.linear_mine_area->density;
    }
    else if( message.specific_attributes.t == T_ObjectAttributesSpecific_dispersed_mine_area )
    {
        activityTime_ = message.specific_attributes.u.dispersed_mine_area->activity_time;
        density_  = message.specific_attributes.u.dispersed_mine_area->density;
    }
    else if( message.specific_attributes.t == T_ObjectAttributesSpecific_mine_jam )
    {
        activityTime_ = message.specific_attributes.u.mine_jam->activity_time;
    }
    controller_.Update( *(MineAttributes_ABC*)this );
}
