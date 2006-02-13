// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "FireResult.h"

#include "App.h"
#include "AgentManager.h"
#include "Agent_ABC.h"
#include "Agent.h"
#include "Population.h"
#include "TypeComposante.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: FireResult constructor
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
FireResult::FireResult( const ASN1T_FireDamagesPion& asnMsg )
    : pTarget_    ( & App::GetApp().GetModel().GetAgent( asnMsg.cible ) )
    , equipments_ ()
    , humans_     ()
    , nDead_      ( 0 )
{
    for( uint i = 0; i < asnMsg.equipements.n; ++i )
    {
        const TypeComposante* pType = App::GetApp().GetAgentManager().FindTypeComposante( asnMsg.equipements.elem[ i ].type_equipement );
        assert( pType );
        std::stringstream ss;
        ss  << pType->GetName() << " - [" 
            << asnMsg.equipements.elem[ i ].nb_disponibles   << " "
            << asnMsg.equipements.elem[ i ].nb_indisponibles << " "
            << asnMsg.equipements.elem[ i ].nb_reparables    << "]";
        equipments_.push_back( ss.str() );
    }

    for( uint i = 0; i < asnMsg.humains.n; ++i )
    {
        T_FireResultHuman& result = *new T_FireResultHuman();
        result.reserve( 6 );
        result[ eHumanWound_BlesseUrgence1 ]       = asnMsg.humains.elem[ i ].nb_blesses_urgence_1;
        result[ eHumanWound_BlesseUrgence2 ]       = asnMsg.humains.elem[ i ].nb_blesses_urgence_2;
        result[ eHumanWound_BlesseUrgence3 ]       = asnMsg.humains.elem[ i ].nb_blesses_urgence_3;
        result[ eHumanWound_BlesseUrgenceExtreme ] = asnMsg.humains.elem[ i ].nb_blesses_urgence_extreme;
        result[ eHumanWound_Mort ]                 = asnMsg.humains.elem[ i ].nb_morts;
        result[ eHumanWound_NonBlesse ]            = asnMsg.humains.elem[ i ].nb_non_blesses;
        humans_.insert( std::make_pair( asnMsg.humains.elem[ i ].rang, &result ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FireResult constructor
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
FireResult::FireResult( const ASN1T_FireDamagesPopulation& asnMsg )
    : pTarget_    ( & App::GetApp().GetModel().GetPopulation( asnMsg.cible ) )
    , equipments_ ()
    , humans_     ()
    , nDead_      ( asnMsg.nb_morts )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireResult destructor
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
FireResult::~FireResult()
{
    equipments_.clear();
    for( CIT_FireResultHumans it = humans_.begin(); it != humans_.end(); ++it )
        delete it->second;
    humans_.clear();
}
