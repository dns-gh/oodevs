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
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const Resolver_ABC< Automat_ABC >& resolver, const AutomatType& type, PropertiesDictionary& dictionary )
    : controller_( controller )
    , resolver_( resolver )
    , type_( type )
    , tc2_( 0 )
    , maintenanceSuperior_( 0 )
    , medicalSuperior_( 0 )
    , supplySuperior_( 0 )
{
    CreateDictionary( dictionary );
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
// Name: LogisticLinks::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void LogisticLinks::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/TC2" ), tc2_ );
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Maintenance superior" ), maintenanceSuperior_ );
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Medical superior" ), medicalSuperior_ );
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Supply superior" ), supplySuperior_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetTC2
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Automat_ABC* LogisticLinks::GetTC2() const
{
    return tc2_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMaintenance
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Automat_ABC* LogisticLinks::GetMaintenance() const
{
    return maintenanceSuperior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetMedical
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Automat_ABC* LogisticLinks::GetMedical() const
{
    return medicalSuperior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetSupply
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
Automat_ABC* LogisticLinks::GetSupply() const
{
    return supplySuperior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const ASN1T_MsgAutomateChangeLiensLogistiquesAck& message )
{
    if( message.m.oid_tc2Present )
        tc2_ = resolver_.Find( message.oid_tc2 );
    if( message.m.oid_maintenancePresent )
        maintenanceSuperior_ = resolver_.Find( message.oid_maintenance );
    if( message.m.oid_santePresent )
         medicalSuperior_ = resolver_.Find( message.oid_sante );
    if( message.m.oid_ravitaillementPresent )
        supplySuperior_ = resolver_.Find( message.oid_ravitaillement );

    controller_.Update( *(LogisticLinks_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const ASN1T_MsgAutomateChangeLiensLogistiques& message )
{
    if( message.m.oid_tc2Present )
        tc2_ = resolver_.Find( message.oid_tc2 );
    if( message.m.oid_maintenancePresent )
        maintenanceSuperior_ = resolver_.Find( message.oid_maintenance );
    if( message.m.oid_santePresent )
         medicalSuperior_ = resolver_.Find( message.oid_sante );
    if( message.m.oid_ravitaillementPresent )
        supplySuperior_ = resolver_.Find( message.oid_ravitaillement );

    controller_.Update( *(LogisticLinks_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LogisticLinks::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Logistic", "Logistic links" ) )
                .Display( tools::translate( "Logistic", "TC2:" ),                  GetTC2() )
                .Display( tools::translate( "Logistic", "Maintenance superior:" ), GetMaintenance() )
                .Display( tools::translate( "Logistic", "Medical superior:" ),     GetMedical() )
                .Display( tools::translate( "Logistic", "Supply superior:" ),      GetSupply() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DrawLink
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::DrawLink( const geometry::Point2f& where, Automat_ABC* agent, const GlTools_ABC& tools, float curve, bool link, bool missing ) const
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
void LogisticLinks::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );

    glColor4f( COLOR_YELLOW );
    DrawLink( where, GetTC2(), tools, 0.3f, displayLinks, displayMissing );

    glColor4f( COLOR_PINK );
    DrawLink( where, GetMedical(), tools, 0.4f, displayLinks, displayMissing && type_.IsLogisticMedical() );

    glColor4f( COLOR_MAROON );
    DrawLink( where, GetMaintenance(), tools, 0.5f, displayLinks, displayMissing && type_.IsLogisticMaintenance() );

    glColor4f( COLOR_ORANGE );
    DrawLink( where, GetSupply(), tools, 0.6f, displayLinks, displayMissing && type_.IsLogisticSupply() );

    glPopAttrib();
}
