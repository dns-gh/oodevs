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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/LogisticHelpers.h"
#include "protocol/Protocol.h"
#include <boost/smart_ptr/make_shared.hpp>
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
LogisticLinks::LogisticLinks( Controller& controller,
                              const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                              const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                              const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                              gui::PropertiesDictionary& dictionary,
                              kernel::Entity_ABC& entity )
    : controller_       ( controller )
    , automatResolver_  ( automatResolver )
    , formationResolver_( formationResolver )
    , dotationResolver_ ( dotationResolver )
    , entity_           ( entity )
    , property_         ( tools::translate( "Logistic", "Logistic links/Superiors" ) )
{
    dictionary.RegisterExtension( entity_, this, property_, superiors_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::~LogisticLinks()
{
    // NOTHING
}

namespace
{
    template< typename T >
    boost::shared_ptr< LogisticLink > FindLogisticLink( const T& links, const kernel::Entity_ABC& superior )
    {
        BOOST_FOREACH( const boost::shared_ptr< LogisticLink >& link, links )
            if( &link->GetSuperior() == &superior )
                return link;
        return boost::shared_ptr< LogisticLink >();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::FindLogisticLink
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticLink* LogisticLinks::FindLogisticLink( const kernel::Entity_ABC& superior ) const
{
    return ::FindLogisticLink( superiorLinks_, superior ).get();
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const sword::LogSupplyQuotas& message )
{
    const kernel::Entity_ABC* supplier = logistic_helpers::FindLogisticEntity( message.supplier(), automatResolver_, formationResolver_ );
    assert( supplier );
    if( LogisticLink* link = FindLogisticLink( *supplier ) )
        link->Update( message.quotas(), dotationResolver_ );
    controller_.Update( gui::DictionaryUpdated( entity_, property_ ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void LogisticLinks::DoUpdate( const sword::ChangeLogisticLinks& message )
{
    T_SuperiorLinks oldLinks = superiorLinks_;
    superiorLinks_.clear();
    superiors_.clear();
    BOOST_FOREACH( const sword::ParentEntity& parentEntity, message.superior() )
    {
        const kernel::Entity_ABC* superior = logistic_helpers::FindLogisticEntity( parentEntity, automatResolver_, formationResolver_ );
        assert( superior );
        auto link = ::FindLogisticLink( oldLinks, *superior );
        if( !link )
            superiorLinks_.push_back( boost::make_shared< LogisticLink >( *superior ) );
        else
            superiorLinks_.push_back( link );
        superiors_.push_back( superior );
    }
    controller_.Update( gui::DictionaryUpdated( entity_, property_ ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::HasSuperior
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
bool LogisticLinks::HasSuperior( const kernel::Entity_ABC& entity ) const
{
    return FindLogisticLink( entity ) != 0;
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
        .Display( "Superiors", superiors_ );
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

const kernel::Entity_ABC& LogisticLinks::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLinks::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( !displayLinks && !displayMissing )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_YELLOW );

    if( superiorLinks_.empty() )
    {
        if( displayMissing )
            tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
    }
    else if( displayLinks )
    {
        BOOST_FOREACH( boost::shared_ptr< LogisticLink > link, superiorLinks_ )
        {
            geometry::Point2f pos = logistic_helpers::GetLogisticPosition( entity_ );
            if( !pos.IsZero() )
                link->Draw( pos, viewport, tools );
            glColor4f( COLOR_LIGHT_YELLOW );
        }
    }
    glPopAttrib();
}
