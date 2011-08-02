// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PanelStack_ABC_h_
#define __PanelStack_ABC_h_

namespace gui
{

// =============================================================================
/** @class  PanelStack_ABC
    @brief  Panel stack base class
*/
// Created: SBO 2006-08-10
// =============================================================================
class PanelStack_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PanelStack_ABC() {}
    virtual ~PanelStack_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( QWidget* widget ) = 0;
    virtual void Remove( QWidget* widget ) = 0;
    //@}
};

}

#endif // __PanelStack_ABC_h_
