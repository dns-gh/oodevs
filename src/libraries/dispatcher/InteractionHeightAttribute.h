 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InteractionHeightAttribute_h_
#define _InteractionHeightAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  InteractionHeightAttribute
    @brief  InteractionHeightAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class InteractionHeightAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InteractionHeightAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~InteractionHeightAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    float height_;
    //@}
};

}

#endif // __InteractionHeightAttribute_h_
