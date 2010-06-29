 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlowKnowledge_h_
#define __PopulationFlowKnowledge_h_


#include "SimpleEntity.h"
#include "PopulationFlowPart.h"

namespace Common
{
    enum EnumPopulationAttitude;
}

namespace MsgsSimToClient
{
    class MsgPopulationFlowKnowledgeCreation;
    class MsgPopulationFlowKnowledgeUpdate;
}

namespace kernel
{
    class PopulationKnowledge_ABC;
    class PopulationFlow_ABC;
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
             PopulationFlowKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& msg );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlowKnowledge( const PopulationFlowKnowledge& );            //!< Copy constructor
    PopulationFlowKnowledge& operator=( const PopulationFlowKnowledge& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned portions_fluxPresent : 1;
        unsigned directionPresent : 1;
        unsigned vitessePresent : 1;
        unsigned nb_humains_vivantsPresent : 1;
        unsigned nb_humains_mortsPresent : 1;
        unsigned attitudePresent : 1;
        unsigned est_percuPresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationKnowledge_ABC& populationKnowledge_;
    const kernel::PopulationFlow_ABC*      pFlow_;
    std::vector< PopulationFlowPart >      flowParts_;
    unsigned int                           nDirection_;
    unsigned int                           nSpeed_;
    unsigned long                          nNbrAliveHumans_;
    unsigned long                          nNbrDeadHumans_;
    Common::EnumPopulationAttitude         nAttitude_;
    bool                                   bPerceived_;
    T_Optionals                            optionals_;
    //@}
};

}

#endif // __PopulationFlowKnowledge_h_
