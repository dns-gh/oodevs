// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "CampObjectAttribute.h"

#include "Model.h"
#include "Automat.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CampObjectAttribute::CampObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , model_             ( model )
    , pTC2_              ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CampObjectAttribute::~CampObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void CampObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_prisoner_camp )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.prisoner_camp->tc2 );
    else if( asnMsg.t == T_ObjectAttributesSpecific_refugee_camp )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.refugee_camp->tc2 );
}

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CampObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    assert( pTC2_ );

    asnMsg.t = nType_;
    switch( nType_ )
    {
        case T_ObjectAttributesSpecific_prisoner_camp: 
            asnMsg.u.prisoner_camp = new ASN1T_ObjectAttributesPrisonerCamp();
            asnMsg.u.prisoner_camp->tc2 = pTC2_->GetID(); 
            break;
        case T_ObjectAttributesSpecific_refugee_camp: 
            asnMsg.u.refugee_camp = new ASN1T_ObjectAttributesRefugeeCamp();
            asnMsg.u.refugee_camp->tc2 = pTC2_->GetID(); 
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    }
}

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void CampObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    switch( nType_ )
    {
        case T_ObjectAttributesSpecific_prisoner_camp: 
            delete asnMsg.u.prisoner_camp;
            break;
        case T_ObjectAttributesSpecific_refugee_camp: 
            delete asnMsg.u.refugee_camp;
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    }
}
