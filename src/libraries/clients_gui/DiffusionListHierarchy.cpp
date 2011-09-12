// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::DiffusionListHierarchy */

#include "clients_gui_pch.h"
#include "DiffusionListHierarchy.h"
#include "moc_DiffusionListHierarchy.cpp"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/GeneralConfig.h"
#include "ValuedListItem.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <iterator>

using namespace gui;
using namespace kernel;

const int     DiffusionListHierarchy::iconSize_           = 32;
const QColor  DiffusionListHierarchy::baseColor_          = Qt::white;
const QColor  DiffusionListHierarchy::selectedColor_      = QColor( 222, 250, 218 );
const QColor  DiffusionListHierarchy::recipientsColor_    = QColor( 216, 241, 250 );
const QColor  DiffusionListHierarchy::potentialsColor_    = QColor( 255, 255, 200 );
const QString DiffusionListHierarchy::diffusionSeparator_ = ";";
const QRegExp DiffusionListHierarchy::diffusionRegexp_ ( "([0-9]+[;]{1})*[0-9]*" );

namespace
{
    QPixmap MakePixmap( const std::string& name )
    {
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gui/" ) + name + ".png" ).c_str() ).scaled( DiffusionListHierarchy::iconSize_, DiffusionListHierarchy::iconSize_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy constructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListHierarchy::DiffusionListHierarchy( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile, const EntitySymbols& icons )
    : HierarchyListView< TacticalHierarchies >( pParent, controllers, factory, profile, icons )
    , selected_       ( controllers )
    , filter_         ( EFilterRecipientsAndPotentials )
    , basePixmap_     ( QPixmap() )
    , selectedPixmap_ ( MakePixmap( "diffusion_emitter" ) )
    , recipientsPixmap_( MakePixmap( "diffusion_receiver" ) )
{
    addColumn( "Icon", iconSize_ );
    setColumnAlignment( 1, Qt::AlignCenter );
 //   setColumnWidth( 1, 25 );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy destructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListHierarchy::~DiffusionListHierarchy()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnSelectionChange
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnSelectionChange( Q3ListViewItem* /*item*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::NotifySelected
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::NotifySelected( const Entity_ABC* element )
{
    ValuedListItem* item = element ? FindItem( element, firstChild() ) : 0;
    if( item )
    {
        setSelected( item, true );
        ensureItemVisible( item );
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnContextMenuRequested
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int )
{
    if( !item )
        return;
    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( item );
    popupMenu_.clear();
    if( valuedItem->GetBackgroundColor() == potentialsColor_ )
    {
        popupMenu_.insertItem( tr( "Add to diffusion list" ), this, SLOT( OnAddItem() ) );
        popupMenu_.popup( pos );
    }
    else if( valuedItem->GetBackgroundColor() == recipientsColor_ )
    {
        popupMenu_.insertItem( tr( "Remove from diffusion list" ), this, SLOT( OnRemoveItem() ) );
        popupMenu_.popup( pos );
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnAddItem
// Created: ABR 2011-05-05
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnAddItem()
{
    ValuedListItem* item = static_cast< ValuedListItem* >( selectedItem() );
    item->SetBackgroundColor( recipientsColor_ );
    kernel::Entity_ABC const* entity = item->GetValue< kernel::Entity_ABC >();
    recipients_.insert( entity->GetId() );
    ComputeAndEmitDiffusionList();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnRemoveItem
// Created: ABR 2011-05-05
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnRemoveItem()
{
    ValuedListItem* item = static_cast< ValuedListItem* >( selectedItem() );
    item->SetBackgroundColor( potentialsColor_ );
    kernel::Entity_ABC const* entity = item->GetValue< kernel::Entity_ABC >();
    recipients_.erase( entity->GetId() );
    ComputeAndEmitDiffusionList();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::ComputeAndEmitDiffusionList
// Created: ABR 2011-05-05
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::ComputeAndEmitDiffusionList()
{
    std::string diffusionList;
    for( std::set< unsigned long >::const_iterator it = recipients_.begin(); it != recipients_.end(); ++it )
    {
        diffusionList += ( diffusionList.empty() ) ? "" : diffusionSeparator_;
        diffusionList += boost::lexical_cast< std::string >( *it );
    }
    UpdateDisplay( diffusionList.c_str() );
    emit ListChanged( diffusionList.c_str() );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnFilterChanged
// Created: ABR 2011-05-05
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnFilterChanged( int filter )
{
    filters_.clear();
    assert( filter >= EFilterFull && filter <= EFilterRecipientsAndPotentials );
    filter_ = static_cast< EFilter >( filter );
    switch( filter_ )
    {
    case EFilterFull:
        break;
    case EFilterRecipients:
        filters_.push_back( baseColor_ );
        filters_.push_back( potentialsColor_ );
        break;
    case EFilterRecipientsAndPotentials:
        filters_.push_back( baseColor_ );
        break;
    default:
        assert( false );
    }
    ApplyFilter( boost::bind( &DiffusionListHierarchy::ApplyAllFilters, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::OnFilterChanged
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::OnFilterChanged( const QString& filterLine )
{
    filterLine_ = filterLine.lower();
    ApplyFilter( boost::bind( &DiffusionListHierarchy::ApplyAllFilters, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::ApplyAllFilters
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
bool DiffusionListHierarchy::ApplyAllFilters( ValuedListItem* item )
{
    return ApplyFilterButtons( item ) && ApplyFilterLine( item );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::ApplyFilterButtons
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
bool DiffusionListHierarchy::ApplyFilterButtons( ValuedListItem* item )
{
    return ( item ) ? std::find( filters_.begin(), filters_.end(), item->GetBackgroundColor() ) == filters_.end() : false;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::ApplyFilterLine
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
bool DiffusionListHierarchy::ApplyFilterLine( ValuedListItem* item )
{
    if( filterLine_.isEmpty() )
        return true;
    if( !item )
        return false;
    kernel::Entity_ABC const* entity = item->GetValue< kernel::Entity_ABC >();
    if( !entity )
        return false;
    QString name = ( entity->GetName() + QString::number( entity->GetId() ) ).lower();
    return name.find( filterLine_ ) != -1;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::Purge
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::Purge()
{
    potentials_.clear();
    recipients_.clear();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::Initialize
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::Initialize( SafePointer< Entity_ABC > selected, const QString& diffusionList )
{
    if( !selected )
        return;
    selected_ = selected;
    // Update all potentials
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        kernel::Entity_ABC const* entity = item->GetValue< kernel::Entity_ABC >();
        DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( entity->Retrieve< DictionaryExtensions >() );
        if( !dico )
            continue;
        ExtensionType* type = dico->GetExtensionTypes().tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
        if( !type || dico->GetExtensions().size() == 0 )
            continue;
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( dico->GetExtensionTypes().GetNameByType( AttributeType::ETypeDiffusionList ) );
        if( !attribute )
            continue;
        if( !dico->GetValue( "TypePC" ).empty() && dico->GetValue( "TypePC" ) != "PasCorresp" &&
            &selected_->Get< kernel::TacticalHierarchies >().GetTop() == &entity->Get< kernel::TacticalHierarchies >().GetTop() )
            potentials_.insert( entity->GetId() );
    }
    UpdateDisplay( diffusionList );
    OnFilterChanged( filter_ );
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
        it.current()->setOpen( false );
    ValuedListItem* item = FindItem( selected_, firstChild() );
    if( item )
    {
        setSelected( item, false );
        ensureItemVisible( item );
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListHierarchy::UpdateDisplay
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListHierarchy::UpdateDisplay( const QString& diffusionList )
{
    // Update recipients
    QStringList list = QStringList::split( diffusionSeparator_, diffusionList );
    recipients_.clear();
    for( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
        recipients_.insert( boost::lexical_cast< unsigned long >( *it ) );
    // Update current potentials
    std::set< unsigned long > newPotentials;
    std::set_difference( potentials_.begin(), potentials_.end(), recipients_.begin(), recipients_.end(), std::inserter( newPotentials, newPotentials.begin() ) );
    // Update display
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        kernel::Entity_ABC const* entity = item->GetValue< kernel::Entity_ABC >();
        if( entity->GetId() == selected_->GetId() )
        {
            item->SetBackgroundColor( selectedColor_ );
            item->setPixmap( 1, selectedPixmap_ );
        }
        else if( std::find( recipients_.begin(), recipients_.end(), entity->GetId() ) != recipients_.end() )
        {
            item->SetBackgroundColor( recipientsColor_ );
            item->setPixmap( 1, recipientsPixmap_ );
        }
        else if( std::find( newPotentials.begin(), newPotentials.end(), entity->GetId() ) != newPotentials.end() )
        {
            item->SetBackgroundColor( potentialsColor_ );
            item->setPixmap( 1, basePixmap_ );
        }
        else
        {
            item->SetBackgroundColor( baseColor_ );
            item->setPixmap( 1, basePixmap_ );
        }
        item->repaint();
    }
}

