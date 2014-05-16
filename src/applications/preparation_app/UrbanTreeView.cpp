// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanTreeView.h"
#include "moc_UrbanTreeView.cpp"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_gui/StandardModelVisitor_ABC.h"
#include "clients_gui/RichView.h"
#include "clients_gui/SymbolIcon.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "preparation/StaticModel.h"
#include "preparation/UrbanHierarchies.h"
#include "ENT/ENT_Enums.h"
#include <boost/bind.hpp>

const QString UrbanTreeView::mimeType_( typeid( kernel::UrbanObject_ABC ).name() );

// -----------------------------------------------------------------------------
// Name: UrbanTreeView constructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
UrbanTreeView::UrbanTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, gui::SymbolIcons& symbols, const StaticModel& staticModel, QWidget* parent /*= 0*/ )
    : gui::EntityTreeView_ABC( objectName, controllers, profile, modelObserver, parent )
    , symbols_( symbols )
    , staticModel_( staticModel )
{
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    setItemDelegate( new gui::ItemPixmapDelegate( dataModel_, boost::bind( &UrbanTreeView::GetEntityPixmap, this, _1 ), this, 24 ) );
    SetLessThanEntityFunctor( &tools::LessThanById );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView destructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
UrbanTreeView::~UrbanTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyCreated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyCreated( const kernel::UrbanObject_ABC& object )
{
    const kernel::Hierarchies* hierarchies = object.Retrieve< kernel::Hierarchies >();
    if( !hierarchies ) // $$$$ ABR 2012-05-22: Only urban block have hierarchies
        return;
    const kernel::Entity_ABC* superior = static_cast< const kernel::UrbanObject_ABC* >( hierarchies->GetSuperior() );
    QStandardItem* item = 0;
    if( superior )
    {
        if( QStandardItem* supItem = dataModel_.FindDataItem( *superior ) )
        {
            const int row = supItem->rowCount();
            item = dataModel_.AddChildSafeItem( supItem, row, 0, object.GetName(), object.GetTooltip(), object, Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled );
            dataModel_.AddChildItem( supItem, row, 1 );
        }
    }
    else
    {
        const int row = dataModel_.rowCount();
        item = dataModel_.AddRootSafeItem( row, 0, object.GetName(), object.GetTooltip(), object, Qt::ItemIsDropEnabled );
        dataModel_.AddRootItem( row, 1 );
    }
    if( item )
        item->setEditable( GetCurrentMode() == eModes_Terrain );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyUpdated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyUpdated( const kernel::UrbanObject_ABC& /*object*/ )
{
    doItemsLayout();
}

namespace
{
    class ModelVisitor : public gui::StandardModelVisitor_ABC
                       , private boost::noncopyable
    {
    public:
        ModelVisitor( bool enable ) : enable_( enable ) {}
        ~ModelVisitor() {}
        virtual void Visit( QStandardItem& item )
        {
            item.setEditable( enable_ );
        }
    private:
        bool enable_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyCreated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyCreated( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyModeChanged
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyModeChanged( E_Modes newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    bool isTerrainMode = newMode == eModes_Terrain;
    setSelectionMode( isTerrainMode ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection );
    EnableDragAndDrop( isTerrainMode );
    ModelVisitor visitor( isTerrainMode );
    dataModel_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::OnCreateCity
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::OnCreateCity()
{
    modelObserver_.CreateCityOrDistrict( 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::GetEntityPixmap
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
std::vector< const QPixmap* > UrbanTreeView::GetEntityPixmap( const kernel::Entity_ABC& object )
{
    std::vector< const QPixmap* > ret;
    if( auto infra = object.Retrieve< gui::Infrastructure_ABC >() )
    {
        const kernel::InfrastructureType* infraType = infra->GetType();
        if( infraType )
        {
            gui::SymbolIcon icon( infraType->GetSymbol() );
            icon.SetSize( 24 );
            const QPixmap& pixmap = symbols_.GetSymbol( icon );
            if( !pixmap.isNull() )
            {
                ret.push_back( &pixmap );
                return ret;
            }
            doItemsLayout();
        }
    }
    return ret;
}

namespace
{
    std::string InfrastructureExtractor( const kernel::Entity_ABC& object, bool& valid, bool& empty )
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( urbanHierarchies.GetLevel() == eUrbanLevelBlock )
        {
            const auto& infra = object.Get< gui::Infrastructure_ABC >();
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
            const kernel::T_Usages& usages = usagesExtension.GetUsages();
            for( kernel::CIT_Usages it = usages.begin(); it != usages.end(); ++it )
            {
                if( it->second == 0 )
                    continue;
                result += result.empty() ? it->first : ";" + it->first;
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
            const auto& resourceAttribute = object.Get< gui::ResourceNetwork_ABC >();
            const auto& nodes = resourceAttribute.GetResourceNodes();
            if( nodes.empty() )
                empty = true;
            else
                for( auto it = nodes.begin(); it != nodes.end(); ++it )
                    result += result.empty() ? it->first : ";" + it->first;
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

    std::string ModelWrapper( const gui::StandardModel& model,
                              const std::function< std::string( const kernel::Entity_ABC&, bool&, bool& ) >& functor,
                              const QStandardItem& item,
                              bool& valid,
                              bool& empty )
    {
        if( auto entity = model.GetDataFromItem< kernel::Entity_ABC >( item ) )
            return functor( *entity, valid, empty );
        valid = false;
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::CreateFilters
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::CreateFilters( gui::RichView_ABC& richView )
{
    richView.AddCheckComboBoxFilter( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "Infrastructure" ), true, true,
                                     boost::bind( &ModelWrapper, boost::cref( dataModel_ ), &InfrastructureExtractor,  _1, _2, _3 ),
                                     boost::bind( &gui::CheckComboBox::FillFromResolver< kernel::InfrastructureType, std::string >, _1, boost::cref( staticModel_.objectTypes_ ), &kernel::InfrastructureType::GetName ) );
    richView.AddCheckComboBoxFilter( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "Usages" ), false, true,
                                     boost::bind( &ModelWrapper, boost::cref( dataModel_ ), &UsageExtractor,  _1, _2, _3 ),
                                     boost::bind( &gui::CheckComboBox::FillFromResolver< kernel::AccommodationType, std::string >, _1, boost::cref( staticModel_.accommodationTypes_ ), &kernel::AccommodationType::GetRole ) );
    richView.AddCheckComboBoxFilter( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "Resource network" ), true, true,
                                     boost::bind( &ModelWrapper, boost::cref( dataModel_ ), &ResourceNetworkExtractor,  _1, _2, _3 ),
                                     boost::bind( &gui::CheckComboBox::FillFromResolver< kernel::ResourceNetworkType, std::string >, _1, boost::cref( staticModel_.objectTypes_ ), &kernel::ResourceNetworkType::GetName ) );
    richView.AddCheckComboBoxFilter( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "Material" ), false, true,
                                     boost::bind( &ModelWrapper, boost::cref( dataModel_ ), &MaterialExtractor,  _1, _2, _3 ),
                                     boost::bind( &gui::CheckComboBox::FillFromResolver< kernel::MaterialCompositionType, std::string >, _1, boost::cref( staticModel_.objectTypes_ ), &kernel::MaterialCompositionType::GetName ) );
    richView.AddCheckComboBoxFilter( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "RoofShape" ), false, true,
                                     boost::bind( &ModelWrapper, boost::cref( dataModel_ ), &RoofShapeExtractor,  _1, _2, _3 ),
                                     boost::bind( &gui::CheckComboBox::FillFromResolver< kernel::RoofShapeType, std::string >, _1, boost::cref( staticModel_.objectTypes_ ), &kernel::RoofShapeType::GetName ) );
    richView.AddNumericFilter< double, gui::RichDoubleSpinBox >( 0, gui::RichView_ABC::eFilterAreas_Box, tr( "Area" ), [&]( const QStandardItem& item, bool& valid ) -> double {
                                                                     if( auto entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( item ) )
                                                                         return AreaExtractor( *entity, valid );
                                                                     valid = false;
                                                                     return 0.f;
                                                                 },
                                                                 0, 10000000 ); // $$$$ ABR 2012-06-20: 10 million seems enought for urban blocs area...
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::ContextMenuRequested
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::ContextMenuRequested( const QPoint& where )
{
    if( !IsReadOnly() && GetCurrentMode() == eModes_Terrain )
    {
        kernel::ContextMenu* menu = new kernel::ContextMenu( this );
        menu->insertItem( tr( "Create city" ), this, SLOT( OnCreateCity() ) );
        menu->exec( where );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::IsTypeRejected
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
bool UrbanTreeView::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != kernel::UrbanObject_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::MimeTypes
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
QStringList UrbanTreeView::MimeTypes() const
{
    static QStringList list( mimeType_ );
    return list;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::MimeData
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
QMimeData* UrbanTreeView::MimeData( const QModelIndexList& indexes, bool& overriden ) const
{
    overriden = true;
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream( &encodedData, QIODevice::WriteOnly );
    EUrbanLevel level = static_cast< EUrbanLevel >( -1 );
    foreach( QModelIndex index, indexes )
    {
        if( index.isValid() )
        {
            const kernel::UrbanObject_ABC* entity = dataModel_.GetDataFromIndex< kernel::UrbanObject_ABC >( index );
            if( entity )
            {
                const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() );
                if( level != static_cast< EUrbanLevel >( -1 ) && level != hierarchies.GetLevel() )
                {
                    delete mimeData;
                    return 0;
                }
                level = hierarchies.GetLevel();
                QStandardItem* item = dataModel_.itemFromIndex( index.model() == &dataModel_ ? index : proxyModel_->mapToSource( index ) );
                if( item )
                    stream << reinterpret_cast< unsigned int >( item->data( gui::Roles::DataRole ).value< kernel::VariantPointer >().ptr_ );
            }
        }
    }
    mimeData->setData( mimeType_, encodedData );
    return mimeData;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::dragMoveEvent
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::dragMoveEvent( QDragMoveEvent *pEvent )
{
    QTreeView::dragMoveEvent( pEvent );
    kernel::UrbanObject_ABC* target = dataModel_.GetDataFromIndex< kernel::UrbanObject_ABC >( indexAt( pEvent->pos() ) );
    bool accept = false;
    if( !IsReadOnly() && target )
    {
        EUrbanLevel levelTarget = static_cast< const UrbanHierarchies& >( target->Get< kernel::Hierarchies >() ).GetLevel();
        if( const kernel::UrbanObject_ABC* entity = dnd::FindSafeData< kernel::UrbanObject_ABC >( pEvent ) )
        {
            EUrbanLevel level = static_cast< const UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() ).GetLevel();
            if( ( level == eUrbanLevelBlock && levelTarget == eUrbanLevelDistrict ) || ( level == eUrbanLevelDistrict && levelTarget == eUrbanLevelCity ) )
                accept = true;
        }
    }
    if( accept )
        pEvent->accept();
    else
        pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::Drop
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::Drop( const QString& mimeType, void* data, QStandardItem& target )
{
    if( IsReadOnly() )
        return;
    kernel::UrbanObject_ABC* entityTarget = dataModel_.GetDataFromItem< kernel::UrbanObject_ABC >( target );
    if( !entityTarget )
        return;
    if( mimeType == mimeType_ )
    {
        kernel::SafePointer< kernel::UrbanObject_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< kernel::UrbanObject_ABC >* >( data );
        if( !safePtr )
            return;
        kernel::UrbanObject_ABC* object = safePtr->ConstCast();
        if( !object )
            return;
        UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() );
        UrbanHierarchies& targetHierarchies = static_cast< UrbanHierarchies& >( entityTarget->Get< kernel::Hierarchies >() );
        kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( hierarchies.GetSuperior() );
        if( !superior || superior == entityTarget )
            return;
        EUrbanLevel objectLevel = hierarchies.GetLevel();
        EUrbanLevel targetLevel = targetHierarchies.GetLevel();
        if( objectLevel == eUrbanLevelCity || ( objectLevel == eUrbanLevelBlock && targetLevel != eUrbanLevelDistrict )
                                           || ( objectLevel == eUrbanLevelDistrict && targetLevel != eUrbanLevelCity ) )
            return;
        superior->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        QStandardItem* superiorItem = dataModel_.FindDataItem( *superior );
        if( !superiorItem )
            return;
        QModelIndex index = dataModel_.indexFromItem( dataModel_.FindDataItem( *object ) );
        QList< QStandardItem* > rowItems = superiorItem->takeRow( index.row() );
        for( QList< QStandardItem *>::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
            delete *it;
        hierarchies.ChangeSuperior( *entityTarget );
        NotifyCreated( *object );
        tools::Iterator< const kernel::Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const kernel::UrbanObject_ABC& child = static_cast< const kernel::UrbanObject_ABC& >( subIt.NextElement() );
            NotifyCreated( child );
        }
        entityTarget->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        controllers_.actions_.SetSelected( *entityTarget, false );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::keyPressEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void UrbanTreeView::keyPressEvent( QKeyEvent* event )
{
    if( GetCurrentMode() == eModes_Terrain && event->key() == Qt::Key_Delete )
    {
        QModelIndexList list = selectionModel()->selectedIndexes();
        std::vector< const kernel::UrbanObject_ABC* > blocks;
        for( QModelIndexList::const_iterator it = list.begin(); it != list.end(); ++it )
            if( const kernel::UrbanObject_ABC* entity = dataModel_.GetDataFromIndex< kernel::UrbanObject_ABC >( *it ) )
                blocks.push_back( entity );
        if( !blocks.empty() )
        {
            modelObserver_.DeleteBlocks( blocks );
            return;
        }
    }
    gui::EntityTreeView_ABC::keyPressEvent( event );
}
