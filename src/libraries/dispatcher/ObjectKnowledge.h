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
#include "Entity_ABC.h"

namespace dispatcher
{
    class Model;
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
class ObjectKnowledge : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( Model& model, const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    void Update        ( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void SendSpecialUpdate( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals 
    {
        unsigned relevancePresent : 1;
        unsigned locationPresent : 1;
        unsigned construction_percentagePresent : 1;
        unsigned mining_percentagePresent : 1;
        unsigned bypass_construction_percentagePresent : 1;
        unsigned reserved_obstacle_activatedPresent : 1;
        unsigned perceivedPresent : 1;
        unsigned specific_attributesPresent : 1;
        unsigned automat_perceptionPresent : 1;
        unsigned construction_dotation_nbrPresent : 1;
        unsigned mining_dotation_nbrPresent : 1;
        unsigned obstacle_typePresent : 1;
    };
    //@}

private:
          Model&                 model_;
    const unsigned int           nID_; 
    const Side&                  side_;
    const Object*                pObject_;        
    const ASN1T_EnumObjectType   nType_;
    const ASN1T_EnumObstacleType nObstacleType_;
    const unsigned int           nTypeDotationForConstruction_;
    const unsigned int           nTypeDotationForMining_;

    ObjectAttribute_ABC*           pAttributes_;
    unsigned int                   nRelevance_;
    Localisation                   localisation_;
    unsigned int                   nConstructionPercentage_;
    unsigned int                   nMiningPercentage_;
    unsigned int                   nBypassingPercentage_;
    bool                           bReservedObstacleActivated_;
    bool                           bPerceived_;
    ModelRefsContainer< Automat >  automatPerceptions_;
    unsigned int                   nNbrDotationForConstruction_;
    unsigned int                   nNbrDotationForMining_;

    T_Optionals                    optionals_;
};

}

#endif // __ObjectKnowledge_h_
