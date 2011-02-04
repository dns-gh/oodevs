 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineAttribute_h_
#define __MineAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class MineAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MineAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~MineAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned     dotation_;
    unsigned int nNbrDotationForMining_;
    unsigned int nPercentageMining_;
    //@}
};

}

#endif // __MineAttribute_h_
