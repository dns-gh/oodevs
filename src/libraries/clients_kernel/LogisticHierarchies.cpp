// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LogisticHierarchies.h"
#include "Automat_ABC.h"
#include "Formation_ABC.h"
#include "clients_gui/LogisticHelpers.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies constructor
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
LogisticHierarchies::LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity,
                                          const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                          const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver )
    : gui::EntityHierarchies< gui::LogisticHierarchiesBase >( controller, entity, 0 )
    , automatResolver_( automatResolver )
    , formationResolver_( formationResolver )
    , currentSuperior_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
LogisticHierarchies::~LogisticHierarchies()
{
    if( currentSuperior_ )
        currentSuperior_->RemoveSubordinate( entity_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::DoUpdate
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
void LogisticHierarchies::DoUpdate( const sword::ChangeLogisticLinks& message )
{
    const auto requester = logistic_helpers::FindParentEntity( message.requester(), automatResolver_, formationResolver_ );
    if( !requester )
        throw MASA_EXCEPTION( "requester must be a valid automat or formation" );
    auto& requesterHierarchy = dynamic_cast< LogisticHierarchies& >( requester->Get< gui::LogisticHierarchiesBase >() );
    std::vector< kernel::Entity_ABC* > superiors;
    for( int i = 0; i < message.superior().size(); ++i )
    {
        const auto superior = logistic_helpers::FindParentEntity( message.superior( i ), automatResolver_, formationResolver_ );
        if( !superior )
            throw MASA_EXCEPTION( "superior #" + boost::lexical_cast< std::string >( i ) + "must be a valid parent entity" );
        superiors.push_back( superior );
    }
    requesterHierarchy.SetSuperior( 0 );
    if( currentSuperior_ )
    {
        currentSuperior_->RemoveSubordinate( entity_ );
        currentSuperior_ = 0;
    }
    if( superiors.empty() )
        return;
    // The nominal superior plays the role of the usual superior
    requesterHierarchy.SetSuperior( superiors.front() );
    if( superiors.size() < 2 )
        return;
    // the current superior has to be handled separately
    auto& superiorHierarchy = superiors.back()->Get< gui::LogisticHierarchiesBase >();
    requesterHierarchy.currentSuperior_ = &superiorHierarchy;
    superiorHierarchy.AddSubordinate( *requester );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetLogisticSuperior
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
void LogisticHierarchies::SetLogisticSuperior( const kernel::LogisticBaseSuperior& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::Load
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
void LogisticHierarchies::Load( xml::xistream&, const kernel::Entity_ABC* )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
