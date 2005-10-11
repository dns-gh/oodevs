//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.cpp $
// $Author: Age $
// $Modtime: 4/03/05 13:48 $
// $Revision: 3 $
// $Workfile: DEC_Path_ABC.cpp $
// 
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_Path_ABC.h"

#include "DEC_PathSection_ABC.h"
#include "DEC_Pathfind_Manager.h"
#include "MIL_AgentServer.h"

uint DEC_Path_ABC::nIDIdx_ = 0;

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::DEC_Path_ABC()
    : nID_         ( ++ nIDIdx_ )
    , nNbrRefs_    ( 0 )
    , nNbrDIARefs_ ( 0 )
    , nState_      ( eComputing )            
    , bJobCanceled_( false )
{
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::~DEC_Path_ABC()
{
    assert( nNbrRefs_    == 0 );
    assert( nNbrDIARefs_ == 0 );

    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        delete *itPathSection;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetPathPoints
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
void DEC_Path_ABC::GetPathPoints( T_PointVector& points ) const
{
    points.clear();
    if( pathSections_.empty() )
        return;
    points.push_back( pathSections_.front()->GetPosStart() );
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        points.push_back( (**itPathSection).GetPosEnd() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::Cancel
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path_ABC::Cancel()
{
    bJobCanceled_ = true;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        (**itPathSection).Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
MT_Float DEC_Path_ABC::GetLength() const
{
    MT_Float rLength = 0.;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        rLength += (**itPathSection).GetLength();
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path_ABC::Execute( TerrainPathfinder& pathfind )
{
    uint nComputationEndTime = 0;
    
    const uint nMaxComputationDuration = MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration();
    if( nMaxComputationDuration == std::numeric_limits< uint >::max() )
        nComputationEndTime = std::numeric_limits< uint >::max();
    else
        nComputationEndTime = time( 0 ) + nMaxComputationDuration;

    nState_ = eComputing;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        if( bJobCanceled_ )
        {
            nState_ = eCanceled;
            return;
        }
        DEC_PathSection_ABC& pathSection = **itPathSection;
        if( !pathSection.Execute( pathfind, nComputationEndTime ) )
        {
            if( bJobCanceled_ )
                nState_ = eCanceled;
            else if( itPathSection == pathSections_.begin() && pathSection.IsImpossible() )
                nState_ = eImpossible;
            else
                nState_ = ePartial;
            return;
        }
        NotifySectionEnded();
    }
    nState_ = eValid;
}

// =============================================================================
// DEBUG
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetStateAsString
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
std::string DEC_Path_ABC::GetStateAsString() const
{
    switch( nState_ ) 
    {
        case eInvalid    : return "Invalid";
        case eComputing  : return "Computing";
        case eValid      : return "Valid";
        case eImpossible : return "Impossible";
        case ePartial    : return "Partial";
        case eCanceled   : return "Canceled";
        default:           return "UNKNOWN";
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetPathAsString
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
std::string DEC_Path_ABC::GetPathAsString() const
{
    std::stringstream strTmp;
    strTmp << "   Path points : " << pathSections_.front()->GetPosStart();
    for( CIT_PathSectionVector itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection ) 
        strTmp << " -> " << (**itSection).GetPosEnd();
    return strTmp.str();
}


//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DecDIARef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
void DEC_Path_ABC::DecDIARef()
{
    if( nNbrDIARefs_ != 1 ) //$$$ TMP pour corriger les scripts
    {
        assert( !"DEC_DetruireItineraire appele plusieurs fois de suite sur le meme itineraire" );
        return;
    }
    --nNbrDIARefs_;
    if( nNbrDIARefs_ == 0 && nNbrRefs_ == 0 )
        MIL_AgentServer::GetWorkspace().GetPathFindManager().DeletePath( *this );
}


//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DecRef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
void DEC_Path_ABC::DecRef()
{
    assert( nNbrRefs_ > 0 );
    --nNbrRefs_;
    if( nNbrRefs_ == 0 && nNbrDIARefs_ == 0 )
        MIL_AgentServer::GetWorkspace().GetPathFindManager().DeletePath( *this );
}


