// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_PopulationDecision.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationDecision_h_
#define __DEC_PopulationDecision_h_

#include "MIL.h"

class MIL_Population;
class MIL_PopulationMission_ABC;

// =============================================================================
// @class  DEC_PopulationDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationDecision : public DIA_Engine
{
    MT_COPYNOTALLOWED( DEC_PopulationDecision )

public:
             DEC_PopulationDecision( MIL_Population& population );
             DEC_PopulationDecision();
    virtual ~DEC_PopulationDecision();

    //! @name CheckPoints
    //@{
    /*BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;*/
    //@}

    //! @name Init
    //@{
    static void InitializeDIA();
    //@}
    
    //! @name Operations
    //@{
    void UpdateDecision();
    void Reset         ();

    void StartMissionBehavior( MIL_PopulationMission_ABC& mission );
    void StopMissionBehavior ( MIL_PopulationMission_ABC& mission );
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation() const;
    //@}

private:
    //! @name Tools
    //@{
    DIA_BehaviorPart& GetBehaviorPart           () const;
    bool              IsDefaultBehaviorAvailable() const;
    void              StartDefaultBehavior      ();
    void              StopDefaultBehavior       ();

    void              CleanStateAfterCrash      ();
    //@}

private:
    MIL_Population*                      pPopulation_;
    DIA_FunctionCaller< MIL_Population > diaFunctionCaller_;
    DIA_Parameters                       defaultBehaviorParameters_;
    DIA_Parameters                       missionBehaviorParameters_;   

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Population
    static int nDIANameIdx_;

    static uint nDefaultBehaviorDummyId_;
    static uint nMissionBehaviorDummyId_;
};

#include "DEC_PopulationDecision.inl"

#endif // __DEC_PopulationDecision_h_
