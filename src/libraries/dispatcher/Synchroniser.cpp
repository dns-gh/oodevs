// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Synchroniser.h"
#include "Entity_ABC.h"
#include "ClientPublisher_ABC.h"
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Synchroniser constructor
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
Synchroniser::Synchroniser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchroniser destructor
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
Synchroniser::~Synchroniser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForCreation
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForCreation( Entity_ABC& entity )
{
    toCreate_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForUpdate
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForUpdate( Entity_ABC& entity )
{
    toUpdate_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForSpecialUpdate
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForSpecialUpdate( Entity_ABC& entity )
{
    toSpecial_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::FlagForDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::FlagForDestruction( Entity_ABC& entity )
{
    toDestroy_.push_back( &entity );
}

namespace
{
    struct Publisher : public ClientPublisher_ABC
    {
        Publisher( ClientPublisher_ABC& forward, Model& model )
            : forward_( &forward )
            , model_  ( &model ) {}
        virtual void Send( const ASN1T_MsgsSimToClient& msg )
        {
            forward_->Send( msg );
            model_->Update( msg );
        }
        virtual void Send( const ASN1T_MsgsAuthenticationToClient& ) {}
        virtual void Send( const ASN1T_MsgsReplayToClient& ) {}
        virtual void Send( const ASN1T_MsgsAarToClient& ) {}
        ClientPublisher_ABC* forward_;
        Model* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: Synchroniser::Commit
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Synchroniser::Commit( ClientPublisher_ABC& publisher, Model& model )
{
    for( CIT_Entities it = toCreate_.begin(); it != toCreate_.end(); ++it )
        (*it)->SendCreation( publisher );

    for( CIT_Entities it = toUpdate_.begin(); it != toUpdate_.end(); ++it )
        (*it)->SendFullUpdate( publisher );

    for( CIT_Entities it = toSpecial_.begin(); it != toSpecial_.end(); ++it )
        (*it)->SendSpecialUpdate( publisher );

    Publisher p( publisher, model );
    for( std::vector< Entity_ABC* >::reverse_iterator it = toDestroy_.rbegin(); it != toDestroy_.rend(); ++it )
        (*it)->SendDestruction( p );
}
