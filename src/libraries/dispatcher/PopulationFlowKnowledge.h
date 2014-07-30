 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlowKnowledge_h_
#define __PopulationFlowKnowledge_h_

#include "SimpleEntity.h"
#include "PopulationFlowPart.h"

namespace sword
{
    enum EnumCrowdAttitude;
    class CrowdFlowKnowledgeCreation;
    class CrowdFlowKnowledgeUpdate;
}

namespace kernel
{
    class PopulationKnowledge_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationFlowKnowledge
    @brief  PopulationFlowKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlowKnowledge : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlowKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const sword::CrowdFlowKnowledgeCreation& msg );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::CrowdFlowKnowledgeUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlowKnowledge( const PopulationFlowKnowledge& );            //!< Copy constructor
    PopulationFlowKnowledge& operator=( const PopulationFlowKnowledge& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned partsPresent : 1;
        unsigned directionPresent : 1;
        unsigned vitessePresent : 1;
        unsigned alivePresent : 1;
        unsigned deadPresent : 1;
        unsigned attitudePresent : 1;
        unsigned perceivedPresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationKnowledge_ABC& populationKnowledge_;
    unsigned int                           flowId_;
    std::vector< PopulationFlowPart >      flowParts_;
    unsigned int                           nDirection_;
    unsigned int                           nSpeed_;
    unsigned long                          nNbrAliveHumans_;
    unsigned long                          nNbrDeadHumans_;
    sword::EnumCrowdAttitude              nAttitude_;
    bool                                   bPerceived_;
    T_Optionals                            optionals_;
    //@}
};

}

#endif // __PopulationFlowKnowledge_h_
