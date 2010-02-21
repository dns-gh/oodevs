 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentrationKnowledge_h_
#define __PopulationConcentrationKnowledge_h_

#include "SimpleEntity.h"


namespace Common
{
    class MsgCoordLatLong;
    enum EnumPopulationAttitude;
}

namespace MsgsSimToClient
{
    class MsgPopulationConcentrationKnowledgeCreation;
    class MsgPopulationConcentrationKnowledgeUpdate;
}



namespace kernel
{
    class PopulationKnowledge_ABC;
    class PopulationConcentration_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationConcentrationKnowledge
    @brief  PopulationConcentrationKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationConcentrationKnowledge : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
    PopulationConcentrationKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& msg );
    virtual ~PopulationConcentrationKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentrationKnowledge( const PopulationConcentrationKnowledge& );            //!< Copy constructor
    PopulationConcentrationKnowledge& operator=( const PopulationConcentrationKnowledge& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals 
    {
      unsigned nb_humains_vivantsPresent : 1;
      unsigned nb_humains_mortsPresent : 1;
      unsigned attitudePresent : 1;
      unsigned pertinencePresent : 1;
      unsigned est_percuPresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationKnowledge_ABC&     populationKnowledge_;
    const kernel::PopulationConcentration_ABC* pConcentration_;
    const Common::MsgCoordLatLong&             position_;
    unsigned long                              nNbrAliveHumans_;
    unsigned long                              nNbrDeadHumans_;
    Common::EnumPopulationAttitude               nAttitude_;    
    unsigned int                               nRelevance_;
    bool                                       bPerceived_;
    T_Optionals                                optionals_;
    //@}
};

}

#endif // __PopulationConcentrationKnowledge_h_
