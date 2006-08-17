// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Object.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 7 $
// $Workfile: DEC_Knowledge_Object.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Object_h_
#define __DEC_Knowledge_Object_h_

#include "MIL.h"

#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "DEC_Knowledge_ABC.h"
#include "Network/NET_ASN_Types.h"
#include "simulation_terrain/TER_Localisation.h"

class MIL_Army;
class MIL_RealObject_ABC;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectPerception;
class MIL_RealObjectType;
class MIL_Automate;
class MIL_AgentType_ABC;

// =============================================================================
/** @class  DEC_Knowledge_Object
    @brief  This class represents the knowledge of an object
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_Object : public DEC_Knowledge_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_Object ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Object( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
             DEC_Knowledge_Object();
    virtual ~DEC_Knowledge_Object();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Recon  ( const MIL_Agent_ABC& agent );
    void Prepare();

    void Update( const PHY_PerceptionLevel& currentPerceptionLevel );
    void Update( const DEC_Knowledge_ObjectPerception& perception );
    void Update( const DEC_Knowledge_ObjectCollision&  collision  );

    bool Clean() const;
    //@}

    //! @name Relevance management
    //@{
    void     UpdateRelevance();
    MT_Float GetRelevance   () const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     ();
    void SendStateToNewClient();
    //@}

    //! @name Accessors
    //@{
          bool                IsRecon                 () const;
          bool                IsBypassed              () const;
          bool                IsPrepared              () const;
          uint                GetID                   () const;
          MIL_RealObject_ABC* GetObjectKnown          () const;
    const MIL_RealObjectType& GetType                 () const;
    const TER_Localisation&   GetLocalisation         () const;
    const TER_Localisation&   GetAvoidanceLocalisation() const;
    const MIL_Army&           GetArmy                 () const;    
          MT_Float            GetMaxInteractionHeight () const;
    //@}
    
protected:
    //! @name Types
    //@{
    enum E_Attributes
    {
        eAttr_Nothing                        = 0x00000000,           
        eAttr_Localisation                   = 0x00000001,
        eAttr_EtatOps                        = 0x00000002,
        eAttr_ConstructionPercentage         = 0x00000004,
        eAttr_MiningPercentage               = 0x00000008,
        eAttr_BypassPercentage               = 0x00000010,
        eAttr_IsPrepared                     = 0x00000020,
        eAttr_RealObject                     = 0x00000040,
        eAttr_PerceptionSources              = 0x00000080,
        eAttr_Relevance                      = 0x00000100,
        eAttr_CurrentPerceptionLevel         = 0x00000200,
        eAttr_MaxPerceptionLevel             = 0x00000400,
        eAttr_Specific                       = 0x00000800,
        eAttr_Dotations                      = 0x00001000,
        eAttr_AllAttributes                  = 0xFFFFFFFF
    };
    //@}

protected:
    //! @name Update
    //@{
    virtual void UpdateSpecificAttributes( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision&  collision  );

    virtual void BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    //@}

    //! @name Tools
    //@{
    void NotifyAttributeUpdated( E_Attributes nAttribute );
    bool IsAttributeUpdated    ( E_Attributes nAttribute ) const;
    bool IsReconBy             ( const MIL_AgentType_ABC& agentType ) const;
    //@}

private:
    //! @name Internal updaters
    //@{
    void UpdateLocalisations         ();
    void UpdateStates                ();
    void UpdatePerceptionSources     ( const DEC_Knowledge_ObjectPerception& perception );
    void UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    bool UpdateMaxPerceptionLevel    ( const PHY_PerceptionLevel& perceptionLevel );
    //@}
    
    //! @name Internal network senders
    //@{
    void BuildMsgRealObject            ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgPerceptionSources     ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgRelevance             ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgLocalisations         ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgCurrentPerceptionLevel( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgStates                ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;

    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Automate* >               T_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::iterator         IT_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::const_iterator   CIT_PerceptionSourceSet;

    typedef std::set< const MIL_AgentType_ABC* > T_AgentTypeSet;
    typedef T_AgentTypeSet::const_iterator       CIT_AgentTypeSet;
    //@}

private:
    const MIL_Army*           pArmyKnowing_;
          MIL_RealObject_ABC* pObjectKnown_; // Objet réel (peut ne plus exister...)

    const MIL_RealObjectType* pObjectType_;
    const uint                nID_;

    int nAttributesUpdated_;

    // Attributes
    const MIL_Army*   pOwnerArmy_;
    TER_Localisation  localisation_;
    TER_Localisation  avoidanceLocalisation_;
    uint              nConstructionPercentage_;
    uint              nMiningPercentage_;
    uint              nBypassPercentage_;
    bool              bIsPrepared_;
    uint              nNbrDotationForConstruction_;
    uint              nNbrDotationForMining_;            

    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pMaxPerceptionLevel_;
    
    T_PerceptionSourceSet  perceptionPerAutomateSet_;
    T_PerceptionSourceSet  previousPerceptionPerAutomateSet_;

    uint     nTimeLastUpdate_;
    MT_Float rRelevance_;

    T_AgentTypeSet reconByAgentTypes_;
};

#include "DEC_Knowledge_Object.inl"

#endif // __DEC_Knowledge_Object_h_
