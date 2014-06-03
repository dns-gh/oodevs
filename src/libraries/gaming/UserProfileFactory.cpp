// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UserProfileFactory.h"
#include "UserProfile.h"
#include "Model.h"
#include "UserProfilesModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: UserProfileFactory constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfileFactory::UserProfileFactory( const Model& model, kernel::Controllers& controllers, Publisher_ABC& publisher )
    : model_( model )
    , controllers_( controllers )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory destructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfileFactory::~UserProfileFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile* UserProfileFactory::Create( const sword::ProfileCreation& message ) const
{
    return new UserProfile( message, controllers_.controller_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfileFactory::Create()
{
    UserProfile profile( GenerateUniqueLogin(), controllers_.controller_, publisher_ );
    profile.RequestCreation();
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::GenerateUniqueLogin
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
QString UserProfileFactory::GenerateUniqueLogin() const
{
    static const QString defaultName = tools::translate( "UserProfileFactory", "New profile" );
    QString name;
    int i = 0;
    while( model_.profiles_.Find( name = defaultName + ( i > 0 ? " (" + QString::number( i ) + ")" : "" ) ) )
        ++i;
    return name;
}
