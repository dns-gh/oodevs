//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: MOS_AgentKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_AgentKnowledge_h_
#define __MOS_AgentKnowledge_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"
#include "MOS_Perception_Def.h"
#include "OptionalValue.h"

class MOS_Agent;
class MOS_Gtia;


// =============================================================================
/** @class  MOS_AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentKnowledge
{

private:
    MT_COPYNOTALLOWED( MOS_AgentKnowledge );
    friend class MOS_GLTool;
    friend class MOS_AgentKnowledgePanel;

public:
     MOS_AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg, MOS_Gtia& owner );
    ~MOS_AgentKnowledge();

    //! @name Events
    //@{
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
    uint               GetID       () const;
    const MT_Vector2D& GetPosition () const;
    MOS_Agent&         GetRealAgent() const;
    MOS_Gtia&          GetOwner() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< MOS_Agent*, E_PerceptionResult > T_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::iterator          IT_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::const_iterator    CIT_AutomatePerceptionMap;
    //@}

private:
    MOS_Gtia&         owner_;
    MOS_Agent&        realAgent_;
    
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
    static MOS_IDManager idManager_;
};


#	include "MOS_AgentKnowledge.inl"

#endif // __MOS_AgentKnowledge_h_