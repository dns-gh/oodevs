// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_VisitableEntity_ABC_h_
#define __MIL_VisitableEntity_ABC_h_

template< typename T > class MIL_EntityVisitor_ABC;
class MIL_EntitiesVisitor_ABC;

// =============================================================================
/** @class  MIL_VisitableEntity_ABC
    @brief  Visitable Entity base class
*/
// Created: SBO 2006-02-24
// =============================================================================
template< typename T >
class MIL_VisitableEntity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_VisitableEntity_ABC() {}
    virtual ~MIL_VisitableEntity_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( MIL_EntityVisitor_ABC< T >& visitor ) const = 0;
    virtual void Apply( MIL_EntitiesVisitor_ABC& visitor ) const = 0;
    virtual void Apply( const std::function< void( const T& ) >& visitor ) const = 0;
    //@}
};

#endif // __MIL_VisitableEntity_ABC_h_
