// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"

#include "Profile.h"
#include "network/Master_Asn.h"
#include "xeumeuleu/xml.h"

using namespace master;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::Profile( const Master& master, const std::string& strLogin, xml::xistream& xis )
    : master_          ( master )
    , strLogin_        ( strLogin )
    , strPassword_     () 
    , bRoleAdaptation_ ( false )
    , bRolePreparation_( false )
    , bRoleGame_       ( false )
    , bRoleSupervision_( false )
{
    xis >> xml::attribute( "password"           , strPassword_          )
        >> xml::attribute( "role-adaptation"    , bRoleAdaptation_      )
        >> xml::attribute( "role-preparation"   , bRolePreparation_     )
        >> xml::attribute( "role-game"          , bRoleGame_            )
        >> xml::attribute( "role-supervision"   , bRoleSupervision_     );
    MT_LOG_INFO_MSG( "Profile loaded : '" << strLogin_ << "'" );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::~Profile()
{
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::CheckPassword
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
bool Profile::CheckPassword( const std::string& strPassword ) const
{
    return strPassword == strPassword_;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const ASN1T_MsgsInMaster& msg ) const
{
    switch( msg.msg.t )
    {
        case T_MsgsInMaster_msg_msg_authentication_request : return true;

    //$$$ A COMPLETER
        default:
            return false;
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: NLD 2006-10-09
// -----------------------------------------------------------------------------
void Profile::Send( ASN1T_MasterProfile& asn ) const
{
    asn.login = strLogin_.c_str();

    asn.role_adaptation  = bRoleAdaptation_;
    asn.role_preparation = bRolePreparation_;
    asn.role_game        = bRoleGame_;
    asn.role_supervision = bRoleSupervision_;
}
