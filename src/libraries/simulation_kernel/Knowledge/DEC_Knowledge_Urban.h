// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Urban_h_
#define __DEC_Knowledge_Urban_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Tools/MIL_IDManager.h"
#include <geometry/Types.h>

class DEC_Knowledge_UrbanPerception;
class MIL_Army_ABC;
class MIL_Agent_ABC;
class MIL_Automate;

namespace urban
{
    class TerrainObject_ABC;
}

namespace MsgsSimToClient
{
    class MsgUrbanKnowledgeUpdate;
}

// =============================================================================
/** @class  DEC_Knowledge_Urban
    @brief  DEC_Knowledge_Urban
*/
// Created: MGD 2009-12-04
// =============================================================================
class DEC_Knowledge_Urban : public DEC_Knowledge_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Urban( const MIL_Army_ABC& army, const urban::TerrainObject_ABC& object );
             DEC_Knowledge_Urban();
    virtual ~DEC_Knowledge_Urban();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;
    void Prepare();
    void Update( const DEC_Knowledge_UrbanPerception& perception );
    void SetPerceptionLevel( const PHY_PerceptionLevel& level );
    bool Clean() const;
    unsigned GetId() const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const;
    const float GetPathfindCost( float weight ) const;
    double GetProgress() const;
    const geometry::Point2f GetBarycenter() const;
    const urban::TerrainObject_ABC& GetTerrainObjectKnown() const;
    //@}

    //! @name Relevance management
    //@{
    void UpdateRelevance();
    MT_Float GetRelevance() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork();
    void SendStateToNewClient();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Automate*, const PHY_PerceptionLevel* > T_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::iterator                            IT_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::const_iterator                     CIT_PerceptionSourceMap;

    typedef std::map< const MIL_Agent_ABC*, const PHY_PerceptionLevel* > T_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::iterator                        IT_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::const_iterator                 CIT_PerceptionAgentSourceMap;
    //@}

private:
    //! @name Tools
    //@{
    void ChangeRelevance( MT_Float rNewRelevance );
    void UpdatePerceptionSources( const DEC_Knowledge_UrbanPerception& perception );
    //@}

    //! @name Network operations
    //@{
    void WriteMsgPerceptionSources( MsgsSimToClient::MsgUrbanKnowledgeUpdate& asnMsg ) const;
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    void SendChangedState();
    void SendFullState();
    //@}

private:
    //! @name Member data
    //@{
    const unsigned nID_;
    const MIL_Army_ABC* army_;
    const urban::TerrainObject_ABC* object_;
    // Internal attributes
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pMaxPerceptionLevel_;
    double rProgressPercent_;
    double rRelevance_;
    T_PerceptionSourceMap perceptionLevelPerAutomateMap_;
    T_PerceptionAgentSourceMap perceptionLevelPerAgentMap_;
    // Network
    bool bCreatedOnNetwork_;
    unsigned int nTimeLastUpdate_;
    bool bLastPerceived_;
    bool bRelevanceUpdated_;
    bool bCurrentPerceptionLevelUpdated_;
    bool bMaxPerceptionLevelUpdated_;
    double rLastRelevanceSent_;
    double rLastProgressSent_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_Urban )

#endif // __DEC_Knowledge_Urban_h_
