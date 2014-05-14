// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Population_Path_h_
#define __DEC_Population_Path_h_

#include "Decision/DEC_PathResult.h"
#include "Decision/DEC_Population_Path_Channeler.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/shared_ptr.hpp>

class DEC_Path_KnowledgeObject_ABC;
class MIL_Population;
class MIL_Object_ABC;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_PathResult
{
private:
    //! @name Types
    //@{
    typedef std::vector< DEC_Population_Path_Channeler >                     T_PopulationPathChannelers;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > > T_PathKnowledgeObjects;
    typedef std::vector< T_PathKnowledgeObjects >                            T_PathKnowledgeObjectsByTypes;
    //@}

public:
             DEC_Population_Path( const MIL_Population& population, const T_PointVector& destination );
    virtual ~DEC_Population_Path();

    //! @name Path calculation
    //@{
    virtual void Execute( TER_Pathfinder_ABC& pathfind );
    virtual void CleanAfterComputation();
    virtual bool IsPathForUnit( MIL_Agent_ABC* pion ) const;
    //@}

    //! @name Accessors
    //@{
    const T_PopulationPathChannelers& GetChannelers() const;
    double GetCostOutsideOfChanneling() const;
    const T_PathKnowledgeObjectsByTypes& GetPathKnowledgeObjects() const;
    double GetCostOutsideOfAllObjects() const;
    double GetMaxSpeed( const TerrainData& terrainData ) const;
    double GetMaxSpeed() const;
    //@}

private:
    DEC_Population_Path( const DEC_Population_Path& rhs ); // Copy only query parameters, not the result !

    //! @name Init
    //@{
    void Initialize( const T_PointVector& pathPoints );
    void InitializePathKnowledges( const T_PointVector& pathPoints );
    //@}

    //! @name Tools
    //@{
    virtual void InsertDecPoints();
    //@}

private:
    //! @name Member data
    //@{
    const size_t id_;
    const DEC_Population_PathClass& pathClass_;
    const MIL_Population& population_;
    T_PathKnowledgeObjectsByTypes pathKnowledgeObjects_;
    double rCostOutsideOfAllObjects_;
    MT_Profiler profiler_;
    T_PopulationPathChannelers channelers_;
    //@}
};

#endif // __DEC_Population_Path_h_
