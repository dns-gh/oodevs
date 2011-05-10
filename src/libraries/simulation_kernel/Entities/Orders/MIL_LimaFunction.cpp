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
#include "MT_Tools/MT_Logger.h"

MIL_LimaFunction::T_LimaFunctionMap MIL_LimaFunction::limaFunctions_;

const MIL_LimaFunction MIL_LimaFunction::LD_  ( sword::PhaseLineOrder::line_of_departure              , eLimaFuncLD  , "LD"   );
const MIL_LimaFunction MIL_LimaFunction::LCA_ ( sword::PhaseLineOrder::attitude_change_line   , eLimaFuncLCA , "LCA"  );
const MIL_LimaFunction MIL_LimaFunction::LC_  ( sword::PhaseLineOrder::coordination_line          , eLimaFuncLC  , "LC"   );
const MIL_LimaFunction MIL_LimaFunction::LI_  ( sword::PhaseLineOrder::denial_line             , eLimaFuncLI  , "LI"   );
const MIL_LimaFunction MIL_LimaFunction::LO_  ( sword::PhaseLineOrder::objective_line              , eLimaFuncLO  , "LO"   );
const MIL_LimaFunction MIL_LimaFunction::LCAR_( sword::PhaseLineOrder::blocking_line            , eLimaFuncLCAR, "LCAR" );
const MIL_LimaFunction MIL_LimaFunction::LR_  ( sword::PhaseLineOrder::handover_line               , eLimaFuncLR  , "LR"   );
const MIL_LimaFunction MIL_LimaFunction::LDM_ ( sword::PhaseLineOrder::start_of_mission_line         , eLimaFuncLDM , "LDM"  );
const MIL_LimaFunction MIL_LimaFunction::LFM_ ( sword::PhaseLineOrder::end_of_mission_line           , eLimaFuncLFM , "LFM"  );
const MIL_LimaFunction MIL_LimaFunction::LIA_ ( sword::PhaseLineOrder::recognition_and_reception_line, eLimaFuncLIA , "LIA"  );

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
    limaFunctions_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction constructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaFunction::MIL_LimaFunction( const sword::PhaseLineOrder::Function& asn, unsigned int nID, const std::string& strName )
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
const MIL_LimaFunction* MIL_LimaFunction::Find( const sword::PhaseLineOrder::Function& asn )
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
const MIL_LimaFunction* MIL_LimaFunction::Find( unsigned int nID )
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
unsigned int MIL_LimaFunction::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunction::GetAsnID
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
sword::PhaseLineOrder::Function MIL_LimaFunction::GetAsnID() const
{
    return nAsnID_;
}
