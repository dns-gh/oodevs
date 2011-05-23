// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierAttribute_h_
#define __AltitudeModifierAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AltitudeModifierAttribute( const sword::ObjectAttributes& msg );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& msg );
    virtual void Send  ( sword::ObjectAttributes& msg ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool readFromODB_;
    unsigned int height_;
    //@}
};

}

#endif // __AltitudeModifierAttribute_h_
