// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MT_Exception_h_
#define __MT_Exception_h_

#include <tools/Exception.h>

// =============================================================================
/** @class  MT_Exception
    @brief  MT_Exception
*/
// Created: ABR 2012-12-10
// =============================================================================
class MT_Exception : public tools::Exception
{

public:
    //! @name Constructors/Destructor
    //@{
             MT_Exception( const std::string& file, const std::string& function, unsigned int line, const std::string& what );
    virtual ~MT_Exception();
    //@}

    //! @name Operations
    //@{
    void SendToLogger() const;
    //@}
};

#define MASA_EXCEPTION_MT( what ) MT_Exception( __FILE__, __FUNCTION__, __LINE__, what )

#endif // __MT_Exception_h_
