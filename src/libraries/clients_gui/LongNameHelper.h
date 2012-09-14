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

namespace longname
{

std::string GetEntityLongName( const kernel::Entity_ABC& entity );

bool SetItemLongName( const kernel::Entity_ABC& entity, gui::ValuedListItem& item );

bool SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item );

}  // namespace longname

}

#endif // __LongNameHelper_h_
