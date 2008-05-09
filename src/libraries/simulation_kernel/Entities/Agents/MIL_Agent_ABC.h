// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_Agent_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_Agent_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Agent_ABC_h_
#define __MIL_Agent_ABC_h_

#include "MIL.h"

#include "Entities/MIL_Entity_ABC.h"

class MIL_Army;
class MIL_KnowledgeGroup;
class MIL_AgentType_ABC;
class MIL_AgentPion;
class MIL_Population;
class DEC_Knowledge_Agent;

// =============================================================================
// @class  MIL_Agent_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Agent_ABC : public MIL_Entity_ABC
{
    MT_COPYNOTALLOWED( MIL_Agent_ABC )

public:
             MIL_Agent_ABC( uint nID );
             MIL_Agent_ABC();
    virtual ~MIL_Agent_ABC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Accessors
    //@{
            uint                      GetID            () const;
    virtual       MIL_Army&           GetArmy          () const = 0;
    virtual const MIL_AgentType_ABC&  GetType          () const = 0;
    virtual bool                      IsDead           () const = 0;
    virtual bool                      IsNeutralized    () const = 0;
    virtual bool                      IsPC             () const = 0;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void NotifyAttackedBy( MIL_AgentPion& pion ) = 0;
    virtual void NotifyAttackedBy( MIL_Population& population ) = 0;
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Agent& CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup ) = 0;
    virtual bool                 IsPerceived    ( const MIL_Agent_ABC& target ) const = 0; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    //@}

    //! @name Operators
    //@{
    bool operator == ( const MIL_Agent_ABC& rhs ) const;
    bool operator != ( const MIL_Agent_ABC& rhs ) const;
    //@}

private:
    uint nID_;
};

#include "MIL_Agent_ABC.inl"

#endif // __MIL_Agent_ABC_h_
