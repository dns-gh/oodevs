 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  CrossingSiteAttribute
    @brief  CrossingSiteAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class CrossingSiteAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSiteAttribute( const Common::MsgObjectAttributes& asnMsg );
    virtual ~CrossingSiteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send  ( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nWidth_;
    unsigned int nDepth_;
    unsigned int nSpeed_;
    bool         bNeedsConstruction_;    
    //@}
};

}

#endif // __CrossingSiteAttribute_h_
