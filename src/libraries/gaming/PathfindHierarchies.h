// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PathfindHierarchies_h_
#define __PathfindHierarchies_h_

#include "clients_kernel/TacticalHierarchies.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Pathfind_ABC;
}

// =============================================================================
/** @class  PathfindHierarchies
    @brief  Pathfind hierarchies
*/
// Created: LDC 2014-11-07
// =============================================================================
class PathfindHierarchies : public kernel::TacticalHierarchies
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PathfindHierarchies( const kernel::Pathfind_ABC& pathfind );
    virtual ~PathfindHierarchies();

    virtual const kernel::Entity_ABC& GetEntity() const;    
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual tools::Iterator< const kernel::Entity_ABC& > CreateSubordinateIterator() const;
    virtual void RegisterSubordinate( kernel::Entity_ABC& entity );
    virtual void AddSubordinate( kernel::Entity_ABC& entity );
    virtual void RemoveSubordinate( const kernel::Entity_ABC& entity );
    virtual void UnregisterSubordinate( const kernel::Entity_ABC& entity );
    virtual void UnregisterParent();
    virtual const kernel::Hierarchies* RetrieveHierarchies( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    const kernel::Pathfind_ABC& pathfind_;
};

#endif // __PathfindHierarchies_h_
