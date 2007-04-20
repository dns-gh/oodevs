// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "LogSupplyConsign.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::LogisticConsigns_ABC
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
LogisticConsigns_ABC< ConcreteExtension, Consign >::LogisticConsigns_ABC( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::~LogisticConsigns_ABC
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
LogisticConsigns_ABC< ConcreteExtension, Consign >::~LogisticConsigns_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns::IsRelevant
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
bool LogisticConsigns_ABC< ConcreteExtension, Consign >::IsRelevant() const
{
    return ! requested_.empty() || ! handled_.empty();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::AddConsign
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
void LogisticConsigns_ABC< ConcreteExtension, Consign >::AddConsign( Consign& consign )
{
    requested_.insert( &consign );
    controller_.Update( *(ConcreteExtension*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::RemoveConsign
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
void LogisticConsigns_ABC< ConcreteExtension, Consign >::RemoveConsign( Consign& consign )
{
    requested_.erase( &consign );
    controller_.Update( *(ConcreteExtension*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::HandleConsign
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
void LogisticConsigns_ABC< ConcreteExtension, Consign >::HandleConsign( Consign& consign )
{
    handled_.insert( &consign );
    controller_.Update( *(ConcreteExtension*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::TerminateConsign
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
void LogisticConsigns_ABC< ConcreteExtension, Consign >::TerminateConsign( Consign& consign )
{
    handled_.erase( &consign );
    controller_.Update( *(ConcreteExtension*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsigns_ABC::Draw
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteExtension, typename Consign >
void LogisticConsigns_ABC< ConcreteExtension, Consign >::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( ( handled_.empty() && requested_.empty() ) || ! tools.ShouldDisplay( "RealTimeLogistic" ) )
        return;

    const bool handledOnly = tools.ShouldDisplay( "RealTimeLogistic", false );
    if( handled_.empty() && handledOnly )
        return;

    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glLineWidth( 2.0 );
    glEnable( GL_LINE_STIPPLE );
    SelectColor();
    for( T_Consigns::const_iterator it = handled_.begin(); it != handled_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    if( ! handledOnly )
        for( T_Consigns::const_iterator it = requested_.begin(); it != requested_.end(); ++it )
            (*it)->Draw( where, viewport, tools );
    glDisable( GL_LINE_STIPPLE );
    glPopAttrib();
}
