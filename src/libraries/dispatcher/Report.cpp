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
#include "protocol/clientsenders.h"

using namespace dispatcher;
//using namespace MsgsSimToClient;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::Report( Model&, const MsgsSimToClient::MsgReport& report )
    : SimpleEntity< >( report.cr_oid() )
    , id_     ( report.cr_oid() )
    , emitter_( report.oid() )
    , report_ ( report.cr() )
    , type_   ( report.type() )
    , date_   ( report.time().data(), report.time().data().size() )
{
    parameters_.resize( report.parametres().elem_size() );
    for( int i = 0; i < report.parametres().elem_size(); ++i )
        parameters_[i] = MissionParameter_ABC::Create( report.parametres().elem( i ) );
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
    asn().set_cr_oid( id_ );
    asn().set_oid( emitter_ );
    asn().set_cr( report_ );
    asn().set_type( type_ );
    asn().mutable_time()->set_data( date_.c_str() );
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).Send( *asn().mutable_parametres()->add_elem() );

    asn.Send( publisher );

    unsigned int i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).Delete( *asn().mutable_parametres()->mutable_elem(i++) );
    asn().mutable_parametres()->Clear();
}

// -----------------------------------------------------------------------------
// Name: Report::SendDestruction
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::InvalidateReport asn;
    asn().set_oid( emitter_ );
    asn().set_cr_oid( id_ );
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
