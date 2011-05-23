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
#include "LogisticLink.h"
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
#include <boost/foreach.hpp>

using namespace kernel;


namespace kernel
{
    class Agent_ABC;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                     const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                     const kernel::LogisticLevel& currentLevel, PropertiesDictionary& dictionary )
    : controller_       ( controller )
    , dictionary_       ( dictionary )
    , automatResolver_  ( automatResolver )
    , formationResolver_( formationResolver )
    , dotationResolver_ ( dotationResolver )
    , currentLevel_     ( currentLevel )
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
    dictionary_.Register( *this, tools::translate( "Logistic", "Logistic links/Superiors" ), superiors_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::FindLogisticEntity
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
kernel::Entity_ABC* LogisticLinks::FindLogisticEntity( const sword::ParentEntity& message ) const
{
    if( message.has_automat() )
        return (kernel::Entity_ABC*)automatResolver_.Find( message.automat().id() );
    else if( message.has_formation() )
        return (kernel::Entity_ABC*)formationResolver_.Find( message.formation().id() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::FindLogisticLink
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticLink* LogisticLinks::FindLogisticLink( const kernel::Entity_ABC& superior ) const
{
    BOOST_FOREACH( boost::shared_ptr< LogisticLink > link, superiorLinks_ )
    {
        if( &link->GetSuperior() == &superior )
            return link.get();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const sword::LogSupplyQuotas& message )
{
    kernel::Entity_ABC* supplier = FindLogisticEntity( message.supplier() );
    assert( supplier );
    LogisticLink* link = FindLogisticLink( *supplier );
    if( link )
        link->Update( message.quotas(), dotationResolver_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const sword::ChangeLogisticLinks& message )
{
    superiorLinks_.clear();
    superiors_.clear();
    BOOST_FOREACH( const sword::ParentEntity& parentEntity, message.superior() )
    {
        const kernel::Entity_ABC* superior = FindLogisticEntity( parentEntity );
        assert( superior );
        superiorLinks_.push_back( boost::shared_ptr< LogisticLink >( new LogisticLink( *superior ) ) );
        superiors_.push_back( superior );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::HasSuperior
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
bool LogisticLinks::HasSuperior( const kernel::Entity_ABC& entity ) const
{
    return FindLogisticLink( entity ) != 0;
    //return currentSuperior_ == &entity || nominalSuperior_ == &entity;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::HasSuperior
// Created: NLD 2011-03-29
// -----------------------------------------------------------------------------
tools::Iterator< const LogisticLink& > LogisticLinks::CreateSuperiorLinksIterator() const
{
    return new tools::SimpleIterator< const LogisticLink&, T_SuperiorLinks >( superiorLinks_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LogisticLinks::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Logistic", "Logistic links" ) )
        .Display( "Superiors", superiors_);
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogisticLinks::GetNominalSuperior() const
{
    if( superiorLinks_.empty() )
        return 0;
    return &superiorLinks_.front()->GetSuperior();
}

const kernel::Entity_ABC* LogisticLinks::GetCurrentSuperior() const
{
    if( superiorLinks_.empty() )
        return 0;
    return &superiorLinks_.back()->GetSuperior();
}

// --------------------------------------------------   ---------------------------
// Name: LogisticLinks::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( !displayLinks && !displayMissing )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_YELLOW );

    if( superiorLinks_.empty() )
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
    else
    {
        BOOST_FOREACH( boost::shared_ptr< LogisticLink > link, superiorLinks_ )
        {
            link->Draw( where, viewport, tools );
            glColor4f( COLOR_LIGHT_YELLOW );
        }
    }
    glPopAttrib();
}
