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
#include "AfterActionFactory.h"
#include "AfterActionFunctions.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionModel constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::AfterActionModel( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
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
    {
        xml::xistringstream xis( asnMsg.information );
        xis >> xml::start( "functions" )
                >> xml::list( *this, &AfterActionModel::ReadFunction )
            >> xml::end();
    }
    functions_.reset( new AfterActionFunctions( *this ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::ReadFunction
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::ReadFunction( const std::string& type, xml::xistream& xis )
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
    Register( name.c_str(), *new AfterActionFactory( type, xis ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgIndicatorResult& asnMsg )
{

}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::CreateFunctionIterator
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
Iterator< const AfterActionFunction& > AfterActionModel::CreateFunctionIterator() const
{
    if( functions_.get() )
        return functions_->CreateIterator();
    return new NullIterator< const AfterActionFunction& >();
}
