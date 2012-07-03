//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/PathType.cpp $
// $Author: Nld $
// $Modtime: 21/07/05 15:54 $
// $Revision: 11 $
// $Workfile: PathType.cpp $
//
//*****************************************************************************

#include "PathType.h"

using namespace sword::movement;

const PathType PathType::movement_     ( eMovement     , "movement"     );
const PathType PathType::recon_        ( eRecon        , "recon"        );
const PathType PathType::infiltration_ ( eInfiltration , "infiltration" );
const PathType PathType::assault_      ( eAssault      , "assault"      );
const PathType PathType::retreat_      ( eRetreat      , "retreat"      );
const PathType PathType::backup_       ( eBackup       , "backup"       );
const PathType PathType::mineClearance_( eMineClearance, "minesweep"    );
const PathType PathType::logistic_     ( eLogistic     , "logistic"     );
const PathType PathType::criminal_     ( eCriminal     , "criminal"     );
const PathType PathType::nbc_          ( eNBC          , "nbc"          );

PathType::T_PathTypeFromIDVector PathType::pathTypesFromID_;
PathType::T_PathTypeMap          PathType::pathTypes_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PathType::Initialize
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void PathType::Initialize()
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
    pathTypes_[ nbc_          .GetName() ] = &nbc_;

    pathTypesFromID_.assign( pathTypes_.size(), 0 );
    for( CIT_PathTypeMap it = pathTypes_.begin(); it != pathTypes_.end(); ++it )
    {
        assert( pathTypesFromID_.size() > it->second->GetID() );
        pathTypesFromID_[ it->second->GetID() ] = it->second;
    }
}

// =============================================================================
// INSTANCE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PathType constructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
PathType::PathType( E_PathType nType, const std::string& strName )
    : nPathType_( nType   )
    , strName_  ( strName )
{
}


//-----------------------------------------------------------------------------
// Name: PathType destructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
PathType::~PathType()
{
}

