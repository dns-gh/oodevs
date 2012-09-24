// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanListView.h"
#include "moc_UrbanListView.cpp"
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/VolumeType.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/ListItemToolTip.h"
#include "clients_gui/SearchListView.h"
#include "clients_gui/SymbolIcon.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/SymbolIcons.h"
#include "preparation/StaticModel.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanMenuManager.h"
#include "preparation/UrbanModel.h"
#include "preparation/Usages.h"
#include <boost/bind.hpp>

#define ICON_COLUMN_SIZE 25

// -----------------------------------------------------------------------------
// Name: UrbanListView constructor
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanListView::UrbanListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder, gui::SymbolIcons& symbols, const StaticModel& staticModel, UrbanModel& urbanModel )
    : EntityListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , controllers_   ( controllers )
    , modelBuilder_  ( modelBuilder )
    , symbols_       ( symbols )
    , staticModel_   ( staticModel )
    , urbanModel_    ( urbanModel )
    , entitySelected_( controllers )
{
    viewport()->installEventFilter( new ListItemToolTip( viewport(), *this ) );

    timer_ = new QTimer( this );

    addColumn( tr( "Urban" ) );
    addColumn( "HiddenPuce", static_cast< int >( ICON_COLUMN_SIZE * 1.5 ) );
    setColumnAlignment( 1, Qt::AlignCenter );
    setResizeMode( NoColumn );

    setDragAutoScroll( true );
    setAcceptDrops( true );
    viewport()->setAcceptDrops( true );
    controllers_.Register( *this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( Update() ) );
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView destructor
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanListView::~UrbanListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyCreated
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyCreated( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyCreated
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyCreated( const kernel::UrbanObject_ABC& object )
{
    const kernel::Hierarchies* hierarchies = object.Retrieve< kernel::Hierarchies >();
    if( !hierarchies ) // $$$$ ABR 2012-05-22: Only urban block have hierarchies
        return;
    const kernel::Entity_ABC* superior = hierarchies->GetSuperior();
    gui::ValuedListItem* item = 0;
    if( superior )
    {
        if( gui::ValuedListItem* superiorItem = gui::FindItem( superior, firstChild() ) )
            item = CreateItem( superiorItem );
    }
    else
        item = CreateItem( this );
    if( item )
    {
        item->SetNamed( static_cast< const kernel::Entity_ABC& >( object ) );
        item->setRenameEnabled( 0, GetCurrentMode() == ePreparationMode_Terrain );
        UpdatePixmap( object, item );
        item->SetToolTip( QString( "%1 [%L2]" ).arg( object.GetName() ).arg( object.GetId() ) );
        if( superior )
            item->setDragEnabled( true );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyDeleted
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanListView::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    delete gui::FindItem( static_cast< const kernel::Entity_ABC* >( &object ), firstChild() );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::IsTypeRejected
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
bool UrbanListView::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != kernel::UrbanObject_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyModeChanged
// Created: JSR 2012-05-24
// -----------------------------------------------------------------------------
void UrbanListView::NotifyModeChanged( int newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    bool isTerrainMode = newMode == ePreparationMode_Terrain;
    setSelectionMode( isTerrainMode ? Q3ListView::Extended : Q3ListView::Single );
    Q3ListViewItemIterator it( this );
    while( it.current() )
    {
        it.current()->setRenameEnabled( 0, isTerrainMode );
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::OnContextMenuRequested
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int col )
{
    if( !IsReadOnly() )
    {
        if( i == 0 && GetCurrentMode() == ePreparationMode_Terrain )
        {
            QMenu* menu = new QMenu( this );
            menu->addAction( tr( "Create city" ), this, SLOT( OnCreateCity() ) );
            menu->popup( pos );
        }
        else
            gui::EntityListView::OnContextMenuRequested( i, pos, col );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::OnCreateCity
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::OnCreateCity()
{
    modelBuilder_.CreateCityOrDistrict( 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::keyPressEvent
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanListView::keyPressEvent( QKeyEvent* key )
{
    if( GetCurrentMode() == ePreparationMode_Terrain && key->key() == Qt::Key_Delete )
    {
        std::vector< const kernel::UrbanObject_ABC* > blocks;
        Q3ListViewItemIterator it( this );
        while( gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( *it ) )
        {
            if( item->isSelected() && item->IsA< const kernel::Entity_ABC >() )
                blocks.push_back( static_cast< const kernel::UrbanObject_ABC* >( item->GetValueNoCheck< const kernel ::Entity_ABC >() ) );
            ++it;
        }
        if( !blocks.empty() )
        {
            modelBuilder_.DeleteBlocks( blocks );
            key->accept();
            return;
        }
    }
    gui::EntityListView::keyPressEvent( key );
}

namespace
{
    struct sUrbanDrag {};
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::dragObject
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
Q3DragObject* UrbanListView::dragObject()
{
    if( GetCurrentMode() != ePreparationMode_Terrain || IsDragAndDropLocked() )
        return 0;
    gui::EntityListView::dragObject();
    dragged_.clear();
    EUrbanLevel level = static_cast< EUrbanLevel >( -1 );
    Q3ListViewItemIterator it( this );
    while( gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( *it ) )
    {
        if( item->isSelected() && item->IsA< const kernel::Entity_ABC >() )
        {
            kernel::Entity_ABC* entity = const_cast< kernel::Entity_ABC* >( item->GetValueNoCheck< const kernel::Entity_ABC >() );
            if( entity )
            {
                UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() );
                if( level != static_cast< EUrbanLevel >( -1 ) && level != hierarchies.GetLevel() )
                {
                    dragged_.clear();
                    return 0;
                }
                level = hierarchies.GetLevel();
                dragged_.push_back( entity );
            }
        }
        ++it;
    }
    if( dragged_.empty() )
        return 0;
    dnd::CreateDragObject( static_cast< sUrbanDrag* >( 0 ), this );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::Drop
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
kernel::Entity_ABC* UrbanListView::Drop( QDropEvent* pEvent ) const
{
    if( !dragged_.empty() && dnd::HasData< sUrbanDrag >( pEvent ) )
    {
        QPoint position = viewport()->mapFromParent( pEvent->pos() );
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( itemAt( position ) );
        if( item && item->IsA< const kernel::Entity_ABC >() )
        {
            kernel::Entity_ABC* entity = const_cast< kernel::Entity_ABC* >( item->GetValueNoCheck< const kernel::Entity_ABC >() );
            if( entity )
            {
                EUrbanLevel level = static_cast< const UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() ).GetLevel();
                EUrbanLevel draggedLevel = static_cast< const UrbanHierarchies& >( dragged_.front()->Get< kernel::Hierarchies >() ).GetLevel();
                if( ( draggedLevel == eUrbanLevelBlock && level == eUrbanLevelDistrict ) || ( draggedLevel == eUrbanLevelDistrict && level == eUrbanLevelCity ) )
                {
                    pEvent->accept();
                    return entity;
                }
            }
        }
    }
    pEvent->ignore();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDragEnterEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDragEnterEvent( QDragEnterEvent* pEvent )
{
    EntityListView::viewportDragEnterEvent( pEvent );
    pEvent->accept( dnd::HasData< sUrbanDrag >( pEvent ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDragMoveEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDragMoveEvent( QDragMoveEvent *pEvent )
{
    EntityListView::viewportDragMoveEvent( pEvent );
    Drop( pEvent );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportDropEvent
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanListView::viewportDropEvent( QDropEvent* pEvent )
{
    EntityListView::viewportDropEvent( pEvent );
    kernel::Entity_ABC* target = Drop( pEvent );
    if( target )
    {
        for( std::vector< kernel::Entity_ABC* >::const_iterator it = dragged_.begin(); it != dragged_.end(); ++it )
        {
            kernel::UrbanObject_ABC* object = static_cast< kernel::UrbanObject_ABC* >( *it );
            UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() );
            kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( hierarchies.GetSuperior() );
            if( !superior || superior == target )
                continue;
            superior->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
            NotifyDeleted( *object );
            static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() ).ChangeSuperior( *target );
            NotifyCreated( *object );
            tools::Iterator< const kernel::Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
            while( subIt.HasMoreElements() )
            {
                const kernel::UrbanObject_ABC& child = static_cast< const kernel::UrbanObject_ABC& >( subIt.NextElement() );
                NotifyCreated( child );
            }
        }
        target->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        controllers_.actions_.SetSelected( *target, false );
    }
    dragged_.clear();
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::Update
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void UrbanListView::Update()
{
    UpdateItems( static_cast< gui::ValuedListItem* >( firstChild() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::UpdateItem
// Created: SBO 2010-08-03
// -----------------------------------------------------------------------------
void UrbanListView::UpdateItems( gui::ValuedListItem* root )
{
    while( root )
    {
        UpdateItems( static_cast< gui::ValuedListItem* >( root->firstChild() ) );
        if( root->IsA< const kernel::Entity_ABC >() )
        {
            const kernel::UrbanObject_ABC& entity = *root->GetValueNoCheck< const kernel::UrbanObject_ABC >();
            UpdatePixmap( entity, root );
        }
        root = static_cast< gui::ValuedListItem* >( root->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyUpdated
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void UrbanListView::NotifyUpdated( const kernel::UrbanObject_ABC& object )
{
    if( gui::ValuedListItem* item = gui::FindItem( &static_cast< const kernel::Entity_ABC& >( object ), firstChild() ) )
    {
        item->SetNamed( static_cast< const kernel::Entity_ABC& >( object ) );
        UpdatePixmap( object, item );
        item->SetToolTip( QString( "%1 [%L2]" ).arg( object.GetName() ).arg( object.GetId() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::UpdatePixmap
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void UrbanListView::UpdatePixmap( const kernel::UrbanObject_ABC& object, gui::ValuedListItem* item )
{
    assert( item );
    QPixmap pixmap( ICON_COLUMN_SIZE, ICON_COLUMN_SIZE );
    pixmap.fill( Qt::transparent );
    if( const kernel::Infrastructure_ABC* infra = object.Retrieve< kernel::Infrastructure_ABC >() )
    {
        const kernel::InfrastructureType* infraType = infra->GetType();
        if( infraType )
        {
            gui::SymbolIcon icon( infraType->GetSymbol() );
            icon.SetSize( ICON_COLUMN_SIZE );
            pixmap = symbols_.GetSymbol( icon );
            if( pixmap.isNull() )
            {
                timer_->start( 500, true );
                return;
            }
        }
    }
    pixmap.resize( ICON_COLUMN_SIZE, ICON_COLUMN_SIZE );
    item->setPixmap( 1, pixmap );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::viewportResizeEvent
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void UrbanListView::viewportResizeEvent( QResizeEvent* pEvent )
{
    Q3ListView::viewportResizeEvent( pEvent );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::setColumnWidth
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void UrbanListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

namespace
{
    std::string InfrastructureExtractor( const kernel::Entity_ABC& object, bool& valid, bool& empty )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const kernel::Infrastructure_ABC& infra = object.Get< kernel::Infrastructure_ABC >();
            if( infra.GetType() )
                return infra.GetType()->GetName();
            else
                empty = true;
        }
        else
            valid = false;
        return "";
    }

    std::string MaterialExtractor( const kernel::Entity_ABC& object, bool& valid, bool& /* empty */ )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const kernel::PhysicalAttribute_ABC& physical = object.Get< kernel::PhysicalAttribute_ABC >();
            const kernel::Architecture_ABC& architecture = physical.GetArchitecture();
            return architecture.GetMaterial().GetName();
        }
        valid = false;
        return "";
    }

    std::string RoofShapeExtractor( const kernel::Entity_ABC& object, bool& valid, bool& /* empty */ )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const kernel::PhysicalAttribute_ABC& physical = object.Get< kernel::PhysicalAttribute_ABC >();
            const kernel::Architecture_ABC& architecture = physical.GetArchitecture();
            return architecture.GetRoofShape().GetName();
        }
        else
            valid = false;
        return "";
    }

    std::string UsageExtractor( const kernel::Entity_ABC& object, bool& valid, bool& /* empty */ )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        std::string result = "";
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const kernel::PhysicalAttribute_ABC& physical = object.Get< kernel::PhysicalAttribute_ABC >();
            const kernel::Usages_ABC& usagesExtension = physical.GetUsages();
            kernel::T_Usages usages = usagesExtension.GetUsages();
            for( kernel::CIT_Usages it = usages.begin(); it != usages.end(); ++it )
            {
                if( it->second == 0 )
                    continue;
                result += ( !result.empty() ) ? ";" + it->first : it->first;
            }
        }
        valid = !result.empty();
        return result;
    }

    std::string ResourceNetworkExtractor( const kernel::Entity_ABC& object, bool& valid, bool& empty )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        std::string result = "";
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const kernel::ResourceNetwork_ABC& resourceAttribute = object.Get< kernel::ResourceNetwork_ABC >();
            const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes = resourceAttribute.GetResourceNodes();
            if( nodes.empty() )
                empty = true;
            else
                for( kernel::ResourceNetwork_ABC::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it )
                    result += ( !result.empty() ) ? ";" + it->first : it->first;
        }
        else
            valid = false;
        return result;
    }

    double AreaExtractor( const kernel::Entity_ABC& object, bool& valid )
    {
        if( const UrbanHierarchies* hierarchy = static_cast< const UrbanHierarchies* >( object.Retrieve< kernel::Hierarchies >() ) )
            if( hierarchy->GetLevel() == eUrbanLevelBlock )
                if( const kernel::UrbanPositions_ABC* position = object.Retrieve< kernel::UrbanPositions_ABC >() )
                    return position->ComputeArea();
        valid = false;
        return 0.f;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::CreateFilters
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void UrbanListView::CreateFilters( gui::SearchListView_ABC& searchListView )
{
    searchListView.AddResolverFilter< kernel::InfrastructureType >( tr( "Infrastructure" ), staticModel_.objectTypes_, boost::bind( &::InfrastructureExtractor, _1, _2, _3 ), &kernel::InfrastructureType::GetName, tr( "None" ) );
    searchListView.AddResolverFilter< kernel::AccommodationType >( tr( "Usages" ), staticModel_.accommodationTypes_, boost::bind( &::UsageExtractor, _1, _2, _3 ), &kernel::AccommodationType::GetRole );
    searchListView.AddResolverFilter< kernel::ResourceNetworkType >( tr( "Resource network" ), staticModel_.objectTypes_, boost::bind( &::ResourceNetworkExtractor, _1, _2, _3 ), &kernel::ResourceNetworkType::GetName, tr( "None" ) );
    searchListView.AddResolverFilter< kernel::MaterialCompositionType >( tr( "Material" ), staticModel_.objectTypes_, boost::bind( &::MaterialExtractor, _1, _2, _3 ), &kernel::MaterialCompositionType::GetName );
    searchListView.AddResolverFilter< kernel::RoofShapeType >( tr( "RoofShape" ), staticModel_.objectTypes_, boost::bind( &::RoofShapeExtractor, _1, _2, _3 ), &kernel::RoofShapeType::GetName );
    searchListView.AddNumericFilter< double, gui::RichDoubleSpinBox >( tr( "Area" ), boost::bind( &::AreaExtractor, _1, _2 ), 0, 10000000 ); // $$$$ ABR 2012-06-20: 10 million seems enought for urban blocs area...
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifyContextMenu
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void UrbanListView::NotifyContextMenu( const kernel::Entity_ABC& /*element*/, kernel::ContextMenu& menu )
{
    if( GetCurrentMode() == ePreparationMode_Terrain )
        if( entitySelected_ || !isMultiSelection() )
            menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::OnRename
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void UrbanListView::OnRename()
{
    if( !isMultiSelection() && selectedItem() )
        selectedItem()->startRename( 0 );
    if( isMultiSelection() && entitySelected_ )
         if( gui::ValuedListItem* item = gui::FindItem( entitySelected_, firstChild() ) )
             item->startRename( 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanListView::NotifySelectionChanged
// Created: LGY 2012-08-29
// -----------------------------------------------------------------------------
void UrbanListView::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    EntityListView::NotifySelectionChanged( elements );
    entitySelected_ = elements.size() == 1 ? elements.front() : 0;
}
