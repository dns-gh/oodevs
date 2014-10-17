// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_Agent_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_Agent_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Agent_ABC_h_
#define __MIL_Agent_ABC_h_

#include "simulation_kernel/Entities/MIL_Entity_ABC.h"
#include "MIL_AgentType_ABC.h"

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_KnowledgeBlackBoard_AgentPion;
class DEC_Agent_Path;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_KnowledgeGroup;
class MIL_PionOrderManager;
class MIL_Population;
class MT_Vector2D;
class TER_Agent_ABC;

namespace logistic
{
    class LogisticHierarchy_ABC;
}

// =============================================================================
// @class  MIL_Agent_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Agent_ABC : public MIL_Entity_ABC
{
public:
    explicit MIL_Agent_ABC( xml::xistream& xis );
    explicit MIL_Agent_ABC( const std::string& name );
    virtual ~MIL_Agent_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const = 0;
    virtual const MIL_AgentType_ABC& GetType() const = 0;
    virtual bool IsMarkedForDestruction() const = 0;
    virtual bool IsDead() const = 0;
    virtual bool IsNeutralized() const = 0;
    virtual bool UpdateUnderIndirectFire() = 0;
    virtual bool IsPC() const = 0;
    virtual bool IsCivilian() const = 0;
    virtual bool IsAutonomous() const = 0;
    virtual bool IsJammed() const = 0;
    virtual bool IsLogisticJammed() const = 0;
    virtual const std::string& GetCriticalIntelligence() const = 0;
    virtual float GetAffinity( unsigned long teamID ) const = 0;

    virtual const MIL_Automate& GetAutomate() const = 0;
    virtual       MIL_Automate& GetAutomate() = 0;

    virtual const DEC_Decision_ABC& GetDecision() const = 0;
    virtual       DEC_Decision_ABC& GetDecision() = 0;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const = 0;

    virtual const MIL_PionOrderManager& GetOrderManager() const = 0;
    virtual MIL_PionOrderManager& GetOrderManager() = 0;

    virtual void ChangeSuperior( MIL_Automate& newAutomate ) = 0;

    virtual logistic::LogisticHierarchy_ABC& GetLogisticHierarchy() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void NotifyAttackedBy( MIL_AgentPion& pion, bool mustReport ) = 0;
    virtual void NotifyAttackedBy( MIL_Population& population ) = 0;
    virtual void MagicMove( const MT_Vector2D& ) = 0;
    virtual double Distance( const MIL_Agent_ABC& pion ) const = 0;
    virtual bool CanInteractWithTraffic() const = 0;
    virtual bool CanBeImpactedByTraffic() const = 0;
    virtual bool CanBeDeleted() const = 0;
    virtual void InteractWithTraffic( const MIL_Agent_ABC& agent ) = 0;
    virtual void InteractWithTraffic( const std::vector< TER_Agent_ABC* >& agents ) = 0;
    virtual void RegisterPath( const DEC_Agent_Path& agentPath ) = 0;
    virtual void UnregisterPath( const DEC_Agent_Path& agentPath ) = 0;
    virtual bool HasPath() const = 0;
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > CreateKnowledge( const MIL_KnowledgeGroup& group ) = 0;
    virtual bool                                     IsPerceived    ( const MIL_Agent_ABC& target ) const = 0; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    virtual DEC_KnowledgeBlackBoard_AgentPion&       GetKnowledge   () const = 0;
    //@}

    //! @name Operators
    //@{
    bool operator ==( const MIL_Agent_ABC& rhs ) const;
    bool operator !=( const MIL_Agent_ABC& rhs ) const;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Agent_ABC )

#endif // __MIL_Agent_ABC_h_
