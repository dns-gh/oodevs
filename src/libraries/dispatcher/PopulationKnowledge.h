 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "AsnTypes.h"
#include "ModelsContainer.h"

namespace dispatcher
{
class KnowledgeGroup;
class Publisher_ABC;
class Model;
class Side;
class Population;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;

// =============================================================================
/** @class  PopulationKnowledge
    @brief  PopulationKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
     PopulationKnowledge( Model& model, const ASN1T_MsgPopulationKnowledgeCreation& msg );
    ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
          unsigned long   GetID            () const;
    const KnowledgeGroup& GetKnowledgeGroup() const;
    const Population&     GetPopulation    () const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationKnowledgeUpdate&                   msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& msg );
    void Update        ( const ASN1T_MsgPopulationFluxKnowledgeCreation&             msg );
    void Update        ( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               msg );
    void Update        ( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationKnowledge( const PopulationKnowledge& );            //!< Copy constructor
    PopulationKnowledge& operator=( const PopulationKnowledge& ); //!< Assignement operator
    //@}

private:
          Model&          model_;
    const unsigned long   nID_;
    const KnowledgeGroup& knowledgeGroup_;
    const Population&     population_;
    const Side&           side_;
    
    unsigned int         nDominationState_;

    ModelsContainer< PopulationConcentrationKnowledge > concentrations_;
    ModelsContainer< PopulationFlowKnowledge          > flows_;
};

}

#include "PopulationKnowledge.inl"

#endif // __PopulationKnowledge_h_
