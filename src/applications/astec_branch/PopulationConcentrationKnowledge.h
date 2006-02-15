//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationConcentrationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationConcentrationKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationConcentrationKnowledge_h_
#define __PopulationConcentrationKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "OptionalValue.h"

class KnowledgeGroup;
class PopulationConcentration;
class PopulationKnowledge;
class Controller;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationConcentrationKnowledge
{
    MT_COPYNOTALLOWED( PopulationConcentrationKnowledge );
    friend class PopulationKnowledgePanel;
    friend class GLTool;
        
public:
    //! @name Constructor/Destructor
    //@{
    explicit PopulationConcentrationKnowledge( Controller& controller, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg );
    virtual ~PopulationConcentrationKnowledge();
    //@}

    //! @name Accessors
    //@{
    uint                               GetID                 () const;
    const KnowledgeGroup&                    GetKnowledgeGroup               () const;
    const PopulationKnowledge*     GetPopulationKnowledge() const;
    const PopulationConcentration* GetConcentration      () const;
    const MT_Vector2D&                 GetPosition           () const;
    uint                               GetNbrAliveHumans     () const;
    uint                               GetNbrDeadHumans      () const;
    E_PopulationAttitude               GetAttitude           () const;
    MT_Float                           GetRelevance          () const;
    bool                               IsPerceived           () const;
    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg );
    //@}

private:
    Controller& controller_;
    const uint                                   nID_;
          KnowledgeGroup*                              pKnowledgeGroup_;
    const PopulationKnowledge*               pPopulationKnowledge_;
    const PopulationConcentration*           pConcentration_;
          MT_Vector2D                            position_;
          OptionalValue< uint >                  nNbrAliveHumans_;
          OptionalValue< uint >                  nNbrDeadHumans_;
          OptionalValue< E_PopulationAttitude >  eAttitude_;
          OptionalValue< bool >                  bIsPerceived_;
          MT_Float                               rRelevance_;
};


#include "PopulationConcentrationKnowledge.inl"

#endif // __PopulationConcentrationKnowledge_h_