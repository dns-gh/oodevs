// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DefaultProfile_h_
#define __DefaultProfile_h_

#include "Profile_ABC.h"


////using namespace Common;

namespace dispatcher
{

// =============================================================================
/** @class  DefaultProfile
    @brief  DefaultProfile
*/
// Created: AGE 2007-08-24
// =============================================================================
class DefaultProfile : public Profile_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultProfile();
    virtual ~DefaultProfile();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckRights( const MsgsClientToSim::MsgClientToSim& msg ) const;
    virtual bool CheckRights( const MsgsClientToAuthentication::MsgClientToAuthentication& msg ) const;
    virtual bool CheckRights( const MsgsClientToReplay::MsgClientToReplay& msg ) const;
    virtual bool CheckRights( const Common::MsgChatTarget& source, const Common::MsgChatTarget& target ) const;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const;
    //@}
};

}

#endif // __DefaultProfile_h_
