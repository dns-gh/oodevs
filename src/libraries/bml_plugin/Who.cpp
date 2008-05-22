// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Who.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent& entity )
    : agent_( &entity )
    , automat_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Automat& entity )
    : agent_( 0 )
    , automat_( &entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::~Who()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who::operator<<
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
xml::xostream& bml::operator<<( xml::xostream& xos, const Who& who )
{
    xos << xml::start( "C_BML_Who" )
            << xml::start( "WhoRef" )
                << xml::start( "UnitRef" )
                    << xml::content( "jc3iedm:OID", who.agent_ ? who.agent_->strName_ : who.automat_->strName_ )
                << xml::end()
            << xml::end()
        << xml::end();
    return xos;
}
