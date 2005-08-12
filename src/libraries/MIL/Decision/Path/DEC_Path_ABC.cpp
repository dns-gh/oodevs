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

#include "MIL_Pch.h"
#include "DEC_Path_ABC.h"
#include "DEC_PathSection.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Path_ABC::DEC_Path_ABC()
    : pathSections_()
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
    {
        DEC_PathSection& pathSection = **itPathSection;
        pathSection.Cancel();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
MT_Float DEC_Path_ABC::GetLength() const
{
    MT_Float rLength = 0.;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        const DEC_PathSection& pathSection = **itPathSection;
        rLength += pathSection.GetLength();
    }
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path_ABC::Execute( TerrainPathfinder& pathfind )
{
    nState_ = eComputing;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        if( bJobCanceled_ )
        {
            nState_ = eCanceled;
            return;
        }
        DEC_PathSection& pathSection = **itPathSection;
        if( ! pathSection.Execute( pathfind ) )
        {
            if( bJobCanceled_ )
                nState_ = eCanceled;
            else if( itPathSection == pathSections_.begin() && pathSection.IsImpossible() )
            {
                nState_ = eImpossible;
//                assert( false );
            }
            else
                nState_ = ePartial;
            return;
        }
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

