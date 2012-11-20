// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::DisasterPrototype_ABC */

#include "clients_gui_pch.h"
#include "DisasterPrototype_ABC.h"
#include "moc_DisasterPrototype_ABC.cpp"
#include "Tools.h"
#include "LoadableTimeEdit.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype_ABC::DisasterPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::DisasterPrototype_ABC", "Propagation" ) )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2, 7, 10 );
    layout->addWidget( new QLabel( tools::translate( "gui::DisasterPrototype_ABC", "Propagation Model:" ) ) );
    propagationFiles_ = new ValuedComboBox< std::string >( 0 );
    layout->addWidget( propagationFiles_ );
    FillInPaths( config.GetRootDir() );
    checkbox_ = new QCheckBox( tools::translate( "gui::DisasterPrototype_ABC", "Time:" ) );
    connect( checkbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );
    layout->addWidget( checkbox_ );
    date_ = new QDateTimeEdit();
    date_->setEnabled( false );
    layout->addWidget( date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC destructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype_ABC::~DisasterPrototype_ABC()
{
    // NOTHING
}

namespace
{

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
            && bfs::exists( dir / "propagation.xml" );
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::FillInPaths
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype_ABC::FillInPaths( const std::string& root )
{
    std::string path( ( bfs::path( root ) / "data/propagations" ).string() );
    QStringList result( ListDirectories( path, &IsPropagationDir ) );

    for( QStringList::const_iterator it = result.constBegin(); it != result.constEnd(); ++it )
        propagationFiles_->AddItem( *it, (*it).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::CheckValidity
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
bool DisasterPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return !propagationFiles_->GetValue().empty();
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype_ABC::OnStateChanged
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype_ABC::OnStateChanged( int state )
{
    date_->setEnabled( state == Qt::Checked );
}
