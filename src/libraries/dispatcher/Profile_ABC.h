// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_ABC_h_
#define __Profile_ABC_h_

namespace MsgsClientToSim
{
    class MsgClientToSim;
}

namespace MsgsClientToAuthentication
{
    class MsgClientToAuthentication;
}

namespace Common
{
    class MsgChatTarget;
}

namespace MsgsClientToReplay
{
    class MsgClientToReplay;
}

namespace dispatcher
{

// =============================================================================
/** @class  Profile_ABC
    @brief  Profile definition
*/
// Created: AGE 2007-08-24
// =============================================================================
class Profile_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile_ABC() {};
    virtual ~Profile_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool CheckRights( const MsgsClientToSim::MsgClientToSim& msg ) const = 0;
    virtual bool CheckRights( const MsgsClientToAuthentication::MsgClientToAuthentication& msg ) const = 0;
    virtual bool CheckRights( const MsgsClientToReplay::MsgClientToReplay& msg ) const = 0;
    virtual bool CheckRights( const Common::MsgChatTarget& source, const Common::MsgChatTarget& target ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const = 0;
    //@}
};

}

#endif // __Profile_ABC_h_
