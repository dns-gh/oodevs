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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CampObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CampObjectAttribute::CampObjectAttribute( const Model& model, const ASN1T_MsgObjectCreation& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , model_             ( model )
    , pTC2_              ( 0 )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_prisonniers )
        pTC2_ = &model_.GetAgents().Get( asnMsg.attributs_specifiques.u.camp_prisonniers->tc2 );
    else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_refugies )
        pTC2_ = &model_.GetAgents().Get( asnMsg.attributs_specifiques.u.camp_refugies->tc2 );
        
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
void CampObjectAttribute::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_prisonniers )
        pTC2_ = &model_.GetAgents().Get( asnMsg.attributs_specifiques.u.camp_prisonniers->tc2 );
    else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_camp_refugies )
        pTC2_ = &model_.GetAgents().Get( asnMsg.attributs_specifiques.u.camp_refugies->tc2 );
}
