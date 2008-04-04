// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "AgentExtension.h"
#include "SerializationTools.h"
#include "dispatcher/Agent.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitAttributes& attributes )
{
    if( attributes.m.positionPresent || attributes.m.hauteurPresent )
        UpdateWhere( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::UpdateWhere
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void AgentExtension::UpdateWhere( const ASN1T_MsgUnitAttributes& )
{
    xml::xostringstream xos;
    xos << xml::start( "NewWherePush" )
            << xml::attribute( "xmlns:msdl", "http://netlab.gmu.edu/JBML/MSDL" ) // $$$$ SBO 2008-02-29: maybe not necessary
            << xml::attribute( "xmlns:bml", "http://netlab.gmu.edu/JBML/BML" )  // $$$$ SBO 2008-02-29: maybe not necessary
            << xml::start( "NewWhere" )
                << xml::start( "RelocatedWho" )
                    << xml::content( "bml:OrgName", holder_.GetID() )
                << xml::end()
                << xml::start( "RelocatedWhere" )
                    << xml::content( "bml:WhereLabel", holder_.GetName() )
                    << xml::content( "bml:WhereClass", "POINT" )
                    << xml::start( "bml:WhereValue" )
                        << xml::start( "bml:WhereLocation" )
                            << bml::UtmLocation( holder_.position_.latitude, holder_.position_.longitude, holder_.nHeight_ )
                        << xml::end()
                    << xml::end()
                    << xml::content( "WhereQualifier", "AT" )
                << xml::end()
            << xml::end()
        << xml::end();
    // $$$$ SBO 2008-02-29: publisher_.Push( xos.str() ); or something
}
