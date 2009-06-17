// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorActionTypes.h"
#include "SuccessFactorActionType.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionTypes::SuccessFactorActionTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionTypes::~SuccessFactorActionTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes::Load
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionTypes::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &SuccessFactorActionTypes::ReadAction )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes::Purge
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionTypes::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes::ReadAction
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionTypes::ReadAction( xml::xistream& xis )
{
    std::auto_ptr< SuccessFactorActionType > type( new SuccessFactorActionType( xis ) );
    Register( type->GetFunction().c_str(), *type.release() );
}
