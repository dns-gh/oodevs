// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainFeatureSearchBox.h"
#include "moc_TerrainFeatureSearchBox.cpp"
#include "resources.h"
#include "View_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/WorldParameters.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ApproximativeMap.h"
#include "graphics/NoVBOShapeLayer.h"
#include "graphics/GraphicSetup_ABC.h"
#pragma warning( disable : 4512 )
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>

using namespace gui;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearchBox constructor
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
TerrainFeatureSearchBox::TerrainFeatureSearchBox( QWidget* parent, kernel::Controllers& controllers, View_ABC& view )
    : QHBox( parent )
    , controllers_( controllers )
    , view_( view  )
    , nameLocations_( new T_NameLocations() )
    , current_( 0 )
    , index_  ( 0 )
{
    setMargin( 2 );
    setSpacing( 10 );

    QLineEdit* edit = new QLineEdit( this );
    QPushButton* next = new QPushButton( QIconSet( MAKE_ICON( search ) ), "", this );
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( edit->height() );
    next->setMaximumHeight( edit->height() );

    connect( edit, SIGNAL( textChanged( const QString& ) ), this, SLOT( Search( const QString& ) ) );
    connect( next, SIGNAL( pressed() ), this, SLOT( FindNext() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearchBox destructor
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
TerrainFeatureSearchBox::~TerrainFeatureSearchBox()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearchBox::Search
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
void TerrainFeatureSearchBox::Search( const QString& value )
{
    const T_PointVector* points = nameLocations_->Find( value.ascii() );
    if( points != current_ )
        index_ = 0;
    current_ = points;
    if( current_ && current_->size() > index_ )
        view_.CenterOn( current_->at( index_ ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearchBox::FindNext
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
void TerrainFeatureSearchBox::FindNext()
{
    ++index_;
    if( current_ )
    {
        index_ = index_ % current_->size();
        view_.CenterOn( current_->at( index_ ) );
    }
}

namespace
{
    struct NameShapeLayer : public GraphicSetup_ABC, public NoVBOShapeLayer
    {
        NameShapeLayer( const std::string& filename )
            : NoVBOShapeLayer( *this, filename )
        {}
        virtual void DrawName( const geometry::Point2f& at, const std::string& name )
        {
            names_[ Cleanup( name ) ].push_back( at );
        }
        std::string Cleanup( std::string name )
        {
            boost::algorithm::to_lower( name );
            // $$$$ AGE 2008-05-28: 
            boost::algorithm::erase_all( name, "rivière" );
            boost::algorithm::erase_all( name, "riviere" );
            boost::algorithm::erase_all( name, "ruisseau" );
            boost::algorithm::trim( name );
            return name;
        }
        virtual bool ShouldDisplay      ( const TerrainData& , const geometry::Rectangle2f& ) { return false; }
        virtual bool ShouldDisplayBorder( const TerrainData& , const geometry::Rectangle2f& ) { return false; }
        virtual bool ShouldDisplayNames ( const TerrainData& , const geometry::Rectangle2f& ) { return true; }
        virtual void SetupLineGraphics  ( const Data_ABC* ) {}
        virtual void SetupBorderGraphics( const Data_ABC* ) {}
        virtual void SetupAreaGraphics  ( const Data_ABC* ) {}
        std::map< std::string, T_PointVector > names_;
    };
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearchBox::NotifyUpdated
// Created: AGE 2008-05-28
// -----------------------------------------------------------------------------
void TerrainFeatureSearchBox::NotifyUpdated( const kernel::ModelLoaded& model )
{
    current_ = 0;
    index_ = 0;
    nameLocations_.reset( new T_NameLocations() );

    kernel::WorldParameters parameters( model.config_ );
    if( ! parameters.graphicsDirectory_.empty() )
    {
        const bfs::path dump = bfs::path( parameters.graphicsDirectory_, bfs::native ) / "shapes.dump";
        if( bfs::exists( dump ) )
        {
            NameShapeLayer layer( dump.native_file_string() );
            const float inf = std::numeric_limits< float >::infinity();
            const geometry::Rectangle2f universe( -inf, -inf, inf, inf );
            layer.Initialize( universe );
            layer.Paint( universe );
            for( std::map< std::string, T_PointVector >::const_iterator it = layer.names_.begin(); it != layer.names_.end(); ++it )
                nameLocations_->Add( it->first, it->second );
        }
    }
}
