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
#include "moc_InputPropagationPrototype_ABC.cpp"
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
InputPropagationPrototype_ABC::InputPropagationPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation" ) )
    , root_ ( config.GetRootDir() )
{
    new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Propagation Model:" ), this );
    propagationFiles_ = new ValuedComboBox< std::string >( this );
    FillInPaths();

    new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Lookup data:" ), this );
    dataField_ = new ValuedComboBox< std::string >( this );
    // TODO : %TMP%
    dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure C" ), std::string( "nom_var_shp_mesure_C" ) );
    dataField_->AddItem( tools::translate( "gui::InputPropagationPrototype_ABC", "Mesure Ct" ), std::string( "nom_var_shp_mesure_Ct" ) );

    new QLabel( tools::translate( "gui::InputPropagationPrototype_ABC", "Send data:" ), this );
    exportData_ = new QCheckBox( this );
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
        const bfs::path root = bfs::path( base, bfs::native );
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
                    entry.push_front( p.leaf().c_str() );
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
        return ( bfs::path( root, bfs::native ) / path ).native_directory_string();
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
        propagationFiles_->AddItem( *it, (*it).ascii() );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool InputPropagationPrototype_ABC::CheckValidity() const
{
    return ! propagationFiles_->GetValue().empty() && ! dataField_->GetValue().empty();
}

