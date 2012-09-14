// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __StandardModelVisitor_ABC_h_
#define __StandardModelVisitor_ABC_h_

namespace gui
{

// =============================================================================
/** @class  StandardModelVisitor_ABC
    @brief  StandardModelVisitor_ABC
*/
// Created: JSR 2012-09-14
// =============================================================================
class StandardModelVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StandardModelVisitor_ABC() {}
    virtual ~StandardModelVisitor_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void Visit( QStandardItem& item ) = 0;
    //@}
};

} //! namespace gui

#endif // __StandardModelVisitor_ABC_h_
