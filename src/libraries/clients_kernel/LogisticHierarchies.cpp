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
#include <boost/foreach.hpp>

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
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
LogisticHierarchies::~LogisticHierarchies()
{
    // NOTHING
}

namespace
{
    template< typename T >
    void ClearSubordinate( const tools::Resolver_ABC< T >& resolver, const kernel::Entity_ABC& subordinate )
    {
        for( auto it = resolver.CreateIterator(); it.HasMoreElements(); )
            if( auto* logisticHierarchy = const_cast< T& >( it.NextElement() ).Retrieve< gui::LogisticHierarchiesBase >() )
                logisticHierarchy->UnregisterSubordinate( subordinate );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::DoUpdate
// Created: SLI 2014-08-19
// -----------------------------------------------------------------------------
void LogisticHierarchies::DoUpdate( const sword::ChangeLogisticLinks& message )
{
    auto& requester = *logistic_helpers::FindLogisticEntity( message.requester(), automatResolver_, formationResolver_ );
    if( requester.GetId() != entity_.GetId() )
        return;
    ClearSubordinate( automatResolver_, requester );
    ClearSubordinate( formationResolver_, requester );
    BOOST_FOREACH( const auto& parentEntity, message.superior() )
    {
        const auto superior = logistic_helpers::FindLogisticEntity( parentEntity, automatResolver_, formationResolver_ );
        if( auto* logisticHierarchy = superior->Retrieve< gui::LogisticHierarchiesBase >() )
            logisticHierarchy->RegisterSubordinate( requester );
    }
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
