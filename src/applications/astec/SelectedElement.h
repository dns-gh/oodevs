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
// $Archive: /MVW_v10/Build/SDK/Light2/src/SelectedElement.h $
// $Author: Age $
// $Modtime: 5/04/05 18:17 $
// $Revision: 5 $
// $Workfile: SelectedElement.h $
//
// *****************************************************************************

#ifndef __SelectedElement_h_
#define __SelectedElement_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

class Agent;
class Gtia;
class Team;
class Population;
class PopulationConcentration;
class PopulationFlow;
class Object_ABC;
class AgentKnowledge;
class ObjectKnowledge;
class PopulationKnowledge;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class RC;
class TacticalLine_ABC;


// =============================================================================
// Created: APE 2004-05-05
// =============================================================================
class SelectedElement
{

public:
    //! @name Constructors/Destructor
    //@{
     SelectedElement();
     SelectedElement( Agent&                            agent );
	 SelectedElement( Population&                       population );
     SelectedElement( PopulationConcentration&          populationConcentration );
     SelectedElement( PopulationFlow&                   populationFlow );
     SelectedElement( Gtia&                             gtia );
     SelectedElement( Team&                             team );
     SelectedElement( Object_ABC&                       object );
     SelectedElement( AgentKnowledge&                   agentKnowledge );
     SelectedElement( ObjectKnowledge&                  objectKnowledge );
     SelectedElement( PopulationKnowledge&              populationKnowledge );
     SelectedElement( PopulationConcentrationKnowledge& populationConcentrationKnowledge );
     SelectedElement( PopulationFlowKnowledge&          populationFlowKnowledge );
     SelectedElement( RC&                               rc );
     SelectedElement( TacticalLine_ABC&                 line, int nLinePoint = -1 );
    ~SelectedElement();
    //@}

    //! @name Accessors
    //@{
    bool IsAMapElementSelected() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const SelectedElement& ) const;
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
    Agent*                            pAgent_;
	Population*			              pPopulation_;
    PopulationConcentration*          pPopulationConcentration_;
    PopulationFlow*                   pPopulationFlow_;
    Gtia*                             pGtia_;
    Team*                             pTeam_;
    Object_ABC*			              pObject_;
    AgentKnowledge*                   pAgentKnowledge_;
    ObjectKnowledge*	              pObjectKnowledge_;
    PopulationKnowledge*              pPopulationKnowledge_;
    PopulationConcentrationKnowledge* pPopulationConcentrationKnowledge_;
    PopulationFlowKnowledge*          pPopulationFlowKnowledge_;
    //RC*                               pRC_;
    TacticalLine_ABC*                 pLine_;
    int                                   nLinePoint_;
    //@}
};

#   include "SelectedElement.inl"

#endif // __SelectedElement_h_
