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

class MOS_Agent;
class MOS_Gtia;


// =============================================================================
/** @class  MOS_AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
    @par    A knowledge is often partial. @e nAttrUpdated_ is used to indicate
            which fields are valid.
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentKnowledge
{
    MT_COPYNOTALLOWED( MOS_AgentKnowledge );
    friend class MOS_GLTool;
        
private:
    enum E_AttributeUpdated
    {
        eUpdated_Position                = 0x00000001,
        eUpdated_Direction               = 0x00000002,
        eUpdated_Speed                   = 0x00000004,
        eUpdated_EtatOps                 = 0x00000008,
        eUpdated_CurrentPerceptionLevel  = 0x00000010,
        eUpdated_MaxPerceptionLevel      = 0x00000020,
        eUpdated_Team                    = 0x00000040,
        eUpdated_Level                   = 0x00000080,
        eUpdated_Weapon                  = 0x00000100,
        eUpdated_Specialization          = 0x00000200,
        eUpdated_Qualifier               = 0x00000400,
        eUpdated_Category                = 0x00000800,
        eUpdated_Mobility                = 0x00001000,
        eUpdated_IsPC                    = 0x00002000,
        eUpdated_AutomatePerception      = 0x00004000,
        eUpdated_Relevance               = 0x00008000,
        eUpdated_Capacity                = 0x00010000,
        eUpdated_Prisonner               = 0x00020000,
        eUpdated_Surrendered             = 0x00040000,
        eUpdated_Refugies                = 0x00080000,
    };

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
    bool               IsValid     ( E_AttributeUpdated ) const;
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
    uint nAttrUpdated_;

    uint              nID_;
    MOS_Gtia&         owner_;
    MOS_Agent&        realAgent_;
    MT_Vector2D       vPosition_;
    std::string       strPosition_;
    uint              nDirection_;
    uint              nSpeed_;
    uint              nEtatOps_;
    E_PerceptionResult nCurrentPerceptionLevel_;
    E_PerceptionResult nMaxPerceptionLevel_;

    uint              nTeam_;

    E_NatureLevel              nLevel_;
    E_UnitNatureWeapon         nWeapon_;
    E_UnitNatureSpecialization nSpecialization_;
    E_UnitNatureQualifier      nQualifier_;
    E_UnitNatureCategory       nCategory_;
    E_UnitNatureMobility       nMobility_;
    E_UnitCapaciteMission      nCapacity_;
    bool                       bIsPC_;

    bool bSurrendered_;
    bool bPrisonner_;
    bool bRefugies_;

    T_AutomatePerceptionMap automatePerceptionMap_;

    uint nRelevance_;

private:
    static MOS_IDManager idManager_;
};


#	include "MOS_AgentKnowledge.inl"

#endif // __MOS_AgentKnowledge_h_