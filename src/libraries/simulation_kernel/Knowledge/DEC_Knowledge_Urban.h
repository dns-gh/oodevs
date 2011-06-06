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
#include "DEC_Knowledge_Object.h"
#include "Tools/MIL_IDManager.h"

class DEC_Knowledge_UrbanPerception;
class MIL_Army_ABC;
class MIL_Agent_ABC;
class MIL_Automate;
class UrbanObjectWrapper;

namespace sword
{
    class UrbanKnowledgeUpdate;
}

// =============================================================================
/** @class  DEC_Knowledge_Urban
    @brief  DEC_Knowledge_Urban
*/
// Created: MGD 2009-12-04
// =============================================================================
class DEC_Knowledge_Urban : public DEC_Knowledge_Object
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Urban( const MIL_Army_ABC& army, const UrbanObjectWrapper& wrapper );
             DEC_Knowledge_Urban();
    virtual ~DEC_Knowledge_Urban();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;
    virtual void Prepare();
    void Update( const DEC_Knowledge_UrbanPerception& perception );
    void ComputeProgress( const MIL_Agent_ABC& agent );
    bool Clean() const;
    float GetCurrentRecceProgress() const;
    unsigned int GetObjectKnownId() const;
    //@}

    //! @name Relevance management
    //@{
    virtual void UpdateRelevance();
    //@}

    //! @name Network operations
    //@{
    virtual void UpdateOnNetwork();
    virtual void SendStateToNewClient();
    //@}

private:
    //! @name Helpers
    //@{
    void SetProgress( float progress );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_Automate* >   T_PerceptionSource;
    typedef T_PerceptionSource::iterator        IT_PerceptionSource;
    typedef T_PerceptionSource::const_iterator CIT_PerceptionSource;

    typedef std::vector< const MIL_Agent_ABC* >       T_PerceptionAgentSource;
    typedef T_PerceptionAgentSource::iterator        IT_PerceptionAgentSource;
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
    void WriteMsgPerceptionSources( sword::UrbanKnowledgeUpdate& asnMsg ) const;
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    void SendChangedState();
    void SendFullState();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int armyId_;
    unsigned int objectId_;
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
