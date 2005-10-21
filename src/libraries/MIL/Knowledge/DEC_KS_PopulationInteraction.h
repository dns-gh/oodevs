// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationInteraction.h $
// $Author: Jvt $
// $Modtime: 24/03/05 15:16 $
// $Revision: 4 $
// $Workfile: DEC_KS_PopulationInteraction.h $
//
// *****************************************************************************

#ifndef __DEC_KS_PopulationInteraction_h_
#define __DEC_KS_PopulationInteraction_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_PopulationCollision;
class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_KS_PopulationInteraction
    @brief  This knowledge source (KS) is an 'IN', which is used when an agent
            interacts with an Population, to add this Population to its perception
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_PopulationInteraction : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_PopulationInteraction ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_PopulationInteraction( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& agentInteracting );
    virtual ~DEC_KS_PopulationInteraction();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    virtual void Clean  ();
    //@}

    //! @name Events
    //@{
    void NotifyPopulationCollision( MIL_PopulationFlow&          flow          );
    void NotifyPopulationCollision( MIL_PopulationConcentration& concentration );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_PopulationFlow* >     T_PopulationFlowVector;
    typedef T_PopulationFlowVector::const_iterator CIT_PopulationFlowVector;

    typedef std::vector< MIL_PopulationConcentration* >     T_PopulationConcentrationVector;
    typedef T_PopulationConcentrationVector::const_iterator CIT_PopulationConcentrationVector;
    //@}

private:
    //! @name Accessors
    //@{
    DEC_Knowledge_PopulationCollision& GetKnowledgePopulationCollision( MIL_Population& population ) const;
    //@}

    //! @name Tools
    //@{
    void CleanKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge );
    //@}

private:
    const MIL_AgentPion*                  pAgentInteracting_;
          T_PopulationFlowVector          flowCollisions_;
          T_PopulationConcentrationVector concentrationCollisions_;
};

#include "DEC_KS_PopulationInteraction.inl"

#endif // __DEC_KS_PopulationInteraction_h_
