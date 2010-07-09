// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleHierarchies_h_
#define __SimpleHierarchies_h_

#include "Hierarchies.h"

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  SimpleHierarchies
    @brief  Simple hierarchies
*/
// Created: AGE 2006-11-24
// =============================================================================
template< typename I >
class SimpleHierarchies : public I
{
public:
    //! @name Constructors/Destructor
    //@{
             SimpleHierarchies( const Entity_ABC& holder, const Entity_ABC* superior );
    virtual ~SimpleHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetSuperior() const;
    virtual const Entity_ABC& GetEntity() const;
    virtual tools::Iterator< const Entity_ABC& > CreateSubordinateIterator() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void RegisterSubordinate  (       Entity_ABC& entity );
    virtual void AddSubordinate       (       Entity_ABC& entity );
    virtual void RemoveSubordinate    ( const Entity_ABC& entity );
    virtual void UnregisterSubordinate( const Entity_ABC& entity );
    virtual void UnregisterParent     ();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const Hierarchies* RetrieveHierarchies( const Entity_ABC& entity ) const;
    void SetSuperior( const Entity_ABC* superior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleHierarchies( const SimpleHierarchies& );            //!< Copy constructor
    SimpleHierarchies& operator=( const SimpleHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Entity_ABC& holder_;
    const Entity_ABC* superior_;
    //@}
};

}

#include "SimpleHierarchies.inl"

#endif // __SimpleHierarchies_h_
