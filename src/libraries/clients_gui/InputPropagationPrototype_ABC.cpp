// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::InputPropagationPrototype_ABC */

#include "clients_gui_pch.h"
#include "InputPropagationPrototype_ABC.h"
#include "Tools.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
InputPropagationPrototype_ABC::InputPropagationPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config,
                                                              const std::string& model )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation" ) )
    , root_ ( config.GetRootDir() )
    , model_( model )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2, 6, 10 );
    layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation Model:" ) ) );
    propagationFiles_ = new ValuedComboBox< std::string >( 0 );
    layout->addWidget( propagationFiles_ );
    FillInPaths();

    if( model == "shapefile-input" )
    {
        layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Lookup data:" ) ) );
        dataField_ = new ValuedComboBox< std::string >( this );
        dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure C" ), std::string( "nom_var_shp_mesure_C" ) );
        dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure Ct" ), std::string( "nom_var_shp_mesure_Ct" ) );
        layout->addWidget( dataField_ );

        layout->addWidget( new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Send data:" ) ) );
        exportData_ = new QCheckBox();
        layout->addWidget( exportData_ );
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
InputPropagationPrototype_ABC::~InputPropagationPrototype_ABC()
{
    // NOTHING
}

namespace
{
    // Copied from fronted library

    template< typename Validator >
    QStringList ListDirectories( const std::string& base, Validator v )
    {
        QStringList result;
        const bfs::path root = bfs::path( base );
        if( ! bfs::exists( root ) )
            return result;

        bfs::recursive_directory_iterator end;
        for( bfs::recursive_directory_iterator it( root ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( v( child ) )
            {
                QStringList entry;
                bfs::path p( child );
                for( int i = it.level(); i >= 0; --i )
                {
                    entry.push_front( p.filename().string().c_str() );
                    p = p.parent_path();
                }
                result.append( entry.join( "/" ) );
                it.no_push();
            }
        }
        return result;
    }

    bool IsPropagationDir( const bfs::path& dir )
    {
        return bfs::is_directory( dir )
               && bfs::exists( dir / "propagation.xml" )
               && bfs::exists( dir / "propagation" )
               && bfs::is_directory( dir / "propagation" );
    }

    std::string BuildPropagationDir( const std::string& root, const std::string& path )
    {
        return ( bfs::path( root ) / path ).string();
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::FillInPaths
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void InputPropagationPrototype_ABC::FillInPaths()
{
    std::string path( BuildPropagationDir( root_, "data/propagations" ) );
    QStringList result( ListDirectories( path, &IsPropagationDir ) );

    for( QStringList::const_iterator it = result.constBegin(); it != result.constEnd(); ++it )
        propagationFiles_->AddItem( *it, (*it).toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool InputPropagationPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return !propagationFiles_->GetValue().empty();
}
