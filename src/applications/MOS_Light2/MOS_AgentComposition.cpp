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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentComposition.cpp $
// $Author: Age $
// $Modtime: 6/04/05 11:57 $
// $Revision: 9 $
// $Workfile: MOS_AgentComposition.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_AgentComposition.h"

#include "MOS_App.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentComposition.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_AgentComposition constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentComposition::MOS_AgentComposition()
    : nNbrHeliportedTroops_    ( (uint)-1 )
    , bEmptyGasTank_           ( true )
{
    for( int n = 0; n < eTroopHealthStateNbrStates; ++n )
    {
        officiers_[n] = 0;
        sousOfficiers_[n] = 0;
        mdr_[n] = 0;
    }

    for( int i = 0; i < 3; ++i )
        for( int j = 0; j < (int)eTroopHealthStateNbrStates; ++j )
            transportedTroops_[i][j] = 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentComposition destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentComposition::~MOS_AgentComposition()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentComposition::SetComposition
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_AgentComposition::SetComposition( const ASN1T_MsgUnitDotations& asnMsg )
{
    SetTroops        ( asnMsg );
    SetEquipment     ( asnMsg );
    SetResources     ( asnMsg );
    SetEquipmentLends( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentComposition::SetEquipmentLends
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_AgentComposition::SetEquipmentLends( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( ! asnMsg.m.prets_equipementPresent )
        return;

    lends_.resize( asnMsg.prets_equipement.n );
    for( unsigned int i = 0; i < asnMsg.prets_equipement.n; ++i )
    {
        lends_[i].nBorrowerId_ = asnMsg.prets_equipement.elem[i].oid_pion_emprunteur;
        lends_[i].nEquipment_  = asnMsg.prets_equipement.elem[i].type_equipement;
        lends_[i].nQuantity_   = asnMsg.prets_equipement.elem[i].nombre;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentComposition::SetTroops
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_AgentComposition::SetTroops( const ASN1T_MsgUnitDotations& asnMsg )
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
// Name: MOS_AgentComposition::SetEquipment
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_AgentComposition::SetEquipment( const ASN1T_MsgUnitDotations& asnMsg  )
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
// Name: MOS_AgentComposition::SetResources
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_AgentComposition::SetResources( const ASN1T_MsgUnitDotations& asnMsg )
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
        if(    MOS_App::GetApp().GetResourceName( equipId ) == "essence"
            || MOS_App::GetApp().GetResourceName( equipId ) == "gasoil"
            || MOS_App::GetApp().GetResourceName( equipId ) == "kerosene" )    //$$$ It should be the server's job to tell us that the unit is out of gas.
            bEmptyGasTank_ = (qty == 0);
    }
}
