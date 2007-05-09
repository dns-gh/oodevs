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
CampObjectAttribute::CampObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , model_             ( model )
    , pTC2_              ( 0 )
{
    if( asnMsg.t == T_AttrObjectSpecific_camp_prisonniers )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.camp_prisonniers->tc2 );
    else if( asnMsg.t == T_AttrObjectSpecific_camp_refugies )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.camp_refugies->tc2 );        
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
void CampObjectAttribute::Update( const ASN1T_AttrObjectSpecific& asnMsg )
{
    if( asnMsg.t == T_AttrObjectSpecific_camp_prisonniers )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.camp_prisonniers->tc2 );
    else if( asnMsg.t == T_AttrObjectSpecific_camp_refugies )
        pTC2_ = &model_.GetAutomats().Get( asnMsg.u.camp_refugies->tc2 );
}

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CampObjectAttribute::Send( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    assert( pTC2_ );

    asnMsg.t = nType_;
    switch( nType_ )
    {
        case T_AttrObjectSpecific_camp_prisonniers: 
            asnMsg.u.camp_prisonniers = new ASN1T_AttrObjectCampPrisonniers();
            asnMsg.u.camp_prisonniers->tc2 = pTC2_->GetID(); 
            break;
        case T_AttrObjectSpecific_camp_refugies   : 
            asnMsg.u.camp_refugies = new ASN1T_AttrObjectCampRefugies();
            asnMsg.u.camp_refugies->tc2 = pTC2_->GetID(); 
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    }
}

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void CampObjectAttribute::AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    switch( nType_ )
    {
        case T_AttrObjectSpecific_camp_prisonniers: 
            delete asnMsg.u.camp_prisonniers;
            break;
        case T_AttrObjectSpecific_camp_refugies: 
            delete asnMsg.u.camp_refugies;
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    }
}
