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
#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include "game_asn/Simulation.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/Extendable.h"

class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectPerception;
class MIL_Agent_ABC;
class MIL_AgentType_ABC;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;

// =============================================================================
/** @class  DEC_Knowledge_Object
    @brief  This class represents the knowledge of an object
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_Object : public DEC_Knowledge_ABC
                           , private tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Object( const MIL_Army_ABC& armyKnowing, MIL_Object_ABC& objectKnown );
             DEC_Knowledge_Object();
    virtual ~DEC_Knowledge_Object();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Collision
    //@{
    bool CanCollideWith( const MIL_Agent_ABC& agent ) const;
    //@}

    //! @name Operations
    //@{
    void Recon  ( const MIL_Agent_ABC& agent );
    void Prepare();

    void Update( const PHY_PerceptionLevel& currentPerceptionLevel );
    void Update( const DEC_Knowledge_ObjectPerception& perception );
    void Update( const DEC_Knowledge_ObjectCollision& collision  );

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
          bool                IsRecon                    () const;
          bool                IsBypassed                 () const;
          bool                IsConstructed              () const;
          bool                IsReservedObstacle         () const;
          bool                IsReservedObstacleActivated() const;
          uint                GetID                      () const;
          MIL_Object_ABC*     GetObjectKnown             () const;
    const MIL_ObjectType_ABC& GetType                    () const;
    const TER_Localisation&   GetLocalisation            () const;
    const TER_Localisation&   GetAvoidanceLocalisation   () const;
    const MIL_Army_ABC&       GetArmy                    () const;    
          MT_Float            GetMaxInteractionHeight    () const;
    //@}

    //! @name Extension
    //@{
    template< typename Extension >  void              Attach( Extension& extension );
    template< typename Extension >  void              AttachExtension( Extension& extension );
    template< typename Extension >  const Extension*  Retrieve() const;
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
        eAttr_ReservedObstacleActivated      = 0x00000020,
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
    //! @name Tools
    //@{
          void      NotifyAttributeUpdated( E_Attributes nAttribute );
          bool      IsAttributeUpdated    ( E_Attributes nAttribute ) const;
          bool      IsReconBy             ( const MIL_AgentType_ABC& agentType ) const;
    const MIL_Army_ABC& GetArmyKnowing    () const;
    //@}

private:
    //! @name Internal updaters
    //@{
    void UpdateLocalisations         ();
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
    void BuildMsgAttributes            ( ASN1T_MsgObjectKnowledgeUpdate& asnMsg ) const;

    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Automate* >               T_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::iterator         IT_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::const_iterator   CIT_PerceptionSourceSet;

    typedef std::set< const MIL_AgentType_ABC* >    T_AgentTypeSet;
    typedef T_AgentTypeSet::const_iterator          CIT_AgentTypeSet;

    typedef std::vector< DEC_Knowledge_ObjectAttribute_ABC* >   T_ObjectAttributeVector;
    typedef T_ObjectAttributeVector::iterator                   IT_ObjectAttributeVector;
    typedef T_ObjectAttributeVector::const_iterator             CIT_ObjectAttributeVector;
    //@}

private:
    const MIL_Army_ABC*       pArmyKnowing_;
          MIL_Object_ABC*     pObjectKnown_; // Objet réel (peut ne plus exister...)
    const MIL_ObjectType_ABC* pObjectType_; 
    const uint                nID_;

    T_ObjectAttributeVector   attributes_;

    int nAttributesUpdated_;

    // Attributes
    const MIL_Army_ABC* pOwnerArmy_;
    TER_Localisation  localisation_;
    TER_Localisation  avoidanceLocalisation_;

    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pMaxPerceptionLevel_;
    
    T_PerceptionSourceSet  perceptionPerAutomateSet_;
    T_PerceptionSourceSet  previousPerceptionPerAutomateSet_;

    uint     nTimeLastUpdate_;
    MT_Float rRelevance_;

    T_AgentTypeSet reconByAgentTypes_;
};

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Attach
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
template< typename Extension >  
void DEC_Knowledge_Object::Attach( Extension& extension )
{
    attributes_.push_back( &extension );
    tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >::Attach( extension );
}

// -----------------------------------------------------------------------------
// Name: template< typename Extension >  void DEC_Knowledge_Object::AttachExtension
// Created: LDC 2009-07-16
// -----------------------------------------------------------------------------
template< typename Extension >
void DEC_Knowledge_Object::AttachExtension( Extension& extension )
{
    tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >::Attach( extension );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::Retrieve
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
template< typename Extension >  
const Extension* DEC_Knowledge_Object::Retrieve() const
{
    return tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >::Retrieve< Extension >();
}

#endif // __DEC_Knowledge_Object_h_
