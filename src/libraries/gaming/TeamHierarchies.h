// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamHierarchies_h_
#define __TeamHierarchies_h_

#include "clients_gui/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
}

namespace sword
{
    class KnowledgeGroupUpdate;
}

// =============================================================================
/** @class  TeamHierarchies
    @brief  TeamHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class TeamHierarchies : public gui::EntityHierarchies< kernel::CommunicationHierarchies >
                      , public kernel::Updatable_ABC< sword::KnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamHierarchies( kernel::Controller& controller, kernel::Team_ABC& holder );
    virtual ~TeamHierarchies();
    //@}

    virtual bool CanCommunicate() const;
    virtual bool CanReceive() const;
    virtual bool IsJammed() const;

private:
    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::KnowledgeGroupUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamHierarchies( const TeamHierarchies& );            //!< Copy constructor
    TeamHierarchies& operator=( const TeamHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Team_ABC& holder_;
    //@}
};

#endif // __TeamHierarchies_h_
