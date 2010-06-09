 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class BypassAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BypassAttribute( const Common::MsgObjectAttributes& asnMsg );
    virtual ~BypassAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send  ( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    unsigned int nPercentageBypassing_;
};

}

#endif // __BypassAttribute_h_
