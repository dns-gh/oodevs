// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FireColorPanel.h"
#include "moc_FireColorPanel.cpp"
#include "ColorButton.h"
#include "GLOptions.h"
#include "GLProxy.h"
#include "resources.h"
#include "SignalAdapter.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/StaticModel.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

FireColorPanel::FireColorPanel( QWidget* parent,
                                kernel::Controllers& controllers,
                                const kernel::StaticModel& model,
                                FireGroup group )
    : PreferencePanel_ABC( parent, "FireColorPanel" )
    , group_( group )
    , model_( model )
    , controllers_( controllers )
    , options_( controllers.options_ )
{
    SetupUi( this );
    controllers_.Update( *this );
}

FireColorPanel::~FireColorPanel()
{
    controllers_.Unregister( *this );
}

class gui::Model : public QStringListModel
{
public:
    Model( const QStringList& list, QObject* parent )
        : QStringListModel( list, parent )
        , list_( list )
    {
        // NOTHING
    }

    bool Contains( const QString& name ) const
    {
        return GetIndex( name ).isValid();
    }

    QModelIndex GetIndex( const QString& name ) const
    {
        if( name == FireOption::GetAny() )
            return index( 0, 0 );
        // first item of the list is not sorted
        const auto it = qBinaryFind( list_.begin() + 1, list_.end(), name );
        if( it == list_.end() )
            return QModelIndex();
        const auto idx = std::distance( list_.begin(), it );
        return index( static_cast< int >( idx ), 0 );
    }

    const QStringList list_;
};

namespace
{
    QLayoutItem* ClearLayoutItem( QLayoutItem* item )
    {
        if( auto layout = item->layout() )
            for( int i = 0; i < layout->count(); ++i )
                ClearLayoutItem( layout->itemAt( i ) );
        delete item->widget();
        return item;
    }

    void ClearGrid( QGridLayout* grid, int size, int row )
    {
        while( auto child = grid->takeAt( size ) )
            delete ClearLayoutItem( child );
        for( int i = row; i < grid->rowCount(); ++i )
        {
            grid->setRowMinimumHeight( i, 0 );
            grid->setRowStretch( i, 0 );
        }
    }

    void SetHorizontalPolicy( QWidget* w, QSizePolicy::Policy value )
    {
        auto policy = w->sizePolicy();
        policy.setHorizontalPolicy( value );
        w->setSizePolicy( policy );
    }

    QStringList GetShooters( const kernel::StaticModel& model )
    {
        QStringList names;
        model.types_.tools::Resolver< kernel::AgentType >::Apply(
            [&]( const kernel::AgentType& agent )
            {
                names << QString::fromStdString( agent.GetLocalizedName() );
            } );
        return names;
    }

    QStringList GetAmmos( const kernel::StaticModel& model )
    {
        QStringList names;
        model.objectTypes_.Resolver2< kernel::DotationType >::Apply(
            [&]( const kernel::DotationType& dot )
            {
                if( dot.IsIndirectFireAmmunition() )
                    names << QString::fromStdString( dot.GetName() );
            } );
        return names;
    }

    QStringList GetNames( FireGroup group, const kernel::StaticModel& model )
    {
        if( group == FIRE_GROUP_DIRECT )
            return GetShooters( model );
        return GetAmmos( model );
    }

    typedef std::function< bool( const FireOption& current, const QString& name ) > T_Accepter;

    class Filter : public QSortFilterProxyModel
    {
    public:
        Filter( const FireOption* fire,
                const T_Accepter& accept,
                QObject* parent )
            : QSortFilterProxyModel( parent )
            , fire_( fire )
            , accept_( accept )
        {
            // NOTHING
        }

        bool filterAcceptsRow( int row, const QModelIndex& root ) const
        {
            const auto model = sourceModel();
            const auto index = model->index( row, 0, root );
            const auto name = model->data( index ).toString();
            return accept_( *fire_, name );
        }

        const FireOption* fire_;
        T_Accepter accept_;
    };

