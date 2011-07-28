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
#include "Tristate.h"
#include "DEC_Knowledge_ABC.h"
#include "DEC_Knowledge_IObjectAttributeProxy.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/Extendable.h"
#include "Tools/MIL_IDManager.h"

namespace sword
{
    class ObjectKnowledgeUpdate;
}

class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectPerception;
class MIL_Agent_ABC;
class MIL_AgentType_ABC;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_KnowledgeGroup;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;

// =============================================================================
/** @class  DEC_Knowledge_Object
    @brief  This class represents the knowledge of an object
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_Object : public DEC_Knowledge_ABC
                           , public tools::Extendable< DEC_Knowledge_IObjectAttributeProxy >
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Object( const MIL_Army_ABC& armyKnowing, MIL_Object_ABC& objectKnown, bool sendCreation = true );
             DEC_Knowledge_Object( const MIL_KnowledgeGroup& groupKnowing, MIL_Object_ABC& objectKnown );
             DEC_Knowledge_Object( const DEC_Knowledge_Object& copy, const MIL_KnowledgeGroup* pGroupKnowing );
             DEC_Knowledge_Object();
    virtual ~DEC_Knowledge_Object();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Collision
    //@{
    bool CanCollideWith( const MIL_Agent_ABC& agent ) const;
    bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    //@}

    //! @name Operations
    //@{
    void Recon( const MIL_Agent_ABC& agent );
    virtual void Prepare();
    void Update( const PHY_PerceptionLevel& currentPerceptionLevel );
    void Update( const DEC_Knowledge_ObjectPerception& perception );
    void Update( const DEC_Knowledge_ObjectCollision& collision );
    bool Clean() const;
    bool IsValid() const;
    void Invalidate();
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    bool IsReconBy( const MIL_AgentType_ABC& agentType ) const;
    bool IsObjectInsidePathPoint( const T_PointVector& pathPoints, const MIL_Agent_ABC& agent ) const;
    void HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel );
    bool IsPerceptionDistanceHacked() const;
    //@}

    //! @name Relevance management
    //@{
    virtual void UpdateRelevance();
    double GetRelevance() const;
    //@}

    //! @name Network operations
    //@{
    virtual void UpdateOnNetwork();
    virtual void SendStateToNewClient();
    //@}

    //! @name Accessors
    //@{
    bool IsRecon() const;
    bool IsBypassed() const;
    bool IsConstructed() const;
    bool IsReservedObstacle() const;
    bool IsReservedObstacleActivated() const;
    virtual unsigned int GetID() const;
    const std::string& GetName() const;
    MIL_Object_ABC* GetObjectKnown() const;
    const MIL_ObjectType_ABC& GetType() const;
    const TER_Localisation& GetLocalisation() const;
    const TER_Localisation& GetAvoidanceLocalisation() const;
    const MIL_Army_ABC& GetArmy() const;
    double GetMaxInteractionHeight() const;
    E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const;
    E_Tristate IsAFriend( const MIL_Army_ABC& army ) const;

    template< typename T >
    const T* RetrieveAttribute() const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_Attributes
    {
        eAttr_Nothing                = 0x00000000,
        eAttr_Localisation           = 0x00000001,
        eAttr_RealObject             = 0x00000040,
        eAttr_PerceptionSources      = 0x00000080,
        eAttr_Relevance              = 0x00000100,
        eAttr_CurrentPerceptionLevel = 0x00000200,
        eAttr_MaxPerceptionLevel     = 0x00000400,
        eAttr_Attributes             = 0x00000800,
        eAttr_AllAttributes          = 0xFFFFFFFF
    };
    //@}

private:
    //! @name Tools
    //@{
    void NotifyAttributeUpdated( E_Attributes nAttribute );
    bool IsAttributeUpdated( E_Attributes nAttribute ) const;
    const MIL_Army_ABC& GetArmyKnowing() const;
    //@}

    //! @name Internal updaters
    //@{
    void UpdateLocalisations();
    void UpdateLocalisationPartially( const DEC_Knowledge_ObjectCollision& collision );
    void UpdatePerceptionSources( const DEC_Knowledge_ObjectPerception& perception );
    void UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    bool UpdateMaxPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    template< typename Functor> void UpdateAttributes( Functor functor );
    //@}

    //! @name Internal network senders
    //@{
    void BuildMsgRealObject( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgPerceptionSources( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgRelevance( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgLocalisations( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgCurrentPerceptionLevel( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    void BuildMsgAttributes( sword::ObjectKnowledgeUpdate& asnMsg ) const;
    virtual void SendMsgCreation() const;
    virtual void SendMsgDestruction() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Automate* >               T_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::iterator        IT_PerceptionSourceSet;
    typedef T_PerceptionSourceSet::const_iterator CIT_PerceptionSourceSet;

    typedef std::map< const MIL_Agent_ABC*, const PHY_PerceptionLevel* > T_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::iterator                        IT_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::const_iterator                 CIT_PerceptionAgentSourceMap;

    typedef std::set< const MIL_AgentType_ABC* > T_AgentTypeSet;
    typedef T_AgentTypeSet::const_iterator     CIT_AgentTypeSet;
    //@}

private:
    //! @name Data Members
    //@{
    const MIL_Army_ABC* pArmyKnowing_;
    const MIL_KnowledgeGroup* pGroupKnowing_;
    MIL_Object_ABC* pObjectKnown_; // Objet réel (peut ne plus exister...)
    const MIL_ObjectType_ABC* pObjectType_;
    const unsigned int  nID_;
    std::string name_;
    int nAttributesUpdated_;
    // Attributes
    const MIL_Army_ABC* pOwnerArmy_;
    TER_Localisation localisation_;
    TER_Localisation avoidanceLocalisation_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pMaxPerceptionLevel_;
    T_PerceptionSourceSet perceptionPerAutomateSet_;
    T_PerceptionSourceSet previousPerceptionPerAutomateSet_;
    T_PerceptionAgentSourceMap perceptionLevelPerAgentMap_;
    unsigned int nTimeLastUpdate_;
    double rRelevance_;
    bool bValid_;
    T_AgentTypeSet reconByAgentTypes_;
    static MIL_IDManager idManager_;
    bool bPerceptionDistanceHacked_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_Object )

// =============================================================================
// Implementation
// =============================================================================
template< typename Functor>
void DEC_Knowledge_Object::UpdateAttributes( Functor functor )
{
    for( std::vector< DEC_Knowledge_IObjectAttributeProxy* >::const_iterator it = extensions_.Container().begin(); it != extensions_.Container().end(); ++it )
        if( *it && functor( *it ) )
            NotifyAttributeUpdated( eAttr_Attributes );
}

template< typename T >
const T* DEC_Knowledge_Object::RetrieveAttribute() const
{
    const DEC_Knowledge_ObjectAttributeProxy_ABC< T >* pAttr = Retrieve< DEC_Knowledge_ObjectAttributeProxy_ABC< T > >();
    if( !pAttr )
        return 0;
    return pAttr->GetAttribute();
}

#endif // __DEC_Knowledge_Object_h_
