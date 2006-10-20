// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticLinks.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
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
    dico.Register( *this, tools::translate( "Logistique", "Liens logistiques/TC2" ), tc2_ );
    dico.Register( *this, tools::translate( "Logistique", "Liens logistiques/Superieur maintenance" ), maintenanceSuperior_ );
    dico.Register( *this, tools::translate( "Logistique", "Liens logistiques/Superieur m�dical" ), medicalSuperior_ );
    dico.Register( *this, tools::translate( "Logistique", "Liens logistiques/Superieur ravitaillement" ), supplySuperior_ );
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
void LogisticLinks::DoUpdate( const ASN1T_MsgChangeLiensLogistiquesAck& message )
{
    if( message.m.oid_tc2Present )
        tc2_ = &resolver_.Get( message.oid_tc2 );
    if( message.m.oid_maintenancePresent )
        maintenanceSuperior_ = &resolver_.Get( message.oid_maintenance );
    if( message.m.oid_santePresent )
         medicalSuperior_ = &resolver_.Get( message.oid_sante );
    if( message.m.oid_ravitaillementPresent )
        supplySuperior_ = &resolver_.Get( message.oid_ravitaillement );

    controller_.Update( *(LogisticLinks_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LogisticLinks::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Logistique", "Liens logistiques" ) )
                .Display( tools::translate( "Logistique", "TC2:" ),                      GetTC2() )
                .Display( tools::translate( "Logistique", "Sup�rieur maintenance:" ),    GetMaintenance() )
                .Display( tools::translate( "Logistique", "Sup�rieur sant�:" ),          GetMedical() )
                .Display( tools::translate( "Logistique", "Sup�rieur ravitaillement:" ), GetSupply() );
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
