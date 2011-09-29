// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Knowledge_ABC_h_
#define __Knowledge_ABC_h_

#include "Entity_ABC.h"
#include "KnowledgeListener_ABC.h"
#include <set>

namespace kernel
{
    class Team_ABC;

// =============================================================================
/** @class  Knowledge_ABC
    @brief  Knowledge_ABC
*/
// Created: SBO 2006-10-12
// =============================================================================
class Knowledge_ABC : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Knowledge_ABC() {}
    virtual ~Knowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetEntity() const = 0;
    virtual unsigned long GetEntityId() const = 0;
    virtual const Team_ABC* GetTeam() const = 0;
    virtual const Entity_ABC& GetOwner() const = 0;

    void AddListener( KnowledgeListener_ABC& listener ) { listeners_.insert( &listener ); }
    void RemoveListener( KnowledgeListener_ABC& listener ) { listeners_.erase( &listener ); }

    virtual std::string GetSymbol() const { return ""; } // $$$$ SBO 2007-02-26:
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Knowledge_ABC( const Knowledge_ABC& );
    Knowledge_ABC& operator=( const Knowledge_ABC& );
    //@}

private:
    //! @name Members
    //@{
    std::set< KnowledgeListener_ABC* > listeners_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC destructor
// Created: JSR 2010-05-19
// -----------------------------------------------------------------------------
inline
Knowledge_ABC::~Knowledge_ABC()
{
    for( std::set< KnowledgeListener_ABC* >::const_iterator it = listeners_.begin(); it != listeners_.end(); ++it )
        ( *it )->NotifyDestruction();
}

}

#endif // __Knowledge_ABC_h_
