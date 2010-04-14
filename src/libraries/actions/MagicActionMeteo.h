// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MagicActionMeteo_h_
#define __MagicActionMeteo_h_

#include "MagicAction.h"

namespace kernel
{
    class MagicActionType;
    class Controller;
}

namespace xml
{
    class xistream;
}

namespace actions
{

// =============================================================================
/** @class  MagicActionMeteo
    @brief  MagicActionMeteo
*/
// Created: JSR 2010-04-09
// =============================================================================
class MagicActionMeteo : public MagicAction
{

public:
    //! @name Constructors/Destructor
    //@{
    MagicActionMeteo( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true );
    MagicActionMeteo( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic );
    virtual ~MagicActionMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}
};

}

#endif // __MagicActionMeteo_h_
