 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SealOffAttribute_h_
#define __SealOffAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  SealOffAttribute
    @brief  SealOffAttribute
*/
// Created: MGD 2010-08-24
// =============================================================================
class SealOffAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SealOffAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~SealOffAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    unsigned int nSealOffLevel_;
};

}

#endif // __SealOffAttribute_h_
