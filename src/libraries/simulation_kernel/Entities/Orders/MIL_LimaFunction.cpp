// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LimaFunction.h"

MIL_LimaFunction::T_LimaFunctionMap MIL_LimaFunction::limaFunctions_;

const MIL_LimaFunction MIL_LimaFunction::LD_  ( EnumLimaType::ligne_debouche              , eLimaFuncLD  , "LD"   );
const MIL_LimaFunction MIL_LimaFunction::LCA_ ( EnumLimaType::ligne_changement_attitude   , eLimaFuncLCA , "LCA"  );
const MIL_LimaFunction MIL_LimaFunction::LC_  ( EnumLimaType::ligne_coordination          , eLimaFuncLC  , "LC"   );
const MIL_LimaFunction MIL_LimaFunction::LI_  ( EnumLimaType::ligne_interdire             , eLimaFuncLI  , "LI"   );
const MIL_LimaFunction MIL_LimaFunction::LO_  ( EnumLimaType::ligne_objectif              , eLimaFuncLO  , "LO"   );
const MIL_LimaFunction MIL_LimaFunction::LCAR_( EnumLimaType::ligne_coup_arret            , eLimaFuncLCAR, "LCAR" );
const MIL_LimaFunction MIL_LimaFunction::LR_  ( EnumLimaType::ligne_recueil               , eLimaFuncLR  , "LR"   );
const MIL_LimaFunction MIL_LimaFunction::LDM_ ( EnumLimaType::ligne_debut_mission         , eLimaFuncLDM , "LDM"  );
const MIL_LimaFunction MIL_LimaFunction::LFM_ ( EnumLimaType::ligne_fin_mission           , eLimaFuncLFM , "LFM"  );
const MIL_LimaFunction MIL_LimaFunction::LIA_ ( EnumLimaType::ligne_identification_accueil, eLimaFuncLIA , "LIA"  );

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Initialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_LimaFunction::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing lima functions" );

    limaFunctions_[ LD_  .GetAsnID() ] = &LD_;
    limaFunctions_[ LCA_ .GetAsnID() ] = &LCA_;
    limaFunctions_[ LC_  .GetAsnID() ] = &LC_;
    limaFunctions_[ LI_  .GetAsnID() ] = &LI_;
    limaFunctions_[ LO_  .GetAsnID() ] = &LO_;
    limaFunctions_[ LCAR_.GetAsnID() ] = &LCAR_;
    limaFunctions_[ LR_  .GetAsnID() ] = &LR_;
    limaFunctions_[ LDM_ .GetAsnID() ] = &LDM_;
    limaFunctions_[ LFM_ .GetAsnID() ] = &LFM_;
    limaFunctions_[ LIA_ .GetAsnID() ] = &LIA_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Terminate
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_LimaFunction::Terminate()
{
    for( CIT_LimaFunctionMap it = limaFunctions_.begin(); it != limaFunctions_.end(); ++it )
        delete it->second;
    limaFunctions_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction constructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaFunction::MIL_LimaFunction( const ASN1T_EnumLimaType& asn, uint nID, const std::string& strName )
    : nAsnID_ ( asn )
    , nID_    ( nID )
    , strName_( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction destructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaFunction::~MIL_LimaFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Find
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const MIL_LimaFunction* MIL_LimaFunction::Find( const ASN1T_EnumLimaType& asn )
{
    CIT_LimaFunctionMap it = limaFunctions_.find( asn );
    if( it == limaFunctions_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::Find
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const MIL_LimaFunction* MIL_LimaFunction::Find( uint nID )
{
    for( CIT_LimaFunctionMap it = limaFunctions_.begin(); it != limaFunctions_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::GetID
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
uint MIL_LimaFunction::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::GetAsnID
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
ASN1T_EnumLimaType MIL_LimaFunction::GetAsnID() const
{
    return nAsnID_;
}
