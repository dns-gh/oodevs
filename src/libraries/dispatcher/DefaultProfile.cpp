// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DefaultProfile.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DefaultProfile constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DefaultProfile::DefaultProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DefaultProfile::~DefaultProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const ASN1T_MsgsClientToSim& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const ASN1T_MsgsClientToAuthentication& msg ) const
{
    return msg.msg.t == T_MsgsClientToAuthentication_msg_msg_authentication_request;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const ASN1T_MsgsClientToReplay& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::CheckRights
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
bool DefaultProfile::CheckRights( const ASN1T_ChatTarget& , const ASN1T_ChatTarget& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultProfile::GetName
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
std::string DefaultProfile::GetName() const
{
    return "";
}
