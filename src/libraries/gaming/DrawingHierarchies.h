// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingHierarchies_h_
#define __DrawingHierarchies_h_

#include "clients_kernel/TacticalHierarchies.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Drawing_ABC;
}

// =============================================================================
/** @class  DrawingHierarchies
    @brief  Drawing hierarchies
*/
// Created: LDC 2014-11-12
// =============================================================================
class DrawingHierarchies : public kernel::TacticalHierarchies
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingHierarchies( const kernel::Drawing_ABC& drawing );
    virtual ~DrawingHierarchies();

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
    const kernel::Drawing_ABC& drawing_;
};

#endif // __DrawingHierarchies_h_
