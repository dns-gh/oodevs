// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "ASN_Types.h"
#include "astec_kernel/KnowledgeGroup_ABC.h"
#include "astec_kernel/IDManager.h"

class Controller;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public KnowledgeGroup_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, Controller& controller, const Team_ABC& team );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors & Modifiers
    //@{
    virtual bool IsInTeam( const Team_ABC& team ) const;
    virtual const Team_ABC& GetTeam() const;
    virtual unsigned long GetId() const;
    virtual std::string GetName() const;
    virtual void AddAutomat( unsigned long id, Agent_ABC& automat );
    virtual void RemoveAutomat( unsigned long id );
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
    const Team_ABC& team_;
    unsigned long      nID_;
    //@}

private:
    static IDManager     idManager_;
};

#endif // __KnowledgeGroup_h_
