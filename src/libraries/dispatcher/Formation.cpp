// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Formation.h"
#include "ClientPublisher_ABC.h"
#include "Model.h"
#include "Side.h"
#include "ModelVisitor_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( const Model& model, const ASN1T_MsgFormationCreation& msg )
    : SimpleEntity< kernel::Formation_ABC >( msg.oid, msg.nom )
    , model_ ( model )
    , name_  ( msg.nom )
    , team_  ( model.sides_.Get( msg.oid_camp ) )
    , level_ ( msg.niveau )
    , parent_( msg.m.oid_formation_parentePresent ? &model.formations_.Get( msg.oid_formation_parente ) : 0 )
{
    if( parent_ )
        parent_->formations_.Register( msg.oid, *this );
    else
        team_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // $$$ RDS : completement invalide si la formation parente a déja été detruite !!! 
    if( parent_ )
        parent_->formations_.Remove( GetId() );
    else
        team_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Formation::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::FormationCreation asn;

    asn().oid      = GetId();
    asn().oid_camp = team_.GetId();
    asn().nom      = name_.c_str();
    asn().niveau   = level_;

    if( parent_ )
    {
        asn().m.oid_formation_parentePresent = 1;
        asn().oid_formation_parente = parent_->GetId();
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Formation::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    formations_.Apply( boost::bind( &Formation::Accept, _1, boost::ref( visitor ) ) );
    automats_.Apply( boost::bind( &Automat::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::HierarchyLevel_ABC& Formation::GetLevel() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" ); // $$$$ AGE 2008-06-20: 
}
