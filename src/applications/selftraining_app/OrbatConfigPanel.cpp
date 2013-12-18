// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "OrbatConfigPanel.h"
#include "moc_OrbatConfigPanel.cpp"

#include "clients_kernel/Tools.h"

#include "frontend/Exercise_ABC.h"
#include "frontend/commands.h"
#include "frontend/CreateSession.h"

#include <boost/lexical_cast.hpp>
#include <vector>

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel constructor
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
OrbatConfigPanel::OrbatConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
    , sideBox_( 0 )
    , mainLayout_( 0 )
    , noSideObjectsLabel_( 0 )
{
    //no side objects label
    noSideObjectsLabel_ = new QCheckBox();

    //general Layout
    mainLayout_ = new QVBoxLayout( this );
    mainLayout_->addWidget( noSideObjectsLabel_ );
    mainLayout_->setAlignment( Qt::AlignTop );
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel destructor
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
OrbatConfigPanel::~OrbatConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel::GetName
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
QString OrbatConfigPanel::GetName() const
{
    return tools::translate( "OrbatConfigPanel", "Orbat" );
}

namespace
{
    std::string Serialize( const std::vector< unsigned int >& sides )
    {
        std::string result;
        for( auto it = sides.begin(); it != sides.end(); ++it )
        {
            result += boost::lexical_cast< std::string >( *it );
            if( it != sides.end() - 1 )
                result += ";";
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel::Commit
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void OrbatConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    if( sideBox_ && !sideCheckBox_.empty() )
    {
        std::vector< unsigned int > sides;
        for( auto it = sideCheckBox_.begin(); it != sideCheckBox_.end(); ++it )
            if( it->second->isChecked() )
                sides.push_back( it->first );

        if( sides.size() != sideCheckBox_.size() || !noSideObjectsLabel_->isChecked() )
        {
            frontend::CreateSession action( config_, exercise, session );
            if( sides.size() != sideCheckBox_.size() )
                action.SetOption( "session/config/simulation/orbat/subset/@parties", Serialize( sides ) );
            if( !noSideObjectsLabel_->isChecked() )
                action.SetOption( "session/config/simulation/orbat/subset/@no-party", "0" );
            action.Commit();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel::OnLanguageChanged
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void OrbatConfigPanel::OnLanguageChanged()
{
    if( sideBox_ )
        sideBox_->setTitle( tools::translate( "OrbatConfigPanel", "Create entities for those sides only:" ) );
    noSideObjectsLabel_->setText( tools::translate( "OrbatConfigPanel", "Create objects with no sides" ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel::Select
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void OrbatConfigPanel::Select( const frontend::Exercise_ABC& exercise )
{
    const tools::Path currentExercise = exercise.GetName();
    if( currentExercise_ != currentExercise )
    {
        ClearSelection();
        const std::map< unsigned int, QString > map = frontend::commands::ListSides( config_, exercise.GetName() );
        if( !map.empty() )
        {
            sideBox_ = new QGroupBox();
            QVBoxLayout* layout = new QVBoxLayout( sideBox_ );
            for( auto it = map.begin(); it != map.end(); ++it )
            {
                QCheckBox* checkbox = new QCheckBox( it->second );
                checkbox->setChecked( true );
                layout->addWidget( checkbox );
                sideCheckBox_[ it->first ] = checkbox;
            }
            mainLayout_->insertWidget( 0, sideBox_ );
            OnLanguageChanged();
        }
        noSideObjectsLabel_->setChecked( true );
        noSideObjectsLabel_->show();
        currentExercise_ = currentExercise;
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatConfigPanel::ClearSelection
// Created: LGY 2013-02-06
// -----------------------------------------------------------------------------
void OrbatConfigPanel::ClearSelection()
{
    if( sideBox_ )
    {
        delete sideBox_;
        sideBox_ = 0;
    }
    currentExercise_.Clear();
    sideCheckBox_.clear();
    noSideObjectsLabel_->hide();
}
