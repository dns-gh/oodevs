//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_Path.h $
// $Author: Age $
// $Modtime: 16/06/05 15:09 $
// $Revision: 10 $
// $Workfile: DEC_Population_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Population_Path_h_
#define __DEC_Population_Path_h_

#include "Decision/DEC_PathResult.h"
#include "Decision/DEC_Population_Path_Channeler.h"
#include "MT_Tools/MT_Profiler.h"

class DEC_Path_KnowledgeObject_ABC;
class MIL_Population;
class MIL_Object_ABC;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Population_Path : public DEC_PathResult
{
public:
    //! @name Types
    //@{
    typedef std::vector< DEC_Population_Path_Channeler >    T_PopulationPathChannelerVector;
    typedef T_PopulationPathChannelerVector::const_iterator CIT_PopulationPathChannelerVector;

    typedef std::vector< DEC_Path_KnowledgeObject_ABC* > T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator  CIT_PathKnowledgeObjectVector;

    typedef std::vector< T_PathKnowledgeObjectVector >         T_PathKnowledgeObjectByTypesVector;
    typedef T_PathKnowledgeObjectByTypesVector::const_iterator CIT_PathKnowledgeObjectByTypesVector;
    //@}

public:
             DEC_Population_Path( const MIL_Population& population, const MT_Vector2D& start, const MT_Vector2D& destination );
    virtual ~DEC_Population_Path();

    //! @name Path calculation
    //@{
    virtual void Execute( TerrainPathfinder& pathfind );
    virtual void CleanAfterComputation();
    virtual bool NeedRefine() const;
    virtual bool UseStrictClosest() const;
    //@}

    //! @name Accessors
    //@{
    const T_PopulationPathChannelerVector& GetChannelers() const;
    double GetCostOutsideOfChanneling() const;
    const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects() const;
    double GetCostOutsideOfAllObjects() const;
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
    const DEC_Population_PathClass& pathClass_;
    const MIL_Population& population_;
    T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
    double rCostOutsideOfAllObjects_;
    MT_Profiler profiler_;
    T_PopulationPathChannelerVector channelers_;
    //@}
};

#include "DEC_Population_Path.inl"

#endif // __DEC_Population_Path_h_
