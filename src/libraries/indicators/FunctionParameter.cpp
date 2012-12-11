// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "FunctionParameter.h"
#include "clients_kernel/Tools.h"

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: FunctionParameter constructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
FunctionParameter::FunctionParameter( const QString& name, const std::string& attribute, boost::shared_ptr< DataType_ABC > type )
    : Element_ABC( name.toStdString() )
    , name_( name )
    , attribute_( attribute )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FunctionParameter destructor
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
FunctionParameter::~FunctionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FunctionParameter::GetValue
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
std::string FunctionParameter::GetValue() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: FunctionParameter::GetType
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
const DataType_ABC& FunctionParameter::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: FunctionParameter::AddParameter
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void FunctionParameter::AddParameter( boost::shared_ptr< Element_ABC > element )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: FunctionParameter::Serialize
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void FunctionParameter::Serialize( xml::xostream& /*xos*/, ElementDeclarator_ABC& /*declarator*/ ) const
{
    throw MASA_EXCEPTION( tools::translate( "Indicators", "Missing parameter: %1." ).arg( name_ ).toStdString().c_str() );
}
