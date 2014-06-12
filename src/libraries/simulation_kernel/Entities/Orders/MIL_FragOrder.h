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

namespace sword
{
    class MissionParameters;
}

class ActionManager;
class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_KnowledgeResolver_ABC;
class DEC_Representations;
class MIL_AgentPion;
class MIL_Automate;
class MIL_FragOrderType;
class MIL_MissionParameter_ABC;
class MIL_ParameterType_MedicalPriorities;
class MIL_ParameterType_MaintenancePriorities;
class MIL_Population;
class MT_Vector2D;
class TER_Localisation;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class PHY_HumanWound;

namespace sword
{
    class Brain;
}

namespace client
{
    class FragOrder;
}

// =============================================================================
/** @class  MIL_FragOrder
    @brief  MIL_FragOrder
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_FragOrder : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_FragOrder( const MIL_FragOrderType& type, uint32_t id );
             MIL_FragOrder( const MIL_FragOrderType& type, const MIL_FragOrder& rhs, uint32_t id );
    virtual ~MIL_FragOrder();
    //@}

    //! @name Operations
    //@{
    void SetParameters( const DEC_KnowledgeResolver_ABC& resolver, const sword::MissionParameters& parameters );
    static void Register( sword::Brain& brain );
    static boost::shared_ptr< MIL_FragOrder > CreateFragOrder( std::string );
    void Send( ActionManager& actions, MIL_AgentPion& pion ) const;
    void Send( ActionManager& actions, MIL_Automate& automat ) const;
    void Send( ActionManager& actions, MIL_Population& population ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const PHY_HumanWound* > T_MedicalPriorityVector;

    typedef std::vector< const PHY_ComposanteTypePion* > T_MaintenancePriorityVector;
    //@}

    //! @name Accessors
    //@{
    std::string GetDIAType() const;
    int GetAmbianceMission() const;
    const PHY_DotationCategory* GetMunition() const;
    std::vector< const PHY_DotationCategory* > GetMunitions() const;
    int GetNbIT() const;
    int GetNbrAmbulances() const;
    int GetNbrRemorqueurs() const;
    int GetOrderConduiteAutomateActiverObstacle() const;
    int GetOrderConduiteChangerAmbiance() const;
    void SetOrderConduiteChangerAmbiance( int value );
    boost::shared_ptr< MT_Vector2D > GetOrderConduiteChangerPositionDebarquement() const;
    int GetOrderConduiteChangerReglesEngagementPopulation() const;
    int GetOrderConduiteChangerReglesEngagement() const;
    T_MedicalPriorityVector GetOrderConduiteModifierPrioritesBlesses() const;
    T_MaintenancePriorityVector GetOrderConduiteModifierPrioritesReparations() const;
    std::vector< DEC_Decision_ABC* > GetOrderConduiteModifierPrioritesTactiquesBlesses() const;
    std::vector< DEC_Decision_ABC* > GetOrderConduiteModifierPrioritesTactiquesReparations() const;
    std::vector< const PHY_ComposanteTypePion* > GetEquipmentTypeListParameter( const std::string& ) const;
    int GetIntegerParameter( const std::string& ) const;
    std::vector< DEC_Decision_ABC* > GetAutomatListParameter( const std::string& ) const;
    const DEC_Decision_ABC* GetAgentParameter( const std::string& ) const;
    boost::shared_ptr< TER_Localisation > GetLocationParameter( const std::string& ) const;
    boost::shared_ptr< MT_Vector2D > GetPointParameter( const std::string& ) const;
    T_MedicalPriorityVector GetMedicalPriorityParameter( const std::string& ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetAgentKnowledgeParameter( const std::string& ) const;
    const PHY_DotationCategory* GetResourceTypeParameter( const std::string& ) const;
    std::vector< const PHY_DotationCategory* > GetResourceTypeListParameter( const std::string& ) const;
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > GetObjectListParameter( const std::string& ) const;
    int GetOrderConduiteModifierRegimeTravailMaintenance() const;
    int GetOrderConduitePopulationChangerAttitude() const;
    const DEC_Decision_ABC* GetPionARenforcer() const;
    const DEC_Decision_ABC* GetPionRenforce() const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetAgentKnowledge() const;
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > GetObjectKnowledge() const;
    const DEC_Decision_ABC* GetAgent() const;
    boost::shared_ptr< MT_Vector2D > GetPointCible() const;
    boost::shared_ptr< TER_Localisation > GetZoneCible() const;
    int GetPorteeAction() const;
    int GetSiteFranchissementOriginal() const;
    int GetSiteFranchissementVariante() const;
    //@}

private:

    //! @name Helpers
    //@{
    void Serialize( sword::MissionParameters& asn ) const;
    void Send( ActionManager& actions, client::FragOrder& message ) const;
    //@}

    //! @name Member data
    //@{
    const MIL_FragOrderType& type_;
    const uint32_t id_;
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters_;
    //@}
};

#endif // __MIL_FragOrder_h_
