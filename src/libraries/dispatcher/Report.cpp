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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::Report( Model&, const ASN1T_MsgReport& report )
    : SimpleEntity< >( report.cr_oid )
    , id_     ( report.cr_oid )
    , emitter_( report.oid )
    , report_ ( report.cr )
    , type_   ( report.type )
    , date_   ( (const char*)report.time.data, report.time.numocts )
{
    parameters_.resize( report.parametres.n );
    for( unsigned i = 0; i < report.parametres.n; ++i )
        parameters_[i] = MissionParameter_ABC::Create( report.parametres.elem[i] );
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
    client::Report asn;
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
    client::InvalidateReport asn;
    asn().oid    = emitter_;
    asn().cr_oid = id_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Report::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Report::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
