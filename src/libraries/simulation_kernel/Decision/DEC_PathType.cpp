//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.cpp $
// $Author: Nld $
// $Modtime: 21/07/05 15:54 $
// $Revision: 11 $
// $Workfile: DEC_PathType.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_PathType.h"

const DEC_PathType DEC_PathType::movement_     ( eMovement     , "movement"     );
const DEC_PathType DEC_PathType::recon_        ( eRecon        , "recon"        );
const DEC_PathType DEC_PathType::infiltration_ ( eInfiltration , "infiltration" );
const DEC_PathType DEC_PathType::assault_      ( eAssault      , "assault"      );
const DEC_PathType DEC_PathType::retreat_      ( eRetreat      , "retreat"      );
const DEC_PathType DEC_PathType::backup_       ( eBackup       , "backup"       );
const DEC_PathType DEC_PathType::mineClearance_( eMineClearance, "minesweep"    );
const DEC_PathType DEC_PathType::logistic_     ( eLogistic     , "logistic"     );
const DEC_PathType DEC_PathType::criminal_     ( eCriminal     , "criminal"     );



DEC_PathType::T_PathTypeFromIDVector DEC_PathType::pathTypesFromID_;
DEC_PathType::T_PathTypeMap          DEC_PathType::pathTypes_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathType::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_PathType::Initialize()
{
    pathTypes_[ movement_     .GetName() ] = &movement_;
    pathTypes_[ recon_        .GetName() ] = &recon_;
    pathTypes_[ infiltration_ .GetName() ] = &infiltration_;
    pathTypes_[ assault_      .GetName() ] = &assault_;
    pathTypes_[ retreat_      .GetName() ] = &retreat_;
    pathTypes_[ backup_       .GetName() ] = &backup_;
    pathTypes_[ mineClearance_.GetName() ] = &mineClearance_;
    pathTypes_[ logistic_     .GetName() ] = &logistic_;
    pathTypes_[ criminal_     .GetName() ] = &criminal_;

    pathTypesFromID_.assign( pathTypes_.size(), 0 );
    for( CIT_PathTypeMap it = pathTypes_.begin(); it != pathTypes_.end(); ++it )
    {
        assert( pathTypesFromID_.size() > it->second->GetID() );
        pathTypesFromID_[ it->second->GetID() ] = it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::Terminate
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_PathType::Terminate()
{

}

// =============================================================================
// INSTANCE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_PathType constructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::DEC_PathType( E_PathType nType, const std::string& strName )
    : nPathType_( nType   )
    , strName_  ( strName )
{
}


//-----------------------------------------------------------------------------
// Name: DEC_PathType destructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::~DEC_PathType()
{
}

