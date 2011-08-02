// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamLimaList.h"
#include "moc_ParamLimaList.cpp"
#include "LimaParameter.h"
#include "actions/LimaList.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: SBO 2006-11-14
// ----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller, const QDateTime& currentDate )
    : ListParameter( parent, parameter, actions )
    , controller_ ( controller )
    , parameter_  ( parameter )
    , converter_  ( converter )
    , currentDate_( currentDate )
    , count_      ( 0 )
    , potential_  ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::BuildInterface
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
QWidget* ParamLimaList::BuildInterface( QWidget* parent )
{
    EnableCreation( false );
    return ListParameter::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CommitTo
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ParamLimaList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::LimaList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CreateElement
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Param_ABC* ParamLimaList::CreateElement()
{
    if( potential_ )
    {
        LimaParameter* lima = new LimaParameter( this, tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ), converter_, currentDate_, *potential_ );
        limas_[potential_] = lima;
        return lima;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::DeleteElement
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ParamLimaList::DeleteElement( Param_ABC& param )
{
    for( T_Limas::iterator it = limas_.begin(); it != limas_.end(); ++it )
        if( it->second == &param )
        {
            limas_.erase( it );
            break;
        }
    ListParameter::DeleteElement( param );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::NotifyContextMenu
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ParamLimaList::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.IsLimit() )
        return;
    CIT_Limas it = limas_.find( &entity );
    if( it != limas_.end() )
        Select( *it->second );
    else
    {
        potential_ = &entity;
        Q3PopupMenu* limaMenu = new Q3PopupMenu( menu );
        for( unsigned int i = 0; i < kernel::eLimaFuncNbr; ++i )
        {
            int id = limaMenu->insertItem( tools::ToString( (kernel::E_FuncLimaType)i ), this, SLOT( MenuItemValidated( int ) ) );
            limaMenu->setItemParameter( id, i );
        }
        menu.InsertItem( "Parameter", tools::translate( "ParamLimaList", "%1: add item as" ).arg( GetName() ), limaMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::MenuItemValidated
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ParamLimaList::MenuItemValidated( int index )
{
    OnCreate();
    CIT_Limas it = limas_.find( potential_ );
    if( it != limas_.end() )
        it->second->MenuItemValidated( index );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::NotifyDeleted
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ParamLimaList::NotifyDeleted( const kernel::TacticalLine_ABC& entity )
{
    if( entity.IsLimit() )
        return;
    if( potential_ == &entity )
        potential_ = 0;
    CIT_Limas it = limas_.find( &entity );
    if( it != limas_.end() )
    {
        Select( *it->second );
        OnDeleteSelectedItem();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::IsOptional
// Created: SBO 2008-03-06
// -----------------------------------------------------------------------------
bool ParamLimaList::IsOptional() const
{
    return parameter_.IsOptional();
}
