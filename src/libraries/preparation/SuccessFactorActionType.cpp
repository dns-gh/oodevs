// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorActionType.h"
#include "SuccessFactorActionTypeParameter.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    QString ReadDescription( xml::xistream& xis )
    {
        std::string desc;
        xis >> xml::optional >> xml::start( "description" )
                >> desc
            >> xml::end;
        return desc.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionType::SuccessFactorActionType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ).c_str() )
    , function_( xis.attribute< std::string >( "function" ) )
    , description_( ReadDescription( xis ) )
{
    xis >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &SuccessFactorActionType::ReadParameter )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionType::~SuccessFactorActionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType::ReadParameter
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionType::ReadParameter( xml::xistream& xis )
{
    std::unique_ptr< SuccessFactorActionTypeParameter > parameter( new SuccessFactorActionTypeParameter( xis ) );
    Register( static_cast< unsigned long >( elements_.size() ), *parameter.release() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType::GetName
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType::GetFunction
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
std::string SuccessFactorActionType::GetFunction() const
{
    return function_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionType::GetDescription
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QString SuccessFactorActionType::GetDescription() const
{
    return description_;
}
