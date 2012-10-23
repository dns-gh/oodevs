// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LongNameHelper_h_
#define __LongNameHelper_h_

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_gui/ValuedListItem.h"

namespace gui
{

// =============================================================================
/** @class  LongNameHelper
    @brief  LongNameHelper
*/
// Created: ABR 2011-09-19
// =============================================================================
class LongNameHelper
{
public:
    static std::string GetEntityLongName( const kernel::Entity_ABC& entity )
    {
        const kernel::DictionaryExtensions* dico = entity.Retrieve< kernel::DictionaryExtensions >();
        if( dico && dico->IsEnabled() )
            return dico->GetValue( "NomLong" );
        return std::string();
    }

    static bool SetItemLongName( const kernel::Entity_ABC& entity, gui::ValuedListItem& item )
    {
        std::string longName = GetEntityLongName( entity );
        if( longName.empty() )
        {
            item.setText( 0, entity.GetName() );
            return false;
        }
        item.setText( 0, longName.c_str() );
        return true;
    }
};

}

#endif // __LongNameHelper_h_
