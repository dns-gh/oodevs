//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: AgentKnowledge.h $
//
//*****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "Perception_Def.h"
#include "OptionalValue.h"

class Agent;
class Gtia;


// =============================================================================
/** @class  AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentKnowledge
{

private:
    MT_COPYNOTALLOWED( AgentKnowledge );
    friend class GLTool;
    friend class AgentKnowledgePanel;

public:
     AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg, Gtia& owner );
    ~AgentKnowledge();

    //! @name Events
    //@{
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
    uint               GetID       () const;
    const MT_Vector2D& GetPosition () const;
    Agent&         GetRealAgent() const;
    Gtia&          GetOwner() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< Agent*, E_PerceptionResult > T_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::iterator          IT_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::const_iterator    CIT_AutomatePerceptionMap;
    //@}

private:
    Gtia&         owner_;
    Agent&        realAgent_;
    
    MT_Vector2D vPosition_;

    OptionalValue< uint >                       nID_;
    OptionalValue< std::string >                strPosition_;
    OptionalValue< uint >                       nDirection_;
    OptionalValue< uint >                       nSpeed_;
    OptionalValue< uint >                       nEtatOps_;
    OptionalValue< E_PerceptionResult >         nCurrentPerceptionLevel_;
    OptionalValue< E_PerceptionResult >         nMaxPerceptionLevel_;
    OptionalValue< uint >                       nTeam_;
    OptionalValue< E_NatureLevel >              nLevel_;
    OptionalValue< E_UnitNatureWeapon >         nWeapon_;
    OptionalValue< E_UnitNatureSpecialization > nSpecialization_;
    OptionalValue< E_UnitNatureQualifier >      nQualifier_;
    OptionalValue< E_UnitNatureCategory >       nCategory_;
    OptionalValue< E_UnitNatureMobility >       nMobility_;
    OptionalValue< E_UnitCapaciteMission >      nCapacity_;
    OptionalValue< bool >                       bIsPC_;
    OptionalValue< bool >                       bSurrendered_;
    OptionalValue< bool >                       bPrisonner_;
    OptionalValue< bool >                       bRefugies_;
    OptionalValue< uint >                       nRelevance_;
    OptionalValue< T_AutomatePerceptionMap >    automatePerceptionMap_;

private:
    static IDManager idManager_;
};


#	include "AgentKnowledge.inl"

#endif // __AgentKnowledge_h_