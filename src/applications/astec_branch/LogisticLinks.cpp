// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticLinks.h"
#include "Controller.h"
#include "Displayer_ABC.h"

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
                .Display( "Supérieur maintenance:",   GetMaintenance() )
                .Display( "Supérieur santé:",         GetMedical() )
                .Display( "Supérieur ravitaillement:", GetSupply() );
}
