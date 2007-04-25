// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfilesDispatcher_h_
#define __ProfilesDispatcher_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class ProfileManager;
    class Publisher_ABC;

// =============================================================================
/** @class  ProfilesDispatcher
    @brief  ProfilesDispatcher
*/
// Created: AGE 2007-04-10
// =============================================================================
class ProfilesDispatcher
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesDispatcher( ProfileManager& manager, Publisher_ABC& publisher );
    virtual ~ProfilesDispatcher();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsClientToMiddle& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfilesDispatcher( const ProfilesDispatcher& );            //!< Copy constructor
    ProfilesDispatcher& operator=( const ProfilesDispatcher& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void OnReceiveMsgProfileCreationRequest   ( const ASN1T_MsgProfileCreationRequest&    asnMsg );
    void OnReceiveMsgProfileUpdateRequest     ( const ASN1T_MsgProfileUpdateRequest&      asnMsg );
    void OnReceiveMsgProfileDestructionRequest( const ASN1T_MsgProfileDestructionRequest& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    ProfileManager& manager_;
    Publisher_ABC& publisher_;
    //@}
};

}

#endif // __ProfilesDispatcher_h_
