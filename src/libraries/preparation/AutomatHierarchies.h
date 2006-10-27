// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatHierarchies_h_
#define __AutomatHierarchies_h_

#include "TacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  AutomatHierarchies
    @brief  AutomatHierarchies
*/
// Created: SBO 2006-10-26
// =============================================================================
class AutomatHierarchies : public TacticalHierarchies
                         , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AutomatHierarchies();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatHierarchies( const AutomatHierarchies& );            //!< Copy constructor
    AutomatHierarchies& operator=( const AutomatHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}
};

#endif // __AutomatHierarchies_h_
