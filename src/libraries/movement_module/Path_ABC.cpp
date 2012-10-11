//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/Path_ABC.cpp $
// $Author: Age $
// $Modtime: 4/03/05 13:48 $
// $Revision: 3 $
// $Workfile: Path_ABC.cpp $
//
//*****************************************************************************

#include "Path_ABC.h"
#include "PathPoint.h"
#include "PathResult.h"
#include "PathSection_ABC.h"
#include "wrapper/Hook.h"
#include <sstream>
#include <limits>
#include <ctime>

using namespace sword::movement;

DECLARE_HOOK( GetMaxPathFindComputationDuration, unsigned int, () )
DECLARE_HOOK( CancelPathFindJob, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )

unsigned int Path_ABC::nIDIdx_ = 0;

// -----------------------------------------------------------------------------
// Name: Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
Path_ABC::Path_ABC()
    : nID_         ( ++ nIDIdx_ )
    , nNbrRefs_    ( 0 )
    , nState_      ( eComputing )
    , bJobCanceled_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
Path_ABC::~Path_ABC()
{
    CleanAfterComputation();
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::AddRef
// Created: LDC 2012-05-15
// -----------------------------------------------------------------------------
void Path_ABC::AddRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::DecRef
// Created: LDC 2012-05-15
// -----------------------------------------------------------------------------
void Path_ABC::DecRef()
{
    if( --nNbrRefs_ <= 0 )
        GET_HOOK( CancelPathFindJob )( shared_from_this() );
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::CleanAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void Path_ABC::CleanAfterComputation()
{
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        delete *itPathSection;
    pathSections_.clear();
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::Cancel
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void Path_ABC::Cancel()
{
    bJobCanceled_ = true;
    nState_ = eCanceled;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        ( *itPathSection )->Cancel();
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double Path_ABC::GetLength() const
{
    double rLength = 0.;
    for( CIT_PathSectionVector itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        rLength += ( *itPathSection )->GetLength();
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::DoExecute
// Created: AGE 2006-03-07
// -----------------------------------------------------------------------------
void Path_ABC::DoExecute( TerrainPathfinder& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = GET_HOOK( GetMaxPathFindComputationDuration )();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    if( pathSections_.empty() )
        throw std::runtime_error( "List of path sections is empty" );
    const PathResult::T_PathPointList& pathPoints = dynamic_cast< const PathResult* >( &pathSections_.front()->GetPath() )->GetResult( false );

    lastWaypoint_ = pathSections_.back()->GetPosEnd();
    computedWaypoints_.clear();
    nState_ = eComputing;
    for( T_PathSectionVector::iterator itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
    {
        if( bJobCanceled_ )
        {
            nState_ = eCanceled;
            return;
        }
        PathSection_ABC& pathSection = **itPathSection;
        if( !pathSection.Execute( pathfind, nComputationEndTime ) )
        {
            if( !pathPoints.empty() )
                computedWaypoints_.push_back( pathPoints.back()->GetPos() );

            if( bJobCanceled_ )
            {
                nState_ = eCanceled;
                return;
            }
            else if( itPathSection == pathSections_.begin() && pathSection.IsImpossible() )
            {
                nState_ = eImpossible;
                return;
            }
            else
            {
                T_PathSectionVector::iterator itNextPathSection = itPathSection + 1;
                if( itNextPathSection == pathSections_.end() )
                {
                    nState_ = ePartial;
                    return;
                }
                else
                    (*itNextPathSection)->SetPosStart( pathPoints.back()->GetPos() );
            }
        }
        else if( !pathPoints.empty() )
            computedWaypoints_.push_back( pathPoints.back()->GetPos() );
        NotifySectionEnded();
    }
    nState_ = eValid;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::Execute
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void Path_ABC::Execute( TerrainPathfinder& pathfind )
{
    try
    {
        DoExecute( pathfind );
    }
    catch( std::runtime_error& )
    {
        bJobCanceled_ = true;
        nState_ = eCanceled;
    }
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetStateAsString
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
std::string Path_ABC::GetStateAsString() const
{
    switch( nState_ )
    {
        case eInvalid    : return "Invalid";
        case eComputing  : return "Computing";
        case eValid      : return "Valid";
        case eImpossible : return "Impossible";
        case ePartial    : return "Partial";
        case eCanceled   : return "Canceled";
        default          : return "UNKNOWN";
    }
}

namespace
{
    std::ostream& operator<<( std::ostream& out, const MT_Vector2D& vect )
    {
        return out << '[' << vect.rX_ << ", " << vect.rY_ << ']';
    }
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetPathAsString
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
std::string Path_ABC::GetPathAsString() const
{
    std::stringstream strTmp;
    strTmp << "   Path points : " << pathSections_.front()->GetPosStart();
    for( CIT_PathSectionVector itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection )
        strTmp << " -> " << ( *itSection )->GetPosEnd();
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::RemoveComputedWaypoint
// Created: CMA 2012-04-26
// -----------------------------------------------------------------------------
void Path_ABC::RemoveComputedWaypoint()
{
    if( !computedWaypoints_.empty() )
        computedWaypoints_.erase( computedWaypoints_.begin() );
}

//-----------------------------------------------------------------------------
// Name: Path_ABC::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
Path_ABC::E_State Path_ABC::GetState() const
{   
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetComputedWaypoints
// Created: CMA 2012-02-23
// -----------------------------------------------------------------------------
const T_PointVector& Path_ABC::GetComputedWaypoints() const
{
    return computedWaypoints_;
}
// -----------------------------------------------------------------------------
// Name: Path_ABC::GetID
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
unsigned int Path_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetLastWaypoint
// Created: CMA 2012-05-21
// -----------------------------------------------------------------------------
const MT_Vector2D& Path_ABC::GetLastWaypoint() const
{
    return lastWaypoint_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void Path_ABC::RegisterPathSection( PathSection_ABC& section )
{
    pathSections_.push_back( &section );
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool Path_ABC::operator==( const Path_ABC& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool Path_ABC::operator!=( const Path_ABC& rhs ) const
{
    return nID_ != rhs.nID_;
}
