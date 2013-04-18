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
#include <tools/Exception.h>
#include "wrapper/Hook.h"
#include <sstream>
#include <limits>
#include <ctime>

using namespace sword::movement;

DECLARE_HOOK( GetMaxPathFindComputationDuration, unsigned int, () )

// -----------------------------------------------------------------------------
// Name: Path_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
Path_ABC::Path_ABC( std::size_t identifier )
    : identifier_  ( identifier )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::Cancel
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void Path_ABC::Cancel()
{
    bJobCanceled_ = true;
    nState_ = eCanceled;
    for( auto itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        ( *itPathSection )->Cancel();
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double Path_ABC::GetLength() const
{
    double rLength = 0.;
    for( auto itPathSection = pathSections_.begin(); itPathSection != pathSections_.end(); ++itPathSection )
        rLength += ( *itPathSection )->GetLength();
    return rLength;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::DoExecute
// Created: AGE 2006-03-07
// -----------------------------------------------------------------------------
void Path_ABC::DoExecute( TER_Pathfinder_ABC& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = GET_HOOK( GetMaxPathFindComputationDuration )();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    if( pathSections_.empty() )
        throw MASA_EXCEPTION( "List of path sections is empty" );
    const PathResult::T_PathPointList& pathPoints = pathSections_.front()->GetPath().GetResult( false );

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
void Path_ABC::Execute( TER_Pathfinder_ABC& pathfind )
{
    try
    {
        DoExecute( pathfind );
    }
    catch( const std::exception& )
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
    for( auto itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection )
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
std::size_t Path_ABC::GetID() const
{
    return identifier_;
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
void Path_ABC::RegisterPathSection( boost::shared_ptr< PathSection_ABC > section )
{
    pathSections_.push_back( section );
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool Path_ABC::operator==( const Path_ABC& rhs ) const
{
    return identifier_ == rhs.identifier_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool Path_ABC::operator!=( const Path_ABC& rhs ) const
{
    return identifier_ != rhs.identifier_;
}