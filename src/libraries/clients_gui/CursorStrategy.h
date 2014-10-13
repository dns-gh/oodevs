// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CursorStrategy_h_
#define __CursorStrategy_h_

#include "CursorStrategy_ABC.h"

namespace gui
{
    class GLView_ABC;
}

namespace gui
{

// =============================================================================
/** @class  CursorStrategy
    @brief  CursorStrategy
*/
// Created: SBO 2008-02-12
// =============================================================================
class CursorStrategy : public CursorStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CursorStrategy( GLView_ABC& tools );
    virtual ~CursorStrategy();
    //@}

    //! @name Operations
    //@{
    virtual void SelectTool   ( const QCursor& cursor, bool selected );
    virtual void SelectContext( const QCursor& cursor, bool selected );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CursorStrategy( const CursorStrategy& );            //!< Copy constructor
    CursorStrategy& operator=( const CursorStrategy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    GLView_ABC& tools_;
    QCursor selectedTool_;
    QCursor selectedContext_;
    //@}
};

}

#endif // __CursorStrategy_h_
