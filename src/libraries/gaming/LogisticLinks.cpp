// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticLinks.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DataDictionary.h"

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const AutomatType& type, DataDictionary& dictionary )
    : controller_( controller )
    , resolver_( resolver )
    , type_( type )
    , tc2_( 0 )
    , maintenanceSuperior_( 0 )
    , medicalSuperior_( 0 )
    , supplySuperior_( 0 )
{
    dictionary.Register( "Liens logistiques/TC2", tc2_ );
    dictionary.Register( "Liens logistiques/Superieur maintenance", maintenanceSuperior_ );
    dictionary.Register( "Liens logistiques/Superieur médical", medicalSuperior_ );
    dictionary.Register( "Liens logistiques/Superieur ravitaillement", supplySuperior_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::~LogisticLinks()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetTC2
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent_ABC* LogisticLinks::GetTC2() const
{
    return Resolve( tc2_, idTc2_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMaintenance
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent_ABC* LogisticLinks::GetMaintenance() const
{
    return Resolve( maintenanceSuperior_, idMaintenance_);
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMedical
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent_ABC* LogisticLinks::GetMedical() const
{
    return Resolve( medicalSuperior_, idMedical_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetSupply
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent_ABC* LogisticLinks::GetSupply() const
{
    return Resolve( supplySuperior_, idSupply_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Resolve
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent_ABC* LogisticLinks::Resolve( Agent_ABC*& agent, unsigned long id ) const
{
    if( ! agent )
        agent = resolver_.Find( id );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::UpdateData
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void LogisticLinks::UpdateData( const T& message )
{
    if( message.m.oid_tc2Present ) {
        idTc2_ = message.oid_tc2; tc2_ = 0;
    }
    if( message.m.oid_maintenancePresent ) {
        idMaintenance_ = message.oid_maintenance; maintenanceSuperior_ = 0;
    }
    if( message.m.oid_santePresent ) {
        idMedical_ = message.oid_sante; medicalSuperior_ = 0;
    }
    if( message.m.oid_ravitaillementPresent ) {
        idSupply_ = message.oid_ravitaillement; supplySuperior_ = 0;
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const ASN1T_MsgChangeLiensLogistiquesAck& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LogisticLinks::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Liens logistiques" )
                .Display( "TC2:",                     GetTC2() )
                .Display( "Supérieur maintenance:",   GetMaintenance() )
                .Display( "Supérieur santé:",         GetMedical() )
                .Display( "Supérieur ravitaillement:", GetSupply() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DrawLink
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::DrawLink( const geometry::Point2f& where, Agent_ABC* agent, const GlTools_ABC& tools, float curve, bool link, bool missing ) const
{
    if( agent && link )
        tools.DrawCurvedArrow( where, agent->Get< Positions >().GetPosition(), curve );
    else if( ! agent && missing )
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsInside( where );
    if( ! displayLinks && ! displayMissing )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );

    glColor4d( COLOR_YELLOW );
    DrawLink( where, GetTC2(), tools, 0.3f, displayLinks, displayMissing );

    glColor4d( COLOR_PINK );
    DrawLink( where, GetMedical(), tools, 0.4f, displayLinks, displayMissing && type_.IsLogisticMedical() );

    glColor4d( COLOR_MAROON );
    DrawLink( where, GetMaintenance(), tools, 0.5f, displayLinks, displayMissing && type_.IsLogisticMaintenance() );

    glColor4d( COLOR_ORANGE );
    DrawLink( where, GetSupply(), tools, 0.6f, displayLinks, displayMissing && type_.IsLogisticSupply() );

    glPopAttrib();
}
