// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorAction.h"
#include "SuccessFactorActionParameter.h"
#include "SuccessFactorActionType.h"
#include "SuccessFactorActionTypes.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorAction::SuccessFactorAction( const SuccessFactorActionType& type )
    : type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorAction::SuccessFactorAction( xml::xistream& xis, const SuccessFactorActionTypes& actions )
    : type_( actions.Get( xis.attribute< std::string >( "type" ).c_str() ) )
{
    xis >> xml::list( "parameter", *this, &SuccessFactorAction::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorAction::~SuccessFactorAction()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction::ReadParameter
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorAction::ReadParameter( xml::xistream& xis )
{
    std::unique_ptr< SuccessFactorActionParameter > parameter( new SuccessFactorActionParameter( xis ) );
    Register( static_cast< unsigned long >( elements_.size() ), *parameter.release() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "action" )
            << xml::attribute( "type", type_.GetFunction() );
    BOOST_FOREACH( const T_Elements::value_type parameter, elements_ )
        parameter.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction::GetType
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
const SuccessFactorActionType& SuccessFactorAction::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorAction::AddParameter
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorAction::AddParameter( const QString& name, const QString& value )
{
    Register( static_cast< unsigned long >( elements_.size() ), *new SuccessFactorActionParameter( name, value ) );
}
