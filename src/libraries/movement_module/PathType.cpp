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
    for( auto it = pathTypes_.begin(); it != pathTypes_.end(); ++it )
    {
        assert( pathTypesFromID_.size() > it->second->GetID() );
        pathTypesFromID_[ it->second->GetID() ] = it->second;
    }
}

//-----------------------------------------------------------------------------
// Name: PathType constructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
PathType::PathType( E_PathType nType, const std::string& strName )
    : nPathType_( nType )
    , strName_  ( strName )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PathType destructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
PathType::~PathType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const PathType* PathType::Find( const std::string& strName )
{
    auto it = pathTypes_.find ( strName );
    if( it == pathTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PathType::Find
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const PathType* PathType::Find( unsigned int nID )
{
    assert( pathTypesFromID_.size() > nID );
    return pathTypesFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: PathType::GetID
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
unsigned int PathType::GetID() const
{
    return nPathType_;
}

// -----------------------------------------------------------------------------
// Name: PathType::GetName
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const std::string& PathType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PathType::GetPathTypes
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
PathType::T_PathTypeMap& PathType::GetPathTypes()
{
    return pathTypes_;
}
