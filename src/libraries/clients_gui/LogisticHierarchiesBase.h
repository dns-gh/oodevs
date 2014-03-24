// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticHierarchiesBase_h_
#define __LogisticHierarchiesBase_h_

#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/SubTypes.h"

namespace gui
{

// =============================================================================
/** @class  LogisticHierarchiesBase
    @brief  LogisticHierarchiesBase
*/
// Created: AGE 2006-11-21
// =============================================================================
class LogisticHierarchiesBase : public kernel::Hierarchies
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchiesBase() {}
    virtual ~LogisticHierarchiesBase() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "logistic-base"; }
    virtual void Load( xml::xistream& xis, const kernel::Entity_ABC* superior ) = 0;
    virtual void SetLogisticSuperior( const kernel::LogisticBaseSuperior& superior ) = 0;
    //@}
};

}; //! namespace kernel

#endif // __LogisticHierarchiesBase_h_
