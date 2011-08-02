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

#include "clients_gui/CursorStrategy_ABC.h"

namespace kernel
{
    class GlTools_ABC;
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
    explicit CursorStrategy( kernel::GlTools_ABC& tools );
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
    kernel::GlTools_ABC& tools_;
    QCursor selectedTool_;
    QCursor selectedContext_;
    //@}
};

}

#endif // __CursorStrategy_h_
