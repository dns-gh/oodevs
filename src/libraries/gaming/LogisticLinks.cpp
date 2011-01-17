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
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                     const kernel::LogisticLevel& currentLevel, PropertiesDictionary& dictionary )
    : controller_( controller )
    , automatResolver_( automatResolver )
    , formationResolver_ ( formationResolver )
    , currentLevel_ ( currentLevel )
    , tc2_( 0 )
    , superior_( 0 )
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
    dico.Register( *this, tools::translate( "Logistic", "Logistic links/Superior" ), superior_);
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
// Name: LogisticLinks::GetFormationSuperior
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticLinks::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const sword::ChangeLogisticLinks& message )
{
    tc2_ = message.has_combat_train() ? automatResolver_.Find( message.combat_train().id() ) : 0;
    superior_ = message.logistic_base().has_automat() ?
            (kernel::Entity_ABC*)automatResolver_.Find( message.logistic_base().automat().id() ) :
            ( message.logistic_base().has_formation() ?
              (kernel::Entity_ABC*)formationResolver_.Find( message.logistic_base().formation().id() ) : 0
            );

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
                .Display( tools::translate( "Logistic", "Superior:" ),           GetSuperior() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DrawLink
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::DrawLink( const geometry::Point2f& where, Entity_ABC* agent, const GlTools_ABC& tools, float curve, bool link, bool missing ) const
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
    if(currentLevel_==kernel::LogisticLevel::none_ || currentLevel_==kernel::LogisticLevel::tc2_)
        DrawLink( where, GetTC2(), tools, 0.3f, displayLinks, displayMissing );

    glColor4f( COLOR_MAROON );
    if(currentLevel_==kernel::LogisticLevel::logistic_base_ || currentLevel_==kernel::LogisticLevel::tc2_)
        DrawLink( where, GetSuperior(), tools, 0.4f, displayLinks, displayMissing );

    glPopAttrib();
}
