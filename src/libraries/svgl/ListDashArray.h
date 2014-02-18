// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListDashArray_h_
#define __ListDashArray_h_

#include "DashArray.h"

namespace svg
{

// =============================================================================
/** @class  ListDashArray
    @brief  ListDashArray
*/
// Created: AGE 2007-10-30
// =============================================================================
class ListDashArray : public DashArray
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ListDashArray( unsigned list );
    virtual ~ListDashArray();
    //@}

    //! @name Operations
    //@{
    virtual ListDashArray& Clone() const;
    virtual void ReachTop( const RenderingContext_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    unsigned list_;
    //@}
};

}

#endif // __ListDashArray_h_
