// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayAttribute_h_
#define __DelayAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  DelayAttribute
    @brief  DelayAttribute
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DelayAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~DelayAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nDelay_;
    //@}
};

}

#endif // __DelayAttribute_h_
