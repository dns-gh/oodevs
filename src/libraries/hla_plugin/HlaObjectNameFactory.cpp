// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaObjectNameFactory.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaObjectNameFactory constructor
// Created: AHC 2012-03-23
// -----------------------------------------------------------------------------
HlaObjectNameFactory::HlaObjectNameFactory( const std::string& federateName, const std::string& federateID )
    : federateName_( federateName )
    , federateID_( federateID )
{
}

// -----------------------------------------------------------------------------
// Name: HlaObjectNameFactory destructor
// Created: AHC 2012-03-23
// -----------------------------------------------------------------------------
HlaObjectNameFactory::~HlaObjectNameFactory()
{

}

// -----------------------------------------------------------------------------
// Name: HlaObjectNameFactory destructor
// Created: AHC 2012-03-23
// -----------------------------------------------------------------------------
std::string HlaObjectNameFactory::CreateName( const std::string& hint ) const
{
    return federateName_+"_"+federateID_+"_"+hint;
}
