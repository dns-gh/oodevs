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
#include <vector>

class TER_PathPoint;

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
    virtual void Execute( TER_Pathfinder_ABC& pathfind, unsigned int deadline,
           bool debugPath );
    virtual void Cancel();
    virtual TER_Path_ABC::E_State GetState() const;
    virtual void RegisterPathSection( TER_PathSection& section );

    virtual const MT_Vector2D& GetLastWaypoint() const;
    virtual const std::vector< MT_Vector2D >& GetComputedWaypoints() const;
    virtual void RemoveComputedWaypoint();

    void Serialize( sword::PathResult& msg ) const;

private:
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint );
    virtual T_PathPoints GetResult() const;

    void DoExecute( TER_Pathfinder_ABC& pathfind, unsigned int deadline );
    void NotifyPartialSection();
    void NotifyCompletedSection();
    boost::optional< MT_Vector2D > DEC_PathComputer::GetLastPosition() const;

    std::string DEC_PathComputer::GetPathAsString() const;
    std::string DEC_PathComputer::GetStateAsString() const;

private:
    typedef std::vector< TER_PathSection* > T_PathSectionVector;
    typedef std::list< boost::shared_ptr< TER_PathPoint > > T_PathPoints;

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
};

#endif // __DEC_PathComputer_h_
