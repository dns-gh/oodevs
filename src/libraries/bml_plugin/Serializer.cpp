// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Serializer.h"
#include "SerializationTools.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: Serializer constructor
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
Serializer::Serializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Serializer destructor
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
Serializer::~Serializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Serializer::Update
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::Update( const ASN1T_MsgControlBeginTick& message )
{
    currentDate_ = std::string( (const char*)message.date_time.data, 15 );
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeReportingData
// Created: SBO 2008-05-21
// -----------------------------------------------------------------------------
void Serializer::SerializeReportingData( xml::xostream& xos, const dispatcher::Agent& entity ) const
{
    xos << xml::start( "jc3iedm:ReportingData" )
            << xml::content( "jc3iedm:CategoryCode", "ASS" ) //!< report is assumed to be trustable
            << xml::content( "jc3iedm:ReportingDatetime", currentDate_ ) //!< time of report, compact ISO8601
            << xml::start( "jc3iedm:ReportingOrganisationRef" )
                << xml::content( "jc3iedm:OID", entity.GetAutomat().side_.GetID() ) //!< reported by army
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeOrder
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::SerializeOrder( xml::xostream& xos, const ASN1T_MsgUnitOrder& message, const dispatcher::Agent& entity ) const
{
    SerializeTask( xos, message, entity );
    xos << xml::content( "OrderIssuedWhen", currentDate_ ) // $$$$ SBO 2008-05-16: not a C_BML_When...
        << xml::start( "TaskerWho" ) // $$$$ SBO 2008-05-16: not a C_BML_Who...
            << xml::start( "Unit" )
                << xml::content( "jc3iedm:OID", entity.GetID() )
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeTask
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::SerializeTask( xml::xostream& xos, const ASN1T_MsgUnitOrder& message, const dispatcher::Agent& entity ) const
{
    xos << xml::start( "Task" )
            << xml::start( "GroundTask" )
                << xml::start( "TaskeeWho" )
                    << bml::Who( entity )
                << xml::end();
    SerializeAction( xos, message );
    SerializeControlMeasures( xos, message );
    xos     << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeAction
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::SerializeAction( xml::xostream& xos, const ASN1T_MsgUnitOrder& message ) const
{
    xos << xml::start( "What" )
            << xml::start( "C_BML_WhatWhen" )
                << xml::start( "WhatWhenRef" )
                    << xml::start( "ActionRef" )
                        << xml::content( "jc3iedm:OID", message.mission )
                    << xml::end()
                << xml::end()
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeControlMeasures
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::SerializeControlMeasures( xml::xostream& xos, const ASN1T_MsgUnitOrder& message ) const
{
    if( message.parametres.n == 0 )
        return;
    xos << xml::start( "TaskControlMeasures" );
    for( unsigned i = 0; i < message.parametres.n; ++i )
        SerializeControlMeasure( xos, message.parametres.elem[i] );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Serializer::SerializeControlMeasure
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void Serializer::SerializeControlMeasure( xml::xostream& xos, const ASN1T_MissionParameter& message ) const
{
    
}

