// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InitializationMessage_h_
#define __InitializationMessage_h_

// =============================================================================
/** @class  InitializationMessage
    @brief  InitializationMessage
*/
// Created: AGE 2006-04-21
// =============================================================================
class InitializationMessage
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit InitializationMessage( const std::string& message );
    virtual ~InitializationMessage();
    //@}

    //! @name Member data
    //@{
    std::string message_;
    //@}
};

#endif // __InitializationMessage_h_
