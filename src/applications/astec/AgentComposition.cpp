// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentComposition.cpp $
// $Author: Age $
// $Modtime: 6/04/05 11:57 $
// $Revision: 9 $
// $Workfile: AgentComposition.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentComposition.h"

#include "App.h"
#include "DotationType.h"

// -----------------------------------------------------------------------------
// Name: AgentComposition constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentComposition::AgentComposition()
    : nNbrHeliportedTroops_    ( (uint)-1 )
    , bEmptyGasTank_           ( false )
{
    for( int n = 0; n < eTroopHealthStateNbrStates; ++n )
    {
        officiers_[n] = 0;
        sousOfficiers_[n] = 0;
        mdr_[n] = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: AgentComposition destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentComposition::~AgentComposition()
{
}


// -----------------------------------------------------------------------------
// Name: AgentComposition::SetComposition
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void AgentComposition::SetComposition( const ASN1T_MsgUnitDotations& asnMsg )
{
    SetTroops        ( asnMsg );
    SetEquipment     ( asnMsg );
    SetResources     ( asnMsg );
    SetEquipmentLends( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::SetEquipmentLends
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void AgentComposition::SetEquipmentLends( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.equipements_pretesPresent )
    {
        lendings_.resize( asnMsg.equipements_pretes.n );
        for( unsigned int i = 0; i < asnMsg.equipements_pretes.n; ++i )
        {
            lendings_[i].nBorrowerId_ = asnMsg.equipements_pretes.elem[i].oid_pion_emprunteur;
            lendings_[i].nEquipment_  = asnMsg.equipements_pretes.elem[i].type_equipement;
            lendings_[i].nQuantity_   = asnMsg.equipements_pretes.elem[i].nombre;
        }
    }

    if( asnMsg.m.equipements_empruntesPresent )
    {
        borrowings_.resize( asnMsg.equipements_empruntes.n );
        for( unsigned int i = 0; i < asnMsg.equipements_empruntes.n; ++i )
        {
            borrowings_[i].nLenderId_  = asnMsg.equipements_empruntes.elem[i].oid_pion_preteur;
            borrowings_[i].nEquipment_ = asnMsg.equipements_empruntes.elem[i].type_equipement;
            borrowings_[i].nQuantity_  = asnMsg.equipements_empruntes.elem[i].nombre;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentComposition::SetTroops
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void AgentComposition::SetTroops( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.dotation_eff_personnelPresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_personnel.n;
    while( nSize > 0 )
    {
        ASN1T_DotationPersonnel& dot = asnMsg.dotation_eff_personnel.elem[ --nSize ];

        uint* pTable = mdr_;
        if( dot.rang == EnumHumanRank::officier )
            pTable = officiers_;
        else if( dot.rang == EnumHumanRank::sous_officer )
            pTable = sousOfficiers_;

        pTable[ eTroopHealthStateTotal ] = dot.nb_total;
        pTable[ eTroopHealthStateOperational ] = dot.nb_operationnels;
        pTable[ eTroopHealthStateDead ] = dot.nb_morts;
        pTable[ eTroopHealthStateWounded ] = dot.nb_blesses;
        pTable[ eTroopHealthStateMentalWounds ] = dot.nb_blesses_mentaux;
        pTable[ eTroopHealthStateContaminated ] = dot.nb_contamines_nbc;
        pTable[ eTroopHealthStateInTreatment ] = dot.nb_dans_chaine_sante;
        pTable[ eTroopHealthStateUsedForMaintenance ] = dot.nb_utilises_pour_maintenance;
    }
}


// -----------------------------------------------------------------------------
// Name: AgentComposition::SetEquipment
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void AgentComposition::SetEquipment( const ASN1T_MsgUnitDotations& asnMsg  )
{
    if ( asnMsg.m.dotation_eff_materielPresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_materiel.n;
    while ( nSize > 0 )
    {
        ASN1T_DotationEquipement& value = asnMsg.dotation_eff_materiel.elem[ --nSize ];
        MIL_AgentID equipId = value.type_equipement;
        equipment_[ equipId ].nNbrAvailable_       = value.nb_disponibles;
        equipment_[ equipId ].nNbrUnavailable_     = value.nb_indisponibles;
        equipment_[ equipId ].nNbrReparable_       = value.nb_reparables;
        equipment_[ equipId ].nNbrInMaintenance_   = value.nb_dans_chaine_maintenance;
    }
}


// -----------------------------------------------------------------------------
// Name: AgentComposition::SetResources
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void AgentComposition::SetResources( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.dotation_eff_ressourcePresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_ressource.n;
    while( nSize > 0 )
    {
        ASN1T_DotationRessource& value = asnMsg.dotation_eff_ressource.elem[ --nSize ];
        MIL_AgentID equipId = value.ressource_id;
        uint qty = value.quantite_disponible;
        resources_[ equipId ] = qty;
        if( App::GetApp().GetDotationType( equipId ).GetCategory() == "carburant" )
            bEmptyGasTank_ = ( qty == 0  );
    }
}