    class Filters : public QObject
    {
    public:
        Filters( const T_FireOptions& opts, QObject* parent = 0 )
            : QObject( parent )
            , opts_( opts )
        {
            InvalidateCache();
        }

        QSortFilterProxyModel* MakeFilter( const FireOption* fire,
                                           QAbstractItemModel* model,
                                           const T_Accepter& accept )
        {
            auto filter = new Filter( fire, accept, this );
            filter->setSourceModel( model );
            filters_.push_back( filter );
            return filter;
        }

        void InvalidateCache()
        {
            cache_.clear();
            for( auto it = opts_.begin(); it != opts_.end(); ++it )
                cache_.insert( std::make_pair( it->name, &*it ) );
        }

        void Invalidate()
        {
            InvalidateCache();
            for( auto it = filters_.begin(); it != filters_.end(); ++it )
                ( *it )->invalidate();
        }

        const T_FireOptions& opts_;
        std::vector< Filter* > filters_;
        std::multimap< QString, const FireOption* > cache_;
    };

    QString GetKarmaName( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::unknown_ )
            return FireOption::GetAny();
        return karma.GetName();
    }

    kernel::Karma GetKarma( const QString& name )
    {
        if( name == FireOption::GetAny() )
            return kernel::Karma::unknown_;
        return kernel::Karma::ResolveName( name );
    }

    void SetCurrent( QComboBox* w, QSortFilterProxyModel* proxy, Model* model, const QString& name )
    {
        w->setCurrentIndex( proxy->mapFromSource( model->GetIndex( name ) ).row() );
    }
}

void FireColorPanel::FillGrid()
{
    auto grid = ctx_.grid;
    auto fires = &ctx_.data;
    ClearGrid( grid, 3, 1 );
    if( fires->empty() || !ctx_.names )
    {
        Save();
        return;
    }
    const auto filters = new Filters( *fires );
    const auto acceptNames =
        [=]( const FireOption& fire, const QString& name ) -> bool
        {
            if( name == FireOption::GetAny() )
                return true; // always accept any
            if( fire.name == name )
                return true; // always accept current choice
            auto range = filters->cache_.equal_range( name );
            for( auto it = range.first; it != range.second; ++it )
                if( it->second->karma == fire.karma )
                    return false;
            return true;
        };
    const auto acceptKarmas =
        [=]( const FireOption& fire, const QString& name ) -> bool
        {
            const auto current = GetKarma( name );
            if( fire.karma == current )
                return true; // always accept current choice
            auto range = filters->cache_.equal_range( fire.name );
            for( auto it = range.first; it != range.second; ++it )
                if( it->second->karma == current )
                    return false;
            return true;
        };
    int row = 1;
    static const auto trash = MAKE_PNG_ICON( "trash" );
    for( auto it = fires->begin(); it != fires->end(); ++it, ++row )
    {
        FireOption* fire = &*it;
        if( fire->name != FireOption::GetAny() && !ctx_.names->Contains( fire->name ) )
            continue;

        int col = 0;
        auto qname = new QComboBox();
        auto nameProxy = filters->MakeFilter( fire, ctx_.names, acceptNames );
        qname->setModel( nameProxy );
        SetCurrent( qname, nameProxy, ctx_.names, fire->name );
        gui::connect( qname, SIGNAL( activated( int ) ),
            [=]
            {
                fire->name = qname->currentText();
                filters->Invalidate();
                Save();
            } );
        qname->setMinimumWidth( 50 );
        grid->addWidget( qname, row, col++ );

        auto qkarma = new QComboBox();
        auto karmaProxy = filters->MakeFilter( fire, ctx_.karmas, acceptKarmas );
        qkarma->setModel( karmaProxy );
        SetCurrent( qkarma, karmaProxy, ctx_.karmas, GetKarmaName( fire->karma ) );
        grid->addWidget( qkarma, row, col++ );
        gui::connect( qkarma, SIGNAL( activated( int ) ),
            [=]
            {
                fire->karma = GetKarma( qkarma->currentText() );
                filters->Invalidate();
                Save();
            } );

        auto buttons = new QWidget();
        auto layout = new QHBoxLayout( buttons );
        grid->addWidget( buttons, row, col++ );

        auto qcolor = new ColorButton( fire->name + "-color" );
        qcolor->SetColor( fire->color );
        qcolor->setMaximumSize( 28, 24 );
        layout->addWidget( qcolor );
        gui::connect( qcolor, SIGNAL( ColorChanged( const QColor& ) ),
            [=]
            {
                fire->color = qcolor->GetColor();
                Save();
            } );

        auto remove = new QPushButton();
        SetHorizontalPolicy( remove, QSizePolicy::Maximum );
        remove->setIcon( trash );
        remove->setIconSize( QSize( 14, 14 ) );
        layout->addWidget( remove );
        gui::connect( remove, SIGNAL( clicked() ),
            [=]
            {
                // works only because we regenerate all widgets as soon
                // one is deleted or row indexes would be outdated
                RemoveFire( row - 1 );
                // no need to save here cause RemoveFire calls fillGrid
            } );
    }

    // make sure filters are destroyed along with widgets
    filters->setParent( grid->itemAt( grid->count() - 1 )->widget() );
    Save();
}

