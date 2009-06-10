// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaImport_plugin_pch.h"
#include "Publisher.h"
//#include "soapDCSOperationsSoapBindingProxy.h"
//#include "DCSOperationsSoapBinding.nsmap"
#include "ResponseHandler_ABC.h"
//#include "SerializationTools.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace plugins::xmliaImport;

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::Publisher( xml::xistream& xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher destructor
// Created: SBO 2008-04-02
// -----------------------------------------------------------------------------
Publisher::~Publisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher::PullOrder
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Publisher::PullOrder( const std::string& message, ResponseHandler_ABC& handler )
{
    //Récupération des ordres veant    
}