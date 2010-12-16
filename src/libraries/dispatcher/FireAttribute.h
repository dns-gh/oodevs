 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class FireAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FireAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    std::string fireClass_;// XML reference - no resolved by dispatcher
    int maxCombustionEnergy_;
    //@}
};

}

#endif // __FireAttribute_h_
