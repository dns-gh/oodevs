// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "AarFacade.h"
#include "FunctionFactory.h"
#include "ConstantsFactory.h"
#include "ExtractorsFactory.h"
#include "ReductionsFactory.h"
#include "ResultsFactory.h"
#include "TransformationsFactory.h"

// -----------------------------------------------------------------------------
// Name: AarFacade constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
AarFacade::AarFacade(  dispatcher::ClientPublisher_ABC& publisher, int context )
    : factory_( new FunctionFactory() )
{
    factory_->Add( std::auto_ptr< ElementFactory_ABC >( new ConstantsFactory() ) );
    factory_->Add( std::auto_ptr< ElementFactory_ABC >( new ExtractorsFactory() ) );
    factory_->Add( std::auto_ptr< ElementFactory_ABC >( new ReductionsFactory() ) );
    factory_->Add( std::auto_ptr< ElementFactory_ABC >( new ResultsFactory( publisher, context ) ) );
    factory_->Add( std::auto_ptr< ElementFactory_ABC >( new TransformationsFactory() ) );
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
// Name: boost::shared_ptr< Task > AarFacade::CreateTask
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > AarFacade::CreateTask( xml::xistream& xis )
{
    return factory_->CreateTask( xis );
}
