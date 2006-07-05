// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticConsigns.h"
#include "Controller.h"
#include "GlTools_ABC.h"
#include "LogSupplyConsign.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsigns constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsigns::LogisticConsigns( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogisticConsigns::~LogisticConsigns()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.insert( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.insert( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::AddConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::AddConsign( LogMedicalConsign& consign )
{
    requestedMedical_.insert( &consign );
    controller_.Update( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticConsigns::RemoveConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::RemoveConsign( LogMedicalConsign& consign )
{
    requestedMedical_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogSupplyConsign& consign )
{
    handledSupplies_.insert( &consign );
    controller_.Update( *this );
}
    
// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogSupplyConsign& consign )
{
    handledSupplies_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.insert( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::HandleConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::HandleConsign( LogMedicalConsign& consign )
{
    handledMedical_.insert( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::TerminateConsign
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogisticConsigns::TerminateConsign( LogMedicalConsign& consign )
{
    handledMedical_.erase( &consign );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::Draw
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void LogisticConsigns::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    const bool handledOnly = tools.ShouldDisplay( "RealTimeLogistic", false );

    if( handledMaintenances_.empty() && handledMedical_.empty() && handledSupplies_.empty() 
    && ( handledOnly ||
       ( requestedMaintenances_.empty() && requestedMedical_.empty() && requestedSupplies_.empty() ) ) )
        return;

    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glLineWidth( 2.0 );
    glEnable( GL_LINE_STIPPLE );
    glColor4d( COLOR_MAROON );
    for( T_MaintenanceConsigns::const_iterator it = handledMaintenances_.begin(); it != handledMaintenances_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    if( ! handledOnly )
        for( T_MaintenanceConsigns::const_iterator it = requestedMaintenances_.begin(); it != requestedMaintenances_.end(); ++it )
            (*it)->Draw( where, viewport, tools );

    
    for( T_MedicalConsigns::const_iterator it = handledMedical_.begin(); it != handledMedical_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    if( ! handledOnly )
        for( T_MedicalConsigns::const_iterator it = requestedMedical_.begin(); it != requestedMedical_.end(); ++it )
            (*it)->Draw( where, viewport, tools );

    
    for( T_SupplyConsigns::const_iterator it = handledSupplies_.begin(); it != handledSupplies_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    if( ! handledOnly )
        for( T_SupplyConsigns::const_iterator it = requestedSupplies_.begin(); it != requestedSupplies_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    glDisable( GL_LINE_STIPPLE );
    glPopAttrib();
}
