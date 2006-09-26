// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2006-09-25 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Humans.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::Humans( const Model& /*model*/, const ASN1T_DotationPersonnel& asnMsg )
    : nRank_                    ( asnMsg.rang )
    , nNbrTotal_                ( asnMsg.nb_total )
    , nNbrOperational_          ( asnMsg.nb_operationnels )
    , nNbrDead_                 ( asnMsg.nb_morts )
    , nNbrWounded_              ( asnMsg.nb_blesses )
    , nNbrMentalDiseased_       ( asnMsg.nb_blesses_mentaux )
    , nNbrNBC_                  ( asnMsg.nb_contamines_nbc )
    , nNbrInLogisticMedical_    ( asnMsg.nb_dans_chaine_sante )
    , nNbrInLogisticMaintenance_( asnMsg.nb_utilises_pour_maintenance )
{
}

// -----------------------------------------------------------------------------
// Name: Humans destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::~Humans()
{

}
        
// -----------------------------------------------------------------------------
// Name: Humans::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Humans::Update( const ASN1T_DotationPersonnel& asnMsg )
{
    nNbrTotal_                = asnMsg.nb_total;
    nNbrOperational_          = asnMsg.nb_operationnels;
    nNbrDead_                 = asnMsg.nb_morts;
    nNbrWounded_              = asnMsg.nb_blesses;
    nNbrMentalDiseased_       = asnMsg.nb_blesses_mentaux;
    nNbrNBC_                  = asnMsg.nb_contamines_nbc;
    nNbrInLogisticMedical_    = asnMsg.nb_dans_chaine_sante;
    nNbrInLogisticMaintenance_= asnMsg.nb_utilises_pour_maintenance;
}
