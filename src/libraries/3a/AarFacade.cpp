// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AarFacade.h"
#include "FunctionFactory.h"
#include "ConstantsFactory.h"
#include "ExtractorsFactory.h"
#include "ReductionsFactory.h"
#include "ResultsFactory.h"
#include "TransformationsFactory.h"
#include "StaticModel.h"

// -----------------------------------------------------------------------------
// Name: AarFacade constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
AarFacade::AarFacade( dispatcher::ClientPublisher_ABC& publisher, int context, const aar::StaticModel_ABC& model )
    : factory_( new FunctionFactory() )
{
    factory_->Add( std::unique_ptr< ElementFactory_ABC >( new ConstantsFactory() ) );
    factory_->Add( std::unique_ptr< ElementFactory_ABC >( new ExtractorsFactory( model ) ) );
    factory_->Add( std::unique_ptr< ElementFactory_ABC >( new ReductionsFactory() ) );
    factory_->Add( std::unique_ptr< ElementFactory_ABC >( new ResultsFactory( publisher, context ) ) );
    factory_->Add( std::unique_ptr< ElementFactory_ABC >( new TransformationsFactory() ) );
}

// -----------------------------------------------------------------------------
// Name: AarFacade destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
AarFacade::~AarFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AarFacade::CreateTask
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > AarFacade::CreateTask( xml::xistream& xis, unsigned int firstTick, unsigned int lastTick )
{
    return factory_->CreateTask( xis, firstTick, lastTick );
}
