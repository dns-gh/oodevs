// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListLength_h_
#define __ListLength_h_

#include "Length.h"

namespace svg
{

// =============================================================================
/** @class  ListLength
    @brief  ListLength
*/
// Created: ZEBRE 2007-05-24
// =============================================================================
class ListLength : public Length
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ListLength( unsigned list );
    virtual ~ListLength();
    //@}

    //! @name Operations
    //@{
    virtual ListLength& Clone() const;
    virtual void ReachTop( const RenderingContext_ABC& );
    virtual void SetupLineWidth() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned list_;
    //@}
};

}

#endif // __ListLength_h_
