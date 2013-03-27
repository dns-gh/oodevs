// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ExerciseMenu.h"
#include "moc_ExerciseMenu.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#include <Qt3Support/q3url.h>

// -----------------------------------------------------------------------------
// Name: ExerciseMenu constructor
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
ExerciseMenu::ExerciseMenu( QWidget* parent, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter, const QString& title /* = "" */ )
    : gui::RichMenu( "exerciseMenu", parent, controllers, title )
    , controllers_( controllers )
    , interpreter_( interpreter )
{
    connect( this, SIGNAL( activated( int ) ), SLOT( OnSelect( int ) ) );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu destructor
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
ExerciseMenu::~ExerciseMenu()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::NotifyUpdated
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::NotifyUpdated( const kernel::ModelLoaded& model )
{
    Reset();
    tools::Xifstream xis( model.config_.GetExerciseFile() );
    xis >> xml::start( "exercise" )
            >> xml::optional >> xml::start( "meta" )
                >> xml::optional >> xml::start( "resources" )
                    >> xml::list( "resource", *this, &ExerciseMenu::ReadResource, model.config_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::NotifyUpdated
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::NotifyUpdated( const kernel::ModelUnLoaded& /*model*/ )
{
    Reset();
}

namespace
{
    std::string MakeLink( const std::string& file )
    {
        return QFileInfo( file.c_str() ).absFilePath().toStdString();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::ReadResource
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::ReadResource( xml::xistream& xis, const tools::ExerciseConfig& config )
{
    tools::Path file;
    std::string name;
    xis >> xml::attribute( "file", file )
        >> xml::attribute( "name", name );
    if( file.IsEmpty() )
        return;
    const int id = insertItem( name.c_str(), static_cast< int >( links_.size() ) );
    file = config.BuildExerciseChildFile( file );
    links_.push_back( file.Absolute() );
    setItemEnabled( id, file.Exists() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::OnSelect
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::OnSelect( int index )
{
    if( index >= 0 && index < int( links_.size() ) )
    {
        tools::Path& link = links_[ index ];
        tools::Path drive = link.Root();
        tools::Path path = link.Relative();
        QString tmp = path.ToUTF8().c_str();
        Q3Url::encode( tmp );
        tmp = QString( "%1:%2" ).arg( drive.ToUTF8().c_str() ).arg( tmp );
        interpreter_.Interprete( QString( "file://%1" ).arg( tmp ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::Reset
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::Reset()
{
    clear();
    links_.clear();
}
