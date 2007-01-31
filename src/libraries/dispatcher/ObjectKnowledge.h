 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#include "game_asn/Asn.h"
#include "Localisation.h"
#include "ModelRefsContainer.h"

namespace dispatcher
{
    class Model;
    class Publisher_ABC;
    class KnowledgeGroup;
    class Object;
    class Automat;
    class Side;
    class ObjectAttribute_ABC;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObjectKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
     ObjectKnowledge( Model& model, const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    void SendCreation  ( Publisher_ABC& publisher );
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{

    //$$$ bullshit
    struct T_Optionals 
    {
        unsigned oid_objet_reelPresent : 1;
        unsigned pertinencePresent : 1;
        unsigned localisationPresent : 1;
        unsigned pourcentage_constructionPresent : 1;
        unsigned pourcentage_valorisationPresent : 1;
        unsigned pourcentage_contournementPresent : 1;
        unsigned en_preparationPresent : 1;
        unsigned est_percuPresent : 1;
        unsigned attributs_specifiquesPresent : 1;
        unsigned perception_par_compagniePresent : 1;
        unsigned nb_dotation_constructionPresent : 1;
        unsigned nb_dotation_valorisationPresent : 1;
    };
    //@}

private:
          Model&                model_;
    const unsigned int          nID_; 
    const Side&                 side_;
    const Object*               pObject_;        
    const ASN1T_EnumObjectType  nType_;
    const unsigned int          nTypeDotationForConstruction_;
    const unsigned int          nTypeDotationForMining_;

    ObjectAttribute_ABC*           pAttributes_;
    unsigned int                   nRelevance_;
    Localisation                   localisation_;
    unsigned int                   nConstructionPercentage_;
    unsigned int                   nMiningPercentage_;
    unsigned int                   nBypassingPercentage_;
    bool                           bPrepared_;
    bool                           bPerceived_;
    ModelRefsContainer< Automat >  automatPerceptions_;
    unsigned int                   nNbrDotationForConstruction_;
    unsigned int                   nNbrDotationForMining_;

    T_Optionals                    optionals_;
};

}

#endif // __ObjectKnowledge_h_
