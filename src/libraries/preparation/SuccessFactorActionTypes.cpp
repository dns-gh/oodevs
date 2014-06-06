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
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

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
void SuccessFactorActionTypes::Load( const tools::ExerciseConfig& config, const tools::Path& file )
{
    config.GetLoader().LoadFile( file, boost::bind( &SuccessFactorActionTypes::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionTypes::Read
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionTypes::Read( xml::xistream& xis )
{
    xis >> xml::start( "success-actions" )
            >> xml::list( "action", *this, &SuccessFactorActionTypes::ReadAction )
        >> xml::end;
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
    std::unique_ptr< SuccessFactorActionType > type( new SuccessFactorActionType( xis ) );
    Register( type->GetFunction().c_str(), *type.release() );
}
