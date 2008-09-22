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
#include <xeumeuleu/xml.h>
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ExerciseMenu constructor
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
ExerciseMenu::ExerciseMenu( QWidget* parent, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter )
    : QPopupMenu( parent )
    , controllers_( controllers )
    , interpreter_( interpreter )
{
    connect( this, SIGNAL( activated( int ) ), SLOT( OnSelect( int ) ) );
    controllers_.Register( *this );
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
    xml::xifstream xis( model.config_.GetExerciseFile() );
    xis >> xml::start( "exercise" )
            >> xml::optional() >> xml::start( "meta" )
                >> xml::optional() >> xml::start( "resources" )
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
        const QFileInfo info( file.c_str() );
        const QString protocol = info.extension( false ) == "exe" ? "cmd" : "file";
        return QString( "%1://%2" ).arg( protocol ).arg( info.absFilePath() ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::ReadResource
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::ReadResource( xml::xistream& xis, const tools::ExerciseConfig& config )
{
    std::string file, name, description;
    xis >> xml::attribute( "file", file )
        >> xml::attribute( "name", name );
    if( file.empty() )
        return;
    const int id = insertItem( name.c_str(), links_.size() );
    file = config.BuildExerciseChildFile( file );
    links_.push_back( MakeLink( file ) );
    setItemEnabled( id, bfs::exists( file ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseMenu::OnSelect
// Created: SBO 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseMenu::OnSelect( int index )
{
    if( index >= 0 && index < int( links_.size() ) )
        interpreter_.Interprete( links_[index].c_str() );
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
