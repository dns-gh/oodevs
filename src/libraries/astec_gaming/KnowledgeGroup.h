// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "ASN_Types.h"
#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/IDManager.h"
#include "astec_kernel/Resolver.h"

class Team;
class Agent_ABC;
class AgentKnowledge;
class Population;
class PopulationKnowledge;
class Controller;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
    @par    Holds the gtia's knowledge on other agents.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public Entity_ABC
                     , public Resolver< Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, Controller& controller, const Team& team );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors & Modifiers
    //@{
    bool IsInTeam( const Team& team ) const;
    const Team& GetTeam() const;
    virtual unsigned long GetId() const;
    virtual std::string GetName() const;
    void AddAutomat( unsigned long id, Agent_ABC& automat );
    void RemoveAutomat( unsigned long id );
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Team& team_;
    unsigned long      nID_;
    //@}

private:
    static IDManager     idManager_;
};

#endif // __KnowledgeGroup_h_
