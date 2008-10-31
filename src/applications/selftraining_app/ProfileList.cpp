// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Profile.h" 
#include "Side.h"
#include "ProfileList.h"
#include "moc_ProfileList.cpp"
#include "frontend/commands.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h" 
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: ProfileList constructor
// Created: RDS 2008-09-05
// -----------------------------------------------------------------------------
ProfileList::ProfileList( QWidget* parent, const tools::GeneralConfig& config )
    : QListBox( parent )
    , config_( config )
{
    connect( this, SIGNAL( highlighted( int ) ), SLOT( OnSelect( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ProfileList destructor
// Created: RDS 2008-09-05
// -----------------------------------------------------------------------------
ProfileList::~ProfileList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileList::Update
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ProfileList::Update( const QString& exercise )
{
    profiles_.clear();
    clear();
    try
    {
        if( !exercise.isEmpty() )
            ReadProfiles( exercise.ascii() );
        setSelected( 0, true );
    }
    catch( ... )
    {
        // something went wrong ... don't crash since it may be related to the exercise file, but clear
        clear();
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadProfiles
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void ProfileList::ReadProfiles( const std::string& exercise )
{
    std::string profilesFile; 
    {
        xml::xifstream xis( config_.GetExerciseFile( exercise ) ) ;
        xis >> xml::start( "exercise" )
                >> xml::start( "profiles" )
                    >> xml::attribute( "file", profilesFile );
    }
    {
        const std::string file = ( config_.BuildChildPath( config_.GetExerciseFile( exercise ), profilesFile ) );
        xml::xifstream xis( file );
        xis >> xml::start( "profiles" )
                >> xml::list( "profile", *this, &ProfileList::ReadProfile );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadProfile
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ProfileList::ReadProfile( xml::xistream& xis )
{
    const Profile profile( xis );
    const QString name = profile.GetLogin() != "" ?  profile.GetLogin() : tools::translate( "ReadProfile", "anonymous" );
    if ( profile.IsSupervision() )
        insertItem( QImage( "resources/images/selftraining/commandpost.xpm" ), name );
    else
        insertItem( name );
    profiles_.push_back( profile );
}

// -----------------------------------------------------------------------------
// Name: ProfileList::OnSelect
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ProfileList::OnSelect( int index )
{
    if( index < int( profiles_.size() ) )
        emit Select( profiles_[index] );
    repaintContents();
}
