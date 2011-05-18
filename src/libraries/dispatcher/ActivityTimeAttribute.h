 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimeAttribute_h_
#define __ActivityTimeAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  ActivityTimeAttribute
    @brief  ActivityTimeAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ActivityTimeAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActivityTimeAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~ActivityTimeAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    int nActivityTime_;
    //@}

};

}

#endif // __ActivityTimeAttribute_h_
