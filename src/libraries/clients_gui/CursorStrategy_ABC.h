// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CursorStrategy_ABC_h_
#define __CursorStrategy_ABC_h_

namespace gui
{

// =============================================================================
/** @class  CursorStrategy_ABC
    @brief  CursorStrategy_ABC
*/
// Created: SBO 2008-02-12
// =============================================================================
class CursorStrategy_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CursorStrategy_ABC() {}
    virtual ~CursorStrategy_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SelectTool   ( const QCursor& cursor, bool selected ) = 0;
    virtual void SelectContext( const QCursor& cursor, bool selected ) = 0;
    //@}
};

}

#endif // __CursorStrategy_ABC_h_