void FireColorPanel::SetupGroup( QLayout* root,
                                 const QString& name )
{
    auto frame = new QWidget();
    root->addWidget( frame );
    auto nameLabel = new QLabel();
    nameLabel->setText( name );
    auto karmaLabel = new QLabel();
    karmaLabel->setText( "Karma" );

    auto clear = new QPushButton();
    clear->setText( tr( "Clear All" ) );
    SetHorizontalPolicy( clear, QSizePolicy::Maximum );
    QObject::connect( clear, SIGNAL( clicked() ), this, SLOT( ClearAll() ) );

    auto add = new QPushButton();
    add->setText( tr( "Add Rule" ) );
    SetHorizontalPolicy( add, QSizePolicy::Maximum );
    QObject::connect( add, SIGNAL( clicked() ), this, SLOT( AddRule() ) );

    auto right = new QWidget();
    auto rlayout = new QHBoxLayout( right );
    rlayout->addWidget( clear );
    rlayout->addWidget( add );

    ctx_.grid = new QGridLayout( frame );
    int row = 0;
    ctx_.grid->addWidget( nameLabel, 0, row++ );
    ctx_.grid->addWidget( karmaLabel, 0, row++ );
    ctx_.grid->addWidget( right, 0, row++ );
    ctx_.names = nullptr;
    ctx_.karmas = nullptr;
    FillGrid();
}

void FireColorPanel::SetupUi( QScrollArea* parent )
{
    auto frame = new QWidget();
    parent->setWidget( frame );
    auto layout = new QVBoxLayout( frame );
    SetupGroup( layout, group_ == FIRE_GROUP_DIRECT ? tr( "Shooter" ) : tr( "Ammunition" ) );
    layout->addStretch();
}

void FireColorPanel::Save()
{
    options_.Change(
        FireOption::GetOptionName( group_ ),
        SaveFireOptions( ctx_.data ) );
}

void FireColorPanel::AddRule()
{
    ctx_.data.push_front( FireOption() );
    FillGrid();
}

void FireColorPanel::ClearAll()
{
    ctx_.data.clear();
    FillGrid();
}

void FireColorPanel::RemoveFire( int idx )
{
    ctx_.data.erase( ctx_.data.begin() + idx );
    FillGrid();
}

void FireColorPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    auto names = GetNames( group_, model_ );
    names.sort();
    names.push_front( FireOption::GetAny() );
    ctx_.names = new Model( names, this );

    QStringList karmas;
    karmas << kernel::Karma::friend_.GetName()
        << kernel::Karma::enemy_.GetName()
        << kernel::Karma::neutral_.GetName();
    karmas.sort();
    karmas.push_front( FireOption::GetAny() );
    ctx_.karmas = new Model( karmas, this );
}

void FireColorPanel::Load( const GlProxy& view )
{
    const auto& value = view.GetOptions().GetOptions()->Get( FireOption::GetOptionName( group_ ) );
    ctx_.data = LoadFireOptions( value.To< QString >() );
    FillGrid();
}
