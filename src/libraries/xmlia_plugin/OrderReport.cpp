// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "OrderReport.h"
#include "SerializationTools.h"
#include "Publisher_ABC.h"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: OrderReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::OrderReport( const dispatcher::Automat& automat, const kernel::Resolver_ABC< kernel::MissionType >& missions, const ASN1T_MsgAutomatOrder& asn )
{
    // Creation du message a envoyer
}

// -----------------------------------------------------------------------------
// Name: OrderReport destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::~OrderReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderReport::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderReport::Send( Publisher_ABC& publisher ) const
{
    xml::xostream& xos = publisher.CreateReport();
    // Création du contenu du fichier xml a envoyer
}
