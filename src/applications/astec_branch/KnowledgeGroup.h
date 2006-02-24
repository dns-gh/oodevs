// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.h $
// $Author: Ape $
// $Modtime: 23/11/04 18:22 $
// $Revision: 8 $
// $Workfile: KnowledgeGroup.h $
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "ASN_Types.h"
#include "Entity_ABC.h"
#include "IDManager.h"
#include "Resolver.h"

class Team;
class Agent;
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
                     , public Resolver< Agent >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( uint32 nId, Controller& controller, const Team& team );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors & Modifiers
    //@{
    const Team& GetTeam() const;
    unsigned long GetId() const;
    std::string GetName() const;
    void AddAutomat( unsigned long id, Agent& automat );
    void RemoveAutomat( unsigned long id );
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
    uint32      nID_;
    //@}

private:
    static IDManager     idManager_;
};

#endif // __KnowledgeGroup_h_
