// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathComputer_h_
#define __DEC_PathComputer_h_

#include "MT_Tools/MT_Profiler.h"
#include "simulation_terrain/TER_PathComputer_ABC.h"
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class TER_PathPoint;
struct TER_PathResult;

// =============================================================================
/** @class  DEC_PathComputer
    @brief  DEC_PathComputer
*/
// Created: MCO 2014-05-15
// =============================================================================
class DEC_PathComputer : public TER_PathComputer_ABC
{
public:
             DEC_PathComputer( std::size_t id );
    virtual ~DEC_PathComputer();

    virtual double GetLength() const;
    virtual boost::shared_ptr< TER_PathResult > Execute( TER_Pathfinder_ABC& pathfind,
            unsigned int deadlineSeconds, bool debugPath );
    virtual boost::shared_ptr< TER_PathResult > Cancel();
    virtual void RegisterPathSection( TER_PathSection& section );

private:
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint );

    void DoExecute( TER_Pathfinder_ABC& pathfind, unsigned int deadlineSeconds );
    void NotifyPartialSection();
    void NotifyCompletedSection();
    boost::optional< MT_Vector2D > DEC_PathComputer::GetLastPosition() const;
    boost::shared_ptr< TER_PathResult > GetPathResult() const;

    std::string DEC_PathComputer::GetPathAsString() const;
    std::string DEC_PathComputer::GetStateAsString() const;

private:
    typedef std::vector< TER_PathSection* > T_PathSectionVector;
    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;

    class Canceler;

private:
    MT_Profiler profiler_;
    std::size_t id_;
    // Identify the current computation, for logging purpose
    const std::size_t computerId_;
    TER_Path_ABC::E_State nState_;
    bool bJobCanceled_;
    T_PathSectionVector pathSections_;
    MT_Vector2D lastWaypoint_;
    std::vector< MT_Vector2D > computedWaypoints_;
    T_PathPoints resultList_;
    // Kept as a class attribute to have a reference to cancel it asynchronously
    std::unique_ptr< Canceler > canceler_;
};

#endif // __DEC_PathComputer_h_
