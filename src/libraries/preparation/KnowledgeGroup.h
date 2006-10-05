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

#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::KnowledgeGroup_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( kernel::Controller& controller, const kernel::Team_ABC& team, IdManager& idManager );
             KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, const kernel::Team_ABC& team, IdManager& idManager );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors & Modifiers
    //@{
    virtual const kernel::Team_ABC& GetTeam() const;
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    virtual void AddAutomat( unsigned long id, kernel::Agent_ABC& automat );
    virtual void RemoveAutomat( unsigned long id );
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
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
    kernel::Controller& controller_;
    const kernel::Team_ABC& team_;
    unsigned long id_;
    QString name_;
    QString type_;
    //@}
};

#endif // __KnowledgeGroup_h_
