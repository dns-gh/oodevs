// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LongNameHelper.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_gui/ValuedListItem.h"
#pragma warning( push, 0 )
#include <QtGui/qstandarditemmodel.h>
#pragma warning( pop )

namespace gui
{

std::string LongNameHelper::GetEntityLongName( const kernel::Entity_ABC& entity )
{
    const kernel::DictionaryExtensions* dico = entity.Retrieve< kernel::DictionaryExtensions >();
    if( dico && dico->IsEnabled() )
        return dico->GetValue( "NomLong" );
    return std::string();
}

bool LongNameHelper::SetItemLongName( const kernel::Entity_ABC& entity, gui::ValuedListItem& item )
{
    std::string longName = LongNameHelper::GetEntityLongName( entity );
    if( longName.empty() )
    {
        item.setText( 0, entity.GetName() );
        return false;
    }
    item.setText( 0, longName.c_str() );
    return true;
}

bool LongNameHelper::SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item )
{
    std::string longName = LongNameHelper::GetEntityLongName( entity );
    if( longName.empty() )
    {
        item.setText( entity.GetName() );
        return false;
    }
    item.setText( longName.c_str() );
    return true;
}

}  // namespace gui
