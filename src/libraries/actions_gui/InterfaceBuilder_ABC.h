// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InterfaceBuilder_ABC_h_
#define __InterfaceBuilder_ABC_h_

namespace actions {
namespace gui {

// =============================================================================
/** @class  InterfaceBuilder_ABC
    @brief  InterfaceBuilder_ABC
*/
// Created: LDC 2010-08-18
// =============================================================================
class InterfaceBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             InterfaceBuilder_ABC() {}
    virtual ~InterfaceBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual actions::gui::Param_ABC& Build( const kernel::OrderParameter& parameter, bool isRegistered = true ) const = 0;
    //@}
};

}
}

#endif // __InterfaceBuilder_ABC_h_
