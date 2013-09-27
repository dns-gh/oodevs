// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Kernel_CommunicationHierarchies_h_
#define __Kernel_CommunicationHierarchies_h_

#include "Hierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  CommunicationHierarchies
    @brief  Communication hierarchies
            agent -> automat -> knowledge group -> team
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationHierarchies : public Hierarchies
{
public:
    //! @name Constructors/Destructor
    //@{
             CommunicationHierarchies() {};
    virtual ~CommunicationHierarchies() {};
    //@}

    //! @name Accessors
    //@{
    virtual bool CanCommunicate() const = 0;
    virtual const Entity_ABC* GetSuperior() const = 0;
    virtual const Entity_ABC& GetEntity() const = 0;
    virtual bool CanReceive() const = 0;
    virtual bool IsJammed() const = 0;
    virtual tools::Iterator< const Entity_ABC& > CreateSubordinateIterator() const = 0;
    //@}

    //! @name Modifiers
    //@{
    virtual void RegisterSubordinate  (       Entity_ABC& entity ) = 0;
    virtual void AddSubordinate       (       Entity_ABC& entity ) = 0;
    virtual void RemoveSubordinate    ( const Entity_ABC& entity ) = 0;
    virtual void UnregisterSubordinate( const Entity_ABC& entity ) = 0;
    virtual void UnregisterParent     () = 0;
    //@}

    //! @name Helpers
    //@{
    virtual const Hierarchies* RetrieveHierarchies( const Entity_ABC& entity ) const = 0;
    //@}

};

};

#endif // __Kernel_CommunicationHierarchies_h_
