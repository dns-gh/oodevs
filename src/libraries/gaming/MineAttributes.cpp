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
    Displayer_ABC& group = displayer.Group( tools::translate( "Object", "Mine parameters" ) );
    group.Display( tools::translate( "Object", "Activity time:" ), activityTime_ * Units::hours );
    if( minefield_ )
        group.Display( tools::translate( "Object", "Density:" ), density_ * Units::minesPerMeter );
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
    if( !message.m.attributs_specifiquesPresent )
        return;
    if( message.attributs_specifiques.t == T_AttrObjectSpecific_zone_minee_lineaire )
    {
        activityTime_ = message.attributs_specifiques.u.zone_minee_lineaire->delai_activite_mines;
        density_  = message.attributs_specifiques.u.zone_minee_lineaire->densite;
    }
    else if( message.attributs_specifiques.t == T_AttrObjectSpecific_zone_minee_par_dispersion )
    {
        activityTime_ = message.attributs_specifiques.u.zone_minee_par_dispersion->delai_activite_mines;
        density_  = message.attributs_specifiques.u.zone_minee_par_dispersion->densite;
    }
    else if( message.attributs_specifiques.t == T_AttrObjectSpecific_bouchon_mines )
    {
        activityTime_ = message.attributs_specifiques.u.bouchon_mines->delai_activite_mines;
    }
    controller_.Update( *(MineAttributes_ABC*)this );
}
