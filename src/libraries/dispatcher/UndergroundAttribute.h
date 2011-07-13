// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundAttribute_h_
#define __UndergroundAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  UndergroundAttribute
    @brief  UndergroundAttribute
*/
// Created: JSR 2011-07-11
// =============================================================================
class UndergroundAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UndergroundAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~UndergroundAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string network_;
    //@}
};

}

#endif // __UndergroundAttribute_h_
