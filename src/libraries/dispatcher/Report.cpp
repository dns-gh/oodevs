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
#include "Model.h"
#include "MissionParameter_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
Report::Report( Model& model, const MsgsSimToClient::MsgReport& report )
    : SimpleEntity< >( report.id().id() )
    , id_     ( report.id().id() )
    , emitter_( model.TaskerToId( report.cr() ) )
    , report_ ( report.cr_oid().id() )
    , type_   ( report.type() )
    , date_   ( report.time().data() )
    , model_  ( model )
{
    if( report.has_parametres() )
    {
        parameters_.resize( report.parametres().elem_size() );
        for( int i = 0; i < report.parametres().elem_size(); ++i )
            parameters_[i] = MissionParameter_ABC::Create( report.parametres().elem( i ) );
    }
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
    //!!Warning: cr and id fields have been accidently swapped in this version of protocol!
    client::Report message;
    message().mutable_id()->set_id( id_ );
    message().mutable_cr_oid()->set_id( report_ );
    model_.SetToTasker( *message().mutable_cr(), emitter_ );
    message().set_type( type_ );
    message().mutable_time()->set_data( date_ );
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).Send( *message().mutable_parametres()->add_elem() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Report::SendDestruction
// Created: AGE 2007-10-19
// -----------------------------------------------------------------------------
void Report::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::InvalidateReport message;
    message().mutable_id()->set_id( emitter_ );
    message().mutable_id()->set_id( id_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Report::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Report::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
