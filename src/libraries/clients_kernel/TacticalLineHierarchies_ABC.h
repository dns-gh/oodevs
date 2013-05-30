// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineHierarchies_ABC_h_
#define __TacticalLineHierarchies_ABC_h_

#include "SimpleHierarchies.h"
#include "TacticalHierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  TacticalLineHierarchies_ABC
    @brief  TacticalLineHierarchies_ABC
*/
// Created: MMC 2012-05-14
// =============================================================================
class TacticalLineHierarchies_ABC : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
{
protected:
    //! @name Constructors/Destructor
    //@{
             TacticalLineHierarchies_ABC( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~TacticalLineHierarchies_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;
    //@}
};

}

#endif // __TacticalLineHierarchies_ABC_h_
