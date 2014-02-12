// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PropagationAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PropagationAttribute constructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::PropagationAttribute( const sword::ObjectAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute destructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::~PropagationAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Update
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Update( const sword::ObjectAttributes& message )
{
    if( message.has_propagation() )
    {
        model_ = message.propagation().model();
        if( message.propagation().has_date() )
            date_ = message.propagation().date();
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Send
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Send( sword::ObjectAttributes& message ) const
{
    message.mutable_propagation()->set_model( model_ );
    if( date_ != "" )
        message.mutable_propagation()->set_date( date_ );

}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::GetModel
// Created: AHC 2013-07-09
// -----------------------------------------------------------------------------
const std::string& PropagationAttribute::GetModel() const
{
    return model_;
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::GetDate
// Created: AHC 2013-07-09
// -----------------------------------------------------------------------------
const std::string& PropagationAttribute::GetDate() const
{
    return date_;
}
