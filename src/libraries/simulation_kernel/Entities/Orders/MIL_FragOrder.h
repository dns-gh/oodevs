// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FragOrder_h_
#define __MIL_FragOrder_h_

class DEC_Decision_ABC;
class DEC_KnowledgeResolver_ABC;
class DEC_Representations;
class MIL_FragOrderType;
class MIL_MissionParameter_ABC;
class MIL_ParameterType_MedicalPriorities;
class MIL_ParameterType_MaintenancePriorities;
class MT_Vector2D;
class PHY_DotationCategory;

namespace directia
{
    class Brain;
}

// =============================================================================
/** @class  MIL_FragOrder
    @brief  MIL_FragOrder
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_FragOrder : public DIA_TypedObject
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& decision, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MsgFragOrder& asn );
             MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& decision, DEC_Representations& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_FragOrder& rhs );
    virtual ~MIL_FragOrder();
    //@}

    //! @name Operations
    //@{
    void Launch();
    static void Register( directia::Brain& brain );
    //@}
    
    //! @name Accessors
    //@{
    std::string GetDIAType() const;
    int GetAmbianceMission() const;
    boost::shared_ptr< PHY_DotationCategory > GetMunitions() const;
    int GetNbIT() const;
    int GetNbrAmbulances() const;
    int GetNbrRemorqueurs() const;
    int GetOrderConduiteAutomateActiverObstacle() const;
    int GetOrderConduiteChangerAmbiance() const;
    boost::shared_ptr< MT_Vector2D > GetOrderConduiteChangerPositionDebarquement() const;
    int GetOrderConduiteChangerReglesEngagementPopulation() const;
    int GetOrderConduiteChangerReglesEngagement() const;
    MIL_ParameterType_MedicalPriorities* GetOrderConduiteModifierPrioritesBlesses() const;
    MIL_ParameterType_MaintenancePriorities* GetOrderConduiteModifierPrioritesReparations() const;
    std::vector< DEC_Decision_ABC* > GetOrderConduiteModifierPrioritesTactiquesBlesses() const;
    std::vector< DEC_Decision_ABC* > GetOrderConduiteModifierPrioritesTactiquesReparations() const;
    int GetOrderConduiteModifierRegimeTravailMaintenance() const;
    int GetOrderConduitePopulationChangerAttitude() const;
    DEC_Decision_ABC* GetPionARenforcer() const;
    DEC_Decision_ABC* GetPionRenforce() const;
    boost::shared_ptr< MT_Vector2D > GetPointCible() const;
    int GetPorteeAction() const;
    int GetSiteFranchissementOriginal() const;
    int GetSiteFranchissementVariante() const;
    //@}

private:
    const MIL_FragOrderType&   type_;
          DEC_Representations& representations_;
          std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters_;
          DEC_Decision_ABC& decision_;
};

#endif // __MIL_FragOrder_h_
