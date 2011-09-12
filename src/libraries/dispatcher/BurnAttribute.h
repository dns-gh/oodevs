 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnAttribute_h_
#define __BurnAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  BurnAttribute
    @brief  BurnAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class BurnAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BurnAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~BurnAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    int currentHeat_;
    int combustionEnergy_;
    //@}
};

}

#endif // __BurnAttribute_h_
