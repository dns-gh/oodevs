// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SelectedElement.h $
// $Author: Age $
// $Modtime: 5/04/05 18:17 $
// $Revision: 5 $
// $Workfile: MOS_SelectedElement.h $
//
// *****************************************************************************

#ifndef __MOS_SelectedElement_h_
#define __MOS_SelectedElement_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"

class MOS_Agent;
class MOS_Gtia;
class MOS_Team;
class MOS_Population;
class MOS_PopulationConcentration;
class MOS_PopulationFlow;
class MOS_Object_ABC;
class MOS_AgentKnowledge;
class MOS_ObjectKnowledge;
class MOS_PopulationKnowledge;
class MOS_PopulationConcentrationKnowledge;
class MOS_PopulationFlowKnowledge;
class MOS_RC;
class MOS_TacticalLine_ABC;


// =============================================================================
// Created: APE 2004-05-05
// =============================================================================
class MOS_SelectedElement
{

public:
    //! @name Constructors/Destructor
    //@{
     MOS_SelectedElement();
     MOS_SelectedElement( MOS_Agent&                            agent );
	 MOS_SelectedElement( MOS_Population&                       population );
     MOS_SelectedElement( MOS_PopulationConcentration&          populationConcentration );
     MOS_SelectedElement( MOS_PopulationFlow&                   populationFlow );
     MOS_SelectedElement( MOS_Gtia&                             gtia );
     MOS_SelectedElement( MOS_Team&                             team );
     MOS_SelectedElement( MOS_Object_ABC&                       object );
     MOS_SelectedElement( MOS_AgentKnowledge&                   agentKnowledge );
     MOS_SelectedElement( MOS_ObjectKnowledge&                  objectKnowledge );
     MOS_SelectedElement( MOS_PopulationKnowledge&              populationKnowledge );
     MOS_SelectedElement( MOS_PopulationConcentrationKnowledge& populationConcentrationKnowledge );
     MOS_SelectedElement( MOS_PopulationFlowKnowledge&          populationFlowKnowledge );
     MOS_SelectedElement( MOS_RC&                               rc );
     MOS_SelectedElement( MOS_TacticalLine_ABC&                 line, int nLinePoint = -1 );
    ~MOS_SelectedElement();
    //@}

    //! @name Accessors
    //@{
    bool IsAMapElementSelected() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MOS_SelectedElement& ) const;
    //@}

    //! @name Operations
    //@{
    void Dump();
    //@}

private:
    //! @name Helpers
    //@{
    void Init();
    //@}

public:
    //! @name Public member data
    //@{
    MOS_Agent*                            pAgent_;
	MOS_Population*			              pPopulation_;
    MOS_PopulationConcentration*          pPopulationConcentration_;
    MOS_PopulationFlow*                   pPopulationFlow_;
    MOS_Gtia*                             pGtia_;
    MOS_Team*                             pTeam_;
    MOS_Object_ABC*			              pObject_;
    MOS_AgentKnowledge*                   pAgentKnowledge_;
    MOS_ObjectKnowledge*	              pObjectKnowledge_;
    MOS_PopulationKnowledge*              pPopulationKnowledge_;
    MOS_PopulationConcentrationKnowledge* pPopulationConcentrationKnowledge_;
    MOS_PopulationFlowKnowledge*          pPopulationFlowKnowledge_;
    MOS_RC*                               pRC_;
    MOS_TacticalLine_ABC*                 pLine_;
    int                                   nLinePoint_;
    //@}
};

#   include "MOS_SelectedElement.inl"

#endif // __MOS_SelectedElement_h_
