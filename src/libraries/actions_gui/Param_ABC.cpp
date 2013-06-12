// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "Param_ABC.h"
#include "ParamInterface_ABC.h"
#include "moc_Param_ABC.cpp"
#include "ListParameter.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/ObjectNameManager.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: Param_ABC constructor
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
Param_ABC::Param_ABC( QObject* parent, const ParamInterface_ABC& paramInterface, const kernel::OrderParameter& parameter )
    : QObject( parent )
    , paramInterface_ ( paramInterface )
    , parameter_      ( parameter )
    , parentList_     ( 0 )
    , parentParameter_( 0 )
    , name_           ( parameter.GetName().c_str() )
    , type_           ( parameter.GetType() )
    , controller_     ( 0 )
    , group_          ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Param_ABC::~Param_ABC()
{
    RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RemoveFromController
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::RemoveFromController()
{
    if( controller_ )
    {
        controller_->Unregister( *this );
        controller_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RegisterIn
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Param_ABC::RegisterIn( kernel::ActionController& controller )
{
    controller.Register( *this );
    controller_ = &controller;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::Draw( const geometry::Point2f&, const ::gui::Viewport_ABC&, gui::GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CheckValidity
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
bool Param_ABC::CheckValidity()
{
    if( IsChecked() && !InternalCheckValidity() )
    {
        if( group_ )
            group_->Warn();
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::InternalCheckValidity
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool Param_ABC::InternalCheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetName
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QString Param_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsOptional
// Created: SBO 2008-03-06
// -----------------------------------------------------------------------------
bool Param_ABC::IsOptional() const
{
    return parameter_.IsOptional();
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RegisterListener
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::RegisterListener( Param_ABC& param )
{
    parentParameter_ = &param;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::NotifyChange
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::NotifyChange()
{
    if( parentParameter_ )
        parentParameter_->NotifyChanged( *this );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::NotifyChanged
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::NotifyChanged( Param_ABC& /*param*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::BuildInterface
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
QWidget* Param_ABC::BuildInterface( const QString& objectName, QWidget* parent )
{
    if( group_ )
        ::gui::ObjectNameManager::getInstance()->RemoveRegisteredName( group_->objectName() );
    group_ = new ::gui::RichGroupBox( objectName, GetName(), parent );
    group_->setCheckable( IsOptional() );
    if( group_->isCheckable() )
        group_->setChecked( false );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsSet
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
bool Param_ABC::IsChecked() const
{
    if( IsOptional() && group_ )
        return group_->isChecked();
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetOptional
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void Param_ABC::SetOptional( bool optional )
{
    parameter_.SetOptional( optional );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetMenuName
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
QString Param_ABC::GetMenuName() const
{
    QString name;
    if( IsInList() && parentList_->IsPotential( const_cast< Param_ABC* >( this ) ) )
    {
        name = tools::translate( "Param_ABC", "Add item" );
    }
    else
        name = IsInParam() ? GetType().c_str() : GetName();
    return name;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetMenuName
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
const std::string& Param_ABC::GetKeyName() const
{
    return parameter_.GetKeyName();
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::ConnectAction
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
void Param_ABC::ConnectAction()
{
    if( QAction* const* vAction = boost::get< QAction* >( &internalMenu_ ) )
        QObject::connect( *vAction, SIGNAL( triggered() ), this, SLOT( OnMenuClick() ) );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::ConnectWithParentList
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void Param_ABC::ConnectWithParentList()
{
    if( QAction* const* vAction = boost::get< QAction* >( &internalMenu_ ) )
        QObject::connect( *vAction, SIGNAL( triggered() ), parentList_, SLOT( OnMenuClick() ) );
    else if( kernel::ContextMenu* const* vMenu = boost::get< kernel::ContextMenu* >( &internalMenu_ ) )
        ( *vMenu )->ConnectSubActions( parentList_, SLOT( OnMenuClick() ), 1 );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CreateInternalMenu
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void Param_ABC::CreateInternalMenu( kernel::ContextMenu& menu )
{
    QAction* action = new QAction( GetMenuName(), menu );
    internalMenu_ = action;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CreateMenu
// Created: ABR 2011-12-23
// -----------------------------------------------------------------------------
kernel::ContextMenu::T_MenuVariant Param_ABC::CreateMenu( kernel::ContextMenu& mainMenu )
{
    if( IsInParam() )
    {
        kernel::ContextMenu::T_MenuVariant variantMenu = parentParameter_->CreateMenu( mainMenu );
        if( kernel::ContextMenu* const* menu = boost::get< kernel::ContextMenu* >( &variantMenu ) )
        {
            CreateInternalMenu( **menu );
            if( IsInList() && parentList_->IsPotential( parentParameter_ ) )
                ConnectWithParentList();
            ConnectAction();
            internalMenu_ = ( *menu )->InsertVariant( "", internalMenu_, false, parentParameter_->GetIndex( this ) );
            return internalMenu_;
        }
        return static_cast< QAction* >( 0 );
    }
    else if( IsInList() )
    {
        kernel::ContextMenu::T_MenuVariant variantMenu = parentList_->CreateMenu( mainMenu );
        if( kernel::ContextMenu* const* menu = boost::get< kernel::ContextMenu* >( &variantMenu ) )
        {
            if( parentList_->IsSelected( this ) )
            {
                CreateInternalMenu( **menu );
                ConnectAction();
                internalMenu_ = ( *menu )->InsertVariant( "", internalMenu_, false, 1 );
                return internalMenu_;
            }
            else if( parentList_->IsPotential( this ) )
            {
                CreateInternalMenu( **menu );
                ConnectWithParentList();
                ConnectAction();
                internalMenu_ = ( *menu )->InsertVariant( "", internalMenu_, false, 0 );
                return internalMenu_;
            }
            return static_cast< QAction* >( 0 );
        }
        assert( false ); // Parent list must create a ContextMenu
    }

    CreateInternalMenu( mainMenu );
    ConnectAction();
    internalMenu_ = mainMenu.InsertVariant( "Parameter", internalMenu_, false, paramInterface_.GetIndex( this ) );
    return internalMenu_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::OnMenuClick
// Created: ABR 2011-12-23
// -----------------------------------------------------------------------------
void Param_ABC::OnMenuClick()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetIndex
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
int Param_ABC::GetIndex( Param_ABC* ) const
{
    return -1;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetParentList
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
void Param_ABC::SetParentList( ListParameterBase* parentList )
{
    parentList_ = parentList;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetName
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
void Param_ABC::SetName( const QString& name )
{
    name_ = name;
    parameter_.SetName( name.toStdString() );
    if( group_ )
        group_->setTitle( name_ );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetName
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void Param_ABC::SetKeyName( const std::string& keyName )
{
    parameter_.SetKeyName( keyName );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetType
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
void Param_ABC::SetType( const std::string& type )
{
    type_ = type;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetType
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
const std::string& Param_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsInList
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
bool Param_ABC::IsInList() const
{
    return parentList_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsInParam
// Created: ABR 2012-01-11
// -----------------------------------------------------------------------------
bool Param_ABC::IsInParam() const
{
    return parentParameter_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CreateListMenu
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
void Param_ABC::CreateListMenu( QTreeView* list, const QStandardItemModel& model, const QPoint& pos, bool createEnabled )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( list );
    if( createEnabled )
        menu->insertItem( tools::translate( "ListParameter", "Add" ), this, SLOT( OnCreate() ) );

    const QModelIndex index = list->selectionModel()->currentIndex();
    if( index.isValid() )
        if( QStandardItem* item = model.itemFromIndex( index ) )
            menu->insertItem( tools::translate( "ListParameter", "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );

    menu->insertItem( tools::translate( "ListParameter", "Clear list" ), this, SLOT( OnClear() ) );
    menu->popup( list->viewport()->mapToGlobal( pos ) );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetNextName
// Created: NPT 2012-10-25
// -----------------------------------------------------------------------------
QString Param_ABC::GetNextNameAndId( const QStandardItemModel& model )
{
    int previousId = model.rowCount() != 0? model.item( model.rowCount() -1 )->data( IdRole ).toInt() : 0 ;
    return tools::translate( "ListParameter", "%1 (item %2)" ).arg( parameter_.GetName().c_str() ).arg( previousId + 1 );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void Param_ABC::SetEntity( const kernel::Entity_ABC* )
{
    // NOTHING
}
