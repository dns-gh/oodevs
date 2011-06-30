// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationPerception_h_
#define __DEC_Knowledge_PopulationPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class MIL_Population;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class MIL_Agent_ABC;
class DEC_Knowledge_PopulationConcentrationPerception;
class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationPerception : public DEC_Knowledge_ABC
{
public:
    //! @name Types
    //@{
    typedef std::map< const MIL_PopulationConcentration*, DEC_Knowledge_PopulationConcentrationPerception* > T_ConcentrationMap;
    typedef T_ConcentrationMap::iterator                                                                    IT_ConcentrationMap;
    typedef T_ConcentrationMap::const_iterator                                                             CIT_ConcentrationMap;

    typedef std::map< const MIL_PopulationFlow*, DEC_Knowledge_PopulationFlowPerception* > T_FlowMap;
    typedef T_FlowMap::iterator                                                           IT_FlowMap;
    typedef T_FlowMap::const_iterator                                                    CIT_FlowMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationPerception( const MIL_Agent_ABC& agentPerceiving, MIL_Population& populationPerceived );
             DEC_Knowledge_PopulationPerception();
    virtual ~DEC_Knowledge_PopulationPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( MIL_PopulationConcentration& concentrationPerceived, const PHY_PerceptionLevel& level );
    void Update( MIL_PopulationFlow& flowPerceived, const PHY_PerceptionLevel& level, const T_PointVector& shape );
    bool Clean();

    void PublishKnowledges( DEC_Knowledge_Population& knowledge ) const;
    bool IsIdentified( const MIL_PopulationConcentration& concentration );
    bool IsPerceived() const; // Perception au tick courant
    //@}

    //! @name Attacker
    //@{
    void NotifyAttacker();
    bool IsAttacker() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork() const;
    void SendStateToNewClient( unsigned int nCtx = 0 ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulationPerceived() const;
    const MIL_Agent_ABC& GetAgentPerceiving() const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC*  pAgentPerceiving_;
    MIL_Population* pPopulationPerceived_;
    T_ConcentrationMap concentrations_;
    T_FlowMap flows_;
    bool bAttacker_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationPerception )

#endif // __DEC_Knowledge_PopulationPerception_h_
