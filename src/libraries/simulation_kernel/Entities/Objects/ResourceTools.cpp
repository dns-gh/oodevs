// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ResourceTools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "boost/lexical_cast.hpp"

// -----------------------------------------------------------------------------
// Name: ResourceTools constructor
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
ResourceTools::ResourceTools()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceTools destructor
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
ResourceTools::~ResourceTools()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::GetResourceName
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
const std::string& ResourceTools::GetResourceName( unsigned long id ) const
{
    const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( id );
    if( !category )
        throw std::runtime_error( "Bad resource Id :" + boost::lexical_cast< std::string >( id ) );
    return category->GetName();
}

// -----------------------------------------------------------------------------
// Name: ResourceTools::GetResourceId
// Created: JSR 2010-09-03
// -----------------------------------------------------------------------------
unsigned long ResourceTools::GetResourceId( const std::string& name ) const
{
    const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( name );
    if( !category )
        throw std::runtime_error( "Bad resource name :" + name );
    return category->GetMosID();
}
