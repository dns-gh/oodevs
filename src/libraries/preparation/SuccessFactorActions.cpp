// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorActions.h"
#include "SuccessFactorAction.h"
#include "SuccessFactorActionTypes.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActions::SuccessFactorActions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActions::SuccessFactorActions( xml::xistream& xis, const SuccessFactorActionTypes& actionsTypes )
{
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &SuccessFactorActions::ReadAction, actionsTypes )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActions::~SuccessFactorActions()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions::ReadAction
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorActions::ReadAction( xml::xistream& xis, const SuccessFactorActionTypes& actionsTypes )
{
    std::unique_ptr< SuccessFactorAction > action( new SuccessFactorAction( xis, actionsTypes ) );
    Register( static_cast< unsigned long >( elements_.size() ), *action.release() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorActions::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "actions" );
    BOOST_FOREACH( const T_Elements::value_type action, elements_ )
        action.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActions::AddAction
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorActions::AddAction( SuccessFactorAction& action )
{
    Register( static_cast< unsigned long >( elements_.size() ), action );
}
