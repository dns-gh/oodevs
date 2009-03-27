// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ObjectAttributesContainer_h_
#define __ObjectAttributesContainer_h_

#include "tools/SortedInterfaceContainer.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"


namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  ObjectAttributesContainer
    @brief  ObjectAttributesContainer
*/
// Created: JCR 2008-07-09
// =============================================================================
class ObjectAttributesContainer
    : public tools::SortedInterfaceContainer< kernel::Extension_ABC >
    , public kernel::Extension_ABC
    , public kernel::Serializable_ABC
    , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributesContainer();
    virtual ~ObjectAttributesContainer();
    //@}

    //! @name kernel::Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;    
    //@}

    //! @name kernel::Displayable_ABC
    //@{
    virtual void Display         ( kernel::Displayer_ABC& ) const;
    virtual void DisplayInList   ( kernel::Displayer_ABC& ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& ) const;
    //@}
};

#endif // __ObjectAttributesContainer_h_
