// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineHierarchies_h_
#define __TacticalLineHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

// =============================================================================
/** @class  TacticalLineHierarchies
    @brief  TacticalLineHierarchies
*/
// Created: SBO 2006-10-19
// =============================================================================
class TacticalLineHierarchies : public kernel::EntityHierarchies< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TacticalLineHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLineHierarchies( const TacticalLineHierarchies& );            //!< Copy constructor
    TacticalLineHierarchies& operator=( const TacticalLineHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __TacticalLineHierarchies_h_
