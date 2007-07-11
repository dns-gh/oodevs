// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLimaList.h"
#include "moc_ParamLimaList.cpp"
#include "LimaParameter.h"
#include "gaming/Lima.h"
#include "gaming/Tools.h"
#include "gaming/ActionParameterLimaList.h"
#include "gaming/Action_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "game_asn/Asn.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: SBO 2006-11-14
// ----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( QObject* parent, const OrderParameter& parameter, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , count_( 0 )
    , potential_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::BuildInterface
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ParamLimaList::BuildInterface( QWidget* parent )
{
    EnableCreation( false );
    ListParameter::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CommitTo
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ParamLimaList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterLimaList( parameter_ ) );
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
        LimaParameter* lima = new LimaParameter( this, tr( "Lima %1" ).arg( ++count_ ), converter_, *potential_ );
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
        potential_ = static_cast< const Lima* >( &entity );
        QPopupMenu* limaMenu = new QPopupMenu( menu );
        for( unsigned int i = 0; i < eLimaFuncNbr; ++i )
        {
            int id = limaMenu->insertItem( tools::ToString( (E_FuncLimaType)i ), this, SLOT( MenuItemValidated( int ) ) );
            limaMenu->setItemParameter( id, i );
        }
        menu.InsertItem( "Parameter", tr( "Add line as" ), limaMenu );
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
    CIT_Limas it = limas_.find( &entity );
    if( it != limas_.end() )
    {
        Select( *it->second );
        OnDeleteSelectedItem();
    }
}
