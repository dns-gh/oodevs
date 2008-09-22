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
#include "tools/GeneralConfig.h" 
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: ProfileList constructor
// Created: RDS 2008-09-05
// -----------------------------------------------------------------------------
ProfileList::ProfileList( QWidget* parent, const tools::GeneralConfig& config  )
    : QListBox( parent ) 
    , config_  ( config )  
{
    // NOTHING
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
    clear(); 
    try
    {
        if ( exercise != "" )
        {
            ReadSides( exercise.ascii() ) ; 
            ReadProfiles( exercise.ascii() ) ; 
        }
    }
    catch( ... ) 
    {
        // something went wrong ... don't crash since it may be related to the exercise file, but clear 
        clear(); 
    }
}

// =============================================================================
// Sides 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadSides
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void ProfileList::ReadSides( const std::string& exercise )
{
    sides_.DeleteAll();
    std::string orbatFile ; 
    {
        xml::xifstream xis( config_.GetExerciseFile( exercise ) ) ; 
        xis >> xml::start("exercise")
                >> xml::start("orbat")
                    >> xml::attribute("file", orbatFile ); 
    }
    {
        const std::string file = ( config_.BuildChildPath( config_.GetExerciseFile( exercise ), orbatFile ) ); 
        xml::xifstream xis( file ); 
        xis >> xml::start("orbat") 
                >> xml::start("sides") 
                    >> xml::list("side", *this, &ProfileList::ReadSide ); 
    }    
}

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadSide
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void ProfileList::ReadSide( xml::xistream& xis )
{
    Side* side = new Side( xis ) ; 
    sides_.Register( side->GetId(), *side ) ; 
}

// =============================================================================
// Profiles
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadProfiles
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void ProfileList::ReadProfiles( const std::string& exercise )
{
    std::string profilesFile ; 
    {
        xml::xifstream xis( config_.GetExerciseFile( exercise ) ) ; 
        xis >> xml::start("exercise")
                >> xml::start("profiles")
                    >> xml::attribute("file", profilesFile ); 
    }
    {
        const std::string file = ( config_.BuildChildPath( config_.GetExerciseFile( exercise ), profilesFile ) ); 
        xml::xifstream xis( file ); 
        xis >> xml::start("profiles") 
                >> xml::list("profile", *this, &ProfileList::ReadProfile ); 
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileList::ReadProfile
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ProfileList::ReadProfile( xml::xistream& xis )
{
    Profile profile(xis, sides_ ) ;  
    QString name = ( profile.GetLogin() != "" ) ?  profile.GetLogin() :  tr( "no name" ) ; 
    if ( profile.IsSupervision() ) 
        insertItem( QImage("resources/images/selftraining/commandpost.xpm" ), name  ) ;
    else
        insertItem( name  ) ;

}

