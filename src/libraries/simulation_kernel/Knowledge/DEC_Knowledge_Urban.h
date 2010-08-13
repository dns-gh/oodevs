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
    void ComputeProgress( const MIL_Agent_ABC& agent );
    void SetProgress( float progress );
    bool Clean() const;
    unsigned GetId() const;
    float GetCurrentRecceProgress() const;
    bool IsPerceivedBy( const MIL_Agent_ABC& pion ) const;
    const float GetPathfindCost( float weight ) const;
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
    typedef std::vector< const MIL_Automate* >  T_PerceptionSource;
    typedef T_PerceptionSource::iterator        IT_PerceptionSource;
    typedef T_PerceptionSource::const_iterator  CIT_PerceptionSource;

    typedef std::vector< const MIL_Agent_ABC* >     T_PerceptionAgentSource;
    typedef T_PerceptionAgentSource::iterator       IT_PerceptionAgentSource;
    typedef T_PerceptionAgentSource::const_iterator CIT_PerceptionAgentSource;
    //@}

private:
    //! @name Tools
    //@{
    void ChangeRelevance( float rNewRelevance );
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
    // Internal attribute
    float rProgressPercent_;
    float rMaxProgressPercent_;
    T_PerceptionSource perceivedByAutomate_;
    T_PerceptionSource previousPerceivedByAutomate_;
    T_PerceptionAgentSource perceivedByAgent_;
    // Network
    bool bCreatedOnNetwork_;
    unsigned int nTimeLastUpdate_;
    bool bLastPerceived_;
    bool bCurrentProgressUpdated_;
    bool bMaxProgressUpdated_;
    float rLastProgressSent_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_Urban )

#endif // __DEC_Knowledge_Urban_h_
