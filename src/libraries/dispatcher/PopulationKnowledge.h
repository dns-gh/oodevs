 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "tools/Resolver.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "SimpleEntity.h"

namespace MsgsSimToClient
{
    class MsgPopulationKnowledgeCreation;
    class MsgPopulationKnowledgeUpdate;
    class MsgPopulationConcentrationKnowledgeCreation;
    class MsgPopulationConcentrationKnowledgeUpdate;
    class MsgPopulationConcentrationKnowledgeDestruction;
    class MsgPopulationFlowKnowledgeCreation;
    class MsgPopulationFlowKnowledgeUpdate;
    class MsgPopulationFlowKnowledgeDestruction;
}

namespace kernel
{
    class KnowledgeGroup_ABC;
    class Team_ABC;
    class Population_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model;
    class PopulationConcentrationKnowledge;
    class PopulationFlowKnowledge;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationKnowledge
    @brief  PopulationKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationKnowledge : public SimpleEntity< kernel::PopulationKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge( Model& model, const MsgsSimToClient::MsgPopulationKnowledgeCreation& msg );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgPopulationKnowledgeUpdate&                   msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation&    msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate&      msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation&             msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate&               msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction&          msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept ( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Population_ABC*     GetEntity() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationKnowledge( const PopulationKnowledge& );            //!< Copy constructor
    PopulationKnowledge& operator=( const PopulationKnowledge& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
          Model&                      model_;
    const kernel::KnowledgeGroup_ABC& knowledgeGroup_;
    const kernel::Population_ABC&     population_;
    const kernel::Team_ABC&           team_;
    unsigned int                      nDominationState_;
    tools::Resolver< PopulationConcentrationKnowledge > concentrations_;
    tools::Resolver< PopulationFlowKnowledge          > flows_;
    //@}
};

}

#endif // __PopulationKnowledge_h_
