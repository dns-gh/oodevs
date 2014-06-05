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
#include "IndicatorRequests.h"
#include "clients_kernel/Controller.h"
#include "tools/FileWrapper.h"
#include <xeumeuleu/xml.hpp>
#include "protocol/AarSenders.h"

// -----------------------------------------------------------------------------
// Name: AfterActionModel constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , requests_( new IndicatorRequests( controller, publisher ) )
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
void AfterActionModel::Update( const sword::AarInformation& /*message*/ )
{
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const sword::PlotResult& message )
{
    requests_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest& AfterActionModel::CreateRequest( const AfterActionFunction& function, const QString& name )
{
    return requests_->CreateRequest( function, name );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::FindDefinition
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
const IndicatorDefinition_ABC* AfterActionModel::FindDefinition( const std::string& definition ) const
{
    return Find( definition.c_str() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::CreateIterator
// Created: JSR 2013-10-23
// -----------------------------------------------------------------------------
tools::Iterator< const AfterActionFunction& > AfterActionModel::CreateIterator() const
{
    return tools::Resolver< AfterActionFunction, QString >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Load
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::Load( const tools::Path& functions )
{
    tools::Xifstream xis( functions );
    xis >> xml::start( "functions" )
        >> xml::list( "function", *this, &AfterActionModel::ReadFunction )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::ReadFunction
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::ReadFunction( xml::xistream& xis )
{
    std::unique_ptr< AfterActionFunction > function( new AfterActionFunction( xis ) );
    Register( function->GetName(), *function.release() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::SaveRequests
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void AfterActionModel::SaveRequests( const tools::Path& path ) const
{
    requests_->Save( path );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::LoadRequests
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void AfterActionModel::LoadRequests( const tools::Loader_ABC& loader, const tools::Path& path )
{
    requests_->Load( loader, path, *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Purge
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void AfterActionModel::Purge()
{
    requests_->Purge();
}
