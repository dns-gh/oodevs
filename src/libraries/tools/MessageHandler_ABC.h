// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageHandler_ABC_h
#define tools_MessageHandler_ABC_h

#include "Handler_ABC.h"

namespace tools
{
// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Message handler definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageHandler_ABC : public Handler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageHandler_ABC() {}
    virtual ~MessageHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const Category& message ) = 0;
    //@}
};

}

#endif // tools_MessageHandler_ABC_h
