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

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_AgentPion;
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
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_PopulationInteraction( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_PopulationInteraction();
    virtual ~DEC_KS_PopulationInteraction();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk(int currentTimeStep );
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    void Clear();
    //@}

    //! @name Events
    //@{
    void NotifyPopulationCollision( MIL_PopulationFlow& flow );
    void NotifyPopulationCollision( MIL_PopulationConcentration& concentration );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_PopulationFlow* > T_PopulationFlowVector;
    typedef std::vector< MIL_PopulationConcentration* > T_PopulationConcentrationVector;
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
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    T_PopulationFlowVector             flowCollisions_;
    T_PopulationConcentrationVector    concentrationCollisions_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_PopulationInteraction )

#endif // __DEC_KS_PopulationInteraction_h_
