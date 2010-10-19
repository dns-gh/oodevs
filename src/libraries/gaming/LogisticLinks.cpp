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
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, PropertiesDictionary& dictionary )
    : controller_( controller )
    , automatResolver_( automatResolver )
    , formationResolver_ ( formationResolver )
    , tc2_( 0 )
    , automatSuperior_( 0 )
    , formationSuperior_( 0 )
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
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Automat superior" ), automatSuperior_ );
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Formation superior" ), formationSuperior_ );
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
// Name: LogisticLinks::GetAutomatSuperior
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
Automat_ABC* LogisticLinks::GetAutomatSuperior() const
{
    return automatSuperior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetFormationSuperior
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
Formation_ABC* LogisticLinks::GetFormationSuperior() const
{
    return formationSuperior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::GetFormationSuperior
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticLinks::GetSuperior() const
{
    kernel::Entity_ABC* retval = 0;
    if( automatSuperior_ )
        retval = automatSuperior_;
    else if( formationSuperior_ )
        retval = formationSuperior_;
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const Common::MsgChangeLogisticLinks& message )
{
    tc2_ = message.has_tc2()? automatResolver_.Find( message.tc2().id() ) : 0;
    automatSuperior_ = message.logistic_base().has_automat() ? automatResolver_.Find( message.logistic_base().automat().id() ) : 0;
    formationSuperior_ = message.logistic_base().has_formation() ? formationResolver_.Find( message.logistic_base().formation().id() ) : 0;

    controller_.Update( *(LogisticLinks_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LogisticLinks::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Logistic", "Logistic links" ) )
                .Display( tools::translate( "Logistic", "TC2:" ),                GetTC2() )
                .Display( tools::translate( "Logistic", "Automat superior:" ),   GetAutomatSuperior() )
                .Display( tools::translate( "Logistic", "Formation superior:" ), GetFormationSuperior() );
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
// Name: LogisticLinks::DrawLink
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void LogisticLinks::DrawLink( const geometry::Point2f& where, Formation_ABC* agent, const GlTools_ABC& tools, float curve, bool link, bool missing ) const
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

    glColor4f( COLOR_MAROON );
    if( GetAutomatSuperior() )
        DrawLink( where, GetAutomatSuperior(), tools, 0.4f, displayLinks, displayMissing &&
                GetAutomatSuperior()->GetLogisticLevel() != kernel::LogisticLevel::none_ );
    else if ( GetFormationSuperior() )
        DrawLink( where, GetFormationSuperior(), tools, 0.5f, displayLinks, displayMissing &&
                GetFormationSuperior()->GetLogisticLevel() != kernel::LogisticLevel::none_ );

    glPopAttrib();
}
