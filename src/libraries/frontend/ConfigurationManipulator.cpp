// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ConfigurationManipulator.h"
#include <xeumeuleu/xml.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator constructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
ConfigurationManipulator::ConfigurationManipulator( const std::string& filename, const std::string& rootNode )
    : document_( new XmlNode( filename, rootNode ) )
    , output_  ( new xml::xofstream( filename ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator destructor
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
ConfigurationManipulator::~ConfigurationManipulator()
{
    document_->Serialize( *output_ );
}
