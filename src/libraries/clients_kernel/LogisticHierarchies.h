// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_LogisticHierarchies_h_
#define __kernel_LogisticHierarchies_h_

#include "Hierarchies.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  LogisticHierarchiesBase
    @brief  LogisticHierarchiesBase
    // $$$$ AGE 2006-11-21: bouger dans prepa
*/
// Created: AGE 2006-11-21
// =============================================================================
class LogisticHierarchiesBase : public Hierarchies
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchiesBase() {};
    virtual ~LogisticHierarchiesBase() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "logistic-base"; };
    virtual void Load( xml::xistream& xis, const Entity_ABC* superior ) = 0;
    //@}
};

}

#endif // __kernel_LogisticHierarchies_h_
