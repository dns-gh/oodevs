 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCTypeAttribute_h_
#define _NBCTypeAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  NBCTypeAttribute
    @brief  NBCTypeAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class NBCTypeAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCTypeAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~NBCTypeAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned agent_;
    unsigned int concentration_;
    unsigned int sourceLifeDuration_;
    //@}
};

}

#endif // __NBCTypeAttribute_h_
