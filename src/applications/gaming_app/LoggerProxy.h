// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LoggerProxy_h_
#define __LoggerProxy_h_

#include "clients_kernel/Logger_ABC.h"

// =============================================================================
/** @class  LoggerProxy
    @brief  LoggerProxy
*/
// Created: AGE 2008-05-16
// =============================================================================
class LoggerProxy : public kernel::Logger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LoggerProxy();
    virtual ~LoggerProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Info( const std::string& message );
    virtual void Warning( const std::string& message );
    virtual void Error( const std::string& message );
    //@}

    //! @name Modifiers
    //@{
    void SetLogger( kernel::Logger_ABC& base );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Logger_ABC* base_;
    //@}
};

#endif // __LoggerProxy_h_
