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
    explicit MineAttribute( const Common::MsgObjectAttributes& asnMsg );
    virtual ~MineAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send  ( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    unsigned     dotation_;
    unsigned int nNbrDotationForMining_;
    unsigned int nPercentageMining_;
    //@}
};

}

#endif // __MineAttribute_h_
