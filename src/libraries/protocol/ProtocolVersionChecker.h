// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProtocolVersionChecker_h_
#define __ProtocolVersionChecker_h_

#include <string>

namespace Version
{
    class ProtocolVersion;
}

// =============================================================================
/** @class  Version
    @brief  Version
*/
// Created: RPD 2010-05-26
// =============================================================================
class ProtocolVersionChecker
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ProtocolVersionChecker( const Version::ProtocolVersion& version );
    virtual ~ProtocolVersionChecker();
    //@}

    //! @name Operations
    //@{
    bool CheckCompatibility();
    static std::string GetCurrentProtocolVersion();
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    ProtocolVersionChecker( const ProtocolVersionChecker& );            //!< Copy constructor
    ProtocolVersionChecker& operator=( const ProtocolVersionChecker& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::string protocolVersion_;
    //@}
};

#endif // __Version_h_
