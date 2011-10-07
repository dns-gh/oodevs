// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodAttribute_h_
#define __FloodAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-16
// =============================================================================
class FloodAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodAttribute( const sword::ObjectAttributes& msg );
    virtual ~FloodAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& msg );
    virtual void Send  ( sword::ObjectAttributes& msg ) const;
    //@}

private:
    //! @name Member data
    //@{
    int depth_;
    int refDist_;
    //@}
};

}

#endif // __FloodAttribute_h_
