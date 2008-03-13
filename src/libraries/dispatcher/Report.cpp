// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Report.h"
#include "MissionParameter_ABC.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::Report( Model& model, const ASN1T_MsgReport& report )
    : id_     ( report.cr_oid )
    , emitter_( report.oid )
    , report_ ( report.cr )
    , type_   ( report.type )
    , date_   ( (const char*)report.time.data, report.time.numocts )
{
    parameters_.resize( report.parametres.n );
    for( unsigned i = 0; i < report.parametres.n; ++i )
        parameters_[i] = MissionParameter_ABC::Create( model, report.parametres.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: Report destructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::~Report()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Report::SendFullUpdate
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report::SendCreation
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientReport asn;
    asn().cr_oid = id_;
    asn().oid    = emitter_;
    asn().cr     = report_;
    asn().type   = type_;
    asn().time   = date_.c_str();
    asn().parametres.n = parameters_.size();
    asn().parametres.elem = asn().parametres.n ? new ASN1T_MissionParameter[ parameters_.size() ] : 0;

    unsigned i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).Send( asn().parametres.elem[i++] );

    asn.Send( publisher );

    i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).AsnDelete( asn().parametres.elem[i++] );
    delete[] asn().parametres.elem;
}

// -----------------------------------------------------------------------------
// Name: Report::SendDestruction
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientInvalidateReport asn;
    asn().oid    = emitter_;
    asn().cr_oid = id_;
    asn.Send( publisher );
}
