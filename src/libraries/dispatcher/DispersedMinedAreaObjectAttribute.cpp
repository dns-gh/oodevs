// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "DispersedMinedAreaObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DispersedMinedAreaObjectAttribute::DispersedMinedAreaObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nMinesActivityTime_( 0 )
    , rMinesDensity_     ( 0. )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DispersedMinedAreaObjectAttribute::~DispersedMinedAreaObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_dispersed_mine_area )
    {
        nMinesActivityTime_ = asnMsg.u.dispersed_mine_area->activity_time;
        rMinesDensity_      = asnMsg.u.dispersed_mine_area->density;
    }
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.dispersed_mine_area = new ASN1T_ObjectAttributesDispersedMineArea();
    
    asnMsg.u.dispersed_mine_area->activity_time = nMinesActivityTime_;
    asnMsg.u.dispersed_mine_area->density       = rMinesDensity_;
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.dispersed_mine_area;
}
