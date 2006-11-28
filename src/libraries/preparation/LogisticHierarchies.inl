// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Automat_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
LogisticHierarchies< Superior, I >::LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : LogisticHierarchies_ABC< I >( controller, entity, resolver )
    , tc2_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
LogisticHierarchies< Superior, I >::~LogisticHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::Load
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::Load( xml::xistream& xis, const kernel::Automat_ABC* superior )
{
    SetSuperior( superior );
    LogisticHierarchies_ABC< I >::Load( xis );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::SetSuperior( const Superior& automat )
{
    const kernel::Automat_ABC* superior = automat; 
    LogisticHierarchies_ABC< I >::SetSuperior( const_cast< kernel::Automat_ABC* >( superior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperiorInternal
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::SetSuperiorInternal( kernel::Entity_ABC* superior )
{
    tc2_ = static_cast< kernel::Automat_ABC* >( superior );
    LogisticHierarchies_ABC< I >::SetSuperiorInternal( superior );
}
