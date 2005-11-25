// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataRecognition.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:01 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentDataRecognition.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentDataRecognition_h_
#define __DEC_Knowledge_AgentDataRecognition_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_AgentPerceptionDataRecognition;
class MIL_AgentType_ABC;
class MIL_Army;
class MIL_Agent_ABC;

class PHY_NatureWeapon;
class PHY_NatureAtlas;

struct ASN1T_MsgUnitKnowledgeUpdate;

// =============================================================================
// Created: NLD 2004-03-11
// Modified: JVT 2004-12-09
// =============================================================================
class DEC_Knowledge_AgentDataRecognition
{
    MT_COPYNOTALLOWED( DEC_Knowledge_AgentDataRecognition ) 

public:
     DEC_Knowledge_AgentDataRecognition();
    ~DEC_Knowledge_AgentDataRecognition();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare    (); 
    void Update     ( const DEC_Knowledge_AgentPerceptionDataRecognition& data );
    void Update     ( const DEC_Knowledge_AgentDataRecognition&           data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Network
    //@{
    bool HasChanged      () const;
    void SendChangedState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const;
    void SendFullState   ( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const;
    //@}

    //! @name Accessors
    //@{
          uint                         GetTimeLastUpdate  () const;
          MT_Float                     GetOperationalState() const;
    const T_KnowledgeComposanteVector& GetComposantes     () const;
    const MIL_Army*                    GetArmy            () const;
    const PHY_NatureWeapon&            GetNatureWeapon    () const;
    const PHY_NatureAtlas&             GetNatureAtlas     () const;
    const MIL_AgentType_ABC*           GetAgentType       () const;
          bool                         IsPC               () const;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void DoUpdate( const T& data );
    //@}

private:
          uint                         nTimeLastUpdate_;
          MT_Float                     rOperationalState_;
          T_KnowledgeComposanteVector  composantes_;
    const MIL_Army*                    pArmy_;
          bool                         bIsPC_;
    const MIL_AgentType_ABC*           pAgentType_; // For 'natures'

    // Network
    bool bOperationalStateChanged_;
    bool bAgentTypeUpdated_;
};

#include "DEC_Knowledge_AgentDataRecognition.inl"

#endif // __DEC_Knowledge_AgentDataRecognition_h_
