 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentrationKnowledge_h_
#define __PopulationConcentrationKnowledge_h_

#include "game_asn/Asn.h"
#include "Position.h"
#include "Entity_ABC.h"

namespace dispatcher
{
class Model;
class PopulationKnowledge;
class PopulationConcentration;

// =============================================================================
/** @class  PopulationConcentrationKnowledge
    @brief  PopulationConcentrationKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationConcentrationKnowledge : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentrationKnowledge( Model& model, PopulationKnowledge& populationKnowledge, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg );
    virtual ~PopulationConcentrationKnowledge();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentrationKnowledge( const PopulationConcentrationKnowledge& );            //!< Copy constructor
    PopulationConcentrationKnowledge& operator=( const PopulationConcentrationKnowledge& ); //!< Assignement operator
    //@}

private:
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
          PopulationKnowledge&  populationKnowledge_;
    const unsigned long         nID_;
    const Position              position_;

    const PopulationConcentration*     pConcentration_;
          unsigned long                nNbrAliveHumans_;
          unsigned long                nNbrDeadHumans_;
          ASN1T_EnumPopulationAttitude nAttitude_;    
          unsigned int                 nRelevance_;
          bool                         bPerceived_;

          T_Optionals                  optionals_;
};

}

#include "PopulationConcentrationKnowledge.inl"

#endif // __PopulationConcentrationKnowledge_h_
