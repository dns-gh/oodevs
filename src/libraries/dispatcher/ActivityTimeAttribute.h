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
    explicit ActivityTimeAttribute( const Common::MsgObjectAttributes& asnMsg );
    virtual ~ActivityTimeAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send  ( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    unsigned int nActivityTime_;
    //@}

};

}

#endif // __ActivityTimeAttribute_h_
