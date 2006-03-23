// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticLinks.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "GlTools_ABC.h"
#include "Positions.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
    , maintenanceSuperior_( 0 )
    , medicalSuperior_( 0 )
    , supplySuperior_( 0 )
{
    
}
    
// -----------------------------------------------------------------------------
// Name: LogisticLinks destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::~LogisticLinks()
{

}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetTC2
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent* LogisticLinks::GetTC2() const
{
    return Resolve( tc2_, idTc2_ );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMaintenance
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent* LogisticLinks::GetMaintenance() const
{
    return Resolve( maintenanceSuperior_, idMaintenance_);
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMedical
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent* LogisticLinks::GetMedical() const
{
    return Resolve( medicalSuperior_, idMedical_ );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetSupply
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent* LogisticLinks::GetSupply() const
{
    return Resolve( supplySuperior_, idSupply_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Resolve
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Agent* LogisticLinks::Resolve( Agent*& agent, unsigned long id ) const
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
                .Display( "Sup�rieur maintenance:",   GetMaintenance() )
                .Display( "Sup�rieur sant�:",         GetMedical() )
                .Display( "Sup�rieur ravitaillement:", GetSupply() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DrawLink
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::DrawLink( const geometry::Point2f& where, Agent* agent, const GlTools_ABC& tools, float curve ) const
{
    if( agent )
        tools.DrawCurvedArrow( where, agent->Get< Positions >().GetPosition(), curve );
    else // $$$$ AGE 2006-03-17: Pas vraiment ca non plus
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    
    glColor4d( COLOR_YELLOW );  
    DrawLink( where, GetTC2(), tools, 0.1f );

    glColor4d( COLOR_PINK );
    DrawLink( where, GetMedical(), tools, -0.1f );

    glColor4d( COLOR_MAROON );
    DrawLink( where, GetMaintenance(), tools, 0.2f );

    glColor4d( COLOR_ORANGE );
    DrawLink( where, GetSupply(), tools, -0.2f );    

    glPopAttrib();
}