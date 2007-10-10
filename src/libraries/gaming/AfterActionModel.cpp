// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionModel.h"
#include "AfterActionFunction.h"
#include "AfterActionRequests.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionModel constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , requests_( new AfterActionRequests( controller, publisher ) )
{
    Load( "functions.xml" ); // $$$$ AGE 2007-10-10: 
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::~AfterActionModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgAarInformation& asnMsg )
{
    // $$$$ AGE 2007-10-10: 
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgIndicatorResult& asnMsg )
{
    requests_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequest& AfterActionModel::CreateRequest( const AfterActionFunction& function )
{
    return requests_->CreateRequest( function );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Load
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::Load( const std::string& functions )
{
    xml::xifstream xis( functions );
    xis >> xml::start( "functions" )
            >> xml::list( "function", *this, &AfterActionModel::ReadFunction )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::ReadFunction
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::ReadFunction( xml::xistream& xis )
{
    std::auto_ptr< AfterActionFunction > function( new AfterActionFunction( xis ) );
    const QString name = function->GetName();
    Register( name, *function.release() );
}
