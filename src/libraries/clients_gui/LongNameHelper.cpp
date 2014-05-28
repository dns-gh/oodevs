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
#pragma warning( push, 0 )
#include <QtGui/qstandarditemmodel.h>
#pragma warning( pop )

namespace gui
{

namespace longname
{

std::string GetEntityLongName( const kernel::Entity_ABC& entity )
{
    const kernel::DictionaryExtensions* dico = entity.Retrieve< kernel::DictionaryExtensions >();
    if( dico && dico->IsEnabled() )
        return dico->GetValue( "NomLong" );
    return std::string();
}

bool SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item )
{
    std::string longName = longname::GetEntityLongName( entity );
    if( longName.empty() )
    {
        item.setText( entity.GetName() );
        return false;
    }
    item.setText( longName.c_str() );
    return true;
}

}  // namespace longname
}  // namespace gui
