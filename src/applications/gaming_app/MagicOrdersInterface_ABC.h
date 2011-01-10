// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrdersInterface_ABC_h_
#define __MagicOrdersInterface_ABC_h_

namespace kernel
{
    class ContextMenu;
    class Object_ABC;
}

// =============================================================================
/** @class  MagicOrdersInterface_ABC
    @brief  MagicOrdersInterface_ABC
*/
// Created: SLG 2010-12-05
// =============================================================================
class MagicOrdersInterface_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface_ABC(){}
    virtual ~MagicOrdersInterface_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu ) = 0;
    //@}

};

#endif // __MagicOrdersInterface_ABC_h_
