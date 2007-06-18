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

#include "game_asn/Asn.h"
#include "ModelsContainer.h"
#include "Entity_ABC.h"

namespace dispatcher
{
class Publisher_ABC;
class Model;
class PopulationKnowledge;
class PopulationFlow;
class PopulationFlowPart;

// =============================================================================
/** @class  PopulationFlowKnowledge
    @brief  PopulationFlowKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlowKnowledge : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlowKnowledge( Model& model, PopulationKnowledge& populationKnowledge, const ASN1T_MsgPopulationFlowKnowledgeCreation& msg );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationFlowKnowledgeUpdate& msg );
    void Update        ( const ASN1T_MsgPopulationFlowKnowledgeCreation& msg );
    virtual void SendCreation   ( Publisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( Publisher_ABC& publisher ) const;
    virtual void SendDestruction( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlowKnowledge( const PopulationFlowKnowledge& );            //!< Copy constructor
    PopulationFlowKnowledge& operator=( const PopulationFlowKnowledge& ); //!< Assignement operator
    //@}

private:
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
          Model&                model_;
          PopulationKnowledge&  populationKnowledge_;
    const unsigned long         nID_;

    const PopulationFlow*                       pFlow_;
          ModelsContainer< PopulationFlowPart > flowParts_;
          unsigned int                          nDirection_;
          unsigned int                          nSpeed_;
          unsigned long                         nNbrAliveHumans_;
          unsigned long                         nNbrDeadHumans_;
          ASN1T_EnumPopulationAttitude          nAttitude_;    
          bool                                  bPerceived_;

          T_Optionals                           optionals_;
};

}

#include "PopulationFlowKnowledge.inl"

#endif // __PopulationFlowKnowledge_h_
