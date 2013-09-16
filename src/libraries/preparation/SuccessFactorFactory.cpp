// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorFactory.h"
#include "Model.h"
#include "SuccessFactor.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorFactory constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorFactory::SuccessFactorFactory( kernel::Controllers& controllers, const Model& model, const SuccessFactorActionTypes& actionTypes )
    : controllers_( controllers )
    , model_( model )
    , actionTypes_( actionTypes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorFactory destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorFactory::~SuccessFactorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorFactory::Create
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactor* SuccessFactorFactory::Create( xml::xistream& xis ) const
{
    return new SuccessFactor( xis, controllers_, *model_.profiles_, *model_.scores_, actionTypes_ );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorFactory::Create
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactor* SuccessFactorFactory::Create( const QString& name ) const
{
    return new SuccessFactor( name, controllers_ );
}
