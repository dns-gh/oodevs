// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListPaint_h_
#define __ListPaint_h_

#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  ListPaint
    @brief  ListPaint
*/
// Created: ZEBRE 2007-05-24
// =============================================================================
class ListPaint : public Paint_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ListPaint( unsigned list );
    virtual ~ListPaint();
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& references, float opacity ) const;
    virtual ListPaint& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ListPaint( const ListPaint& );            //!< Copy constructor
    ListPaint& operator=( const ListPaint& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned list_;
    //@}
};

}

#endif // __ListPaint_h_
