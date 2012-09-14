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

#include <string>

class QStandardItem;

namespace kernel
{
class Entity_ABC;
}

namespace gui
{

class ValuedListItem;

// =============================================================================
/** @class  LongNameHelper
    @brief  LongNameHelper
*/
// Created: ABR 2011-09-19
// =============================================================================
class LongNameHelper
{
public:
    static std::string GetEntityLongName( const kernel::Entity_ABC& entity );
   
    static bool SetItemLongName( const kernel::Entity_ABC& entity, gui::ValuedListItem& item );

    static bool SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item );
};

}

#endif // __LongNameHelper_h_
