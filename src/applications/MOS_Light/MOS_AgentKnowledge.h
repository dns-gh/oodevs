//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentKnowledge.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:30 $
// $Revision: 17 $
// $Workfile: MOS_AgentKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_AgentKnowledge_h_
#define __MOS_AgentKnowledge_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_IDManager.h"
#include "MOS_Perception_Def.h"

class MOS_AgentKnowledge_Editor;
class MOS_Agent;

//*****************************************************************************
// Created: AGN 03-04-03
//*****************************************************************************
class MOS_AgentKnowledge
{
    MT_COPYNOTALLOWED( MOS_AgentKnowledge )
        
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
        eUpdated_MissionCapacity         = 0x00010000,
        eUpdated_Prisoner                = 0x00020000,
        eUpdated_Surrendered             = 0x00040000,
        eUpdated_RefugeesManaged         = 0x00080000,
    };

    friend class MOS_AgentKnowledge_Editor;

public:
     MOS_AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& asnMsg );
    ~MOS_AgentKnowledge();

    //! @name Events
    //@{
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg );
    //@}

    //! @name Editor
    //@{
    void SetEditor( MOS_AgentKnowledge_Editor* pEditor );
    //@}

    //! @name Accessors
    //@{
    uint               GetID       () const;
    uint               GetAgentID  () const;
    bool               IsValid     ( E_AttributeUpdated ) const;
    const MT_Vector2D& GetPosition () const;
    std::string        GetName() const;
    //@}

    void Draw();

private:
    //! @name Types
    //@{
    typedef std::map< MOS_Agent*, E_PerceptionResult > T_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::iterator          IT_AutomatePerceptionMap;
    typedef T_AutomatePerceptionMap::const_iterator    CIT_AutomatePerceptionMap;
    //@}

private:
    MOS_AgentKnowledge_Editor* pEditor_;

    uint nAttrUpdated_;

    uint              nID_;
    MOS_Agent*        pAgent_;
    MT_Vector2D       vPosition_;
    std::string       strPosition_;
    uint              nDirection_;
    uint              nSpeed_;
    uint              nEtatOps_;
    E_PerceptionResult nCurrentPerceptionLevel_;
    E_PerceptionResult nMaxPerceptionLevel_;

    uint              nTeam_;

    E_NatureLevel               nLevel_;
    E_UnitNatureWeapon          nWeapon_;
    E_UnitNatureSpecialization  nSpecialization_;
    E_UnitNatureQualifier       nQualifier_;
    E_UnitNatureCategory        nCategory_;
    E_UnitNatureMobility        nMobility_;
    E_UnitCapaciteMission       nMissionCapacity_;
    bool                        bIsPC_;
    bool                        bSurrendered_;
    bool                        bPrisoner_;
    bool                        bRefugeesManaged_;

    T_AutomatePerceptionMap automatePerceptionMap_;

    uint nRelevance_;

private:
    static MOS_IDManager idManager_;
};

typedef std::vector< MOS_AgentKnowledge* >      T_AgentKnowledgeVector;
typedef T_AgentKnowledgeVector::iterator        IT_AgentKnowledgeVector;
typedef T_AgentKnowledgeVector::const_iterator  CIT_AgentKnowledgeVector;

#ifdef MOS_USE_INLINE
#	include "MOS_AgentKnowledge.inl"
#endif


#endif // __MOS_AgentKnowledge_h_