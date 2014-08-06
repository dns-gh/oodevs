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
template< typename T > class SafePointer;
class Entity_ABC;
}

namespace gui
{
    class ModelObserver_ABC;

namespace longname
{

std::string GetEntityLongName( const kernel::Entity_ABC& entity );

bool SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item );
void ShowRenameDialog( QWidget* parent, const kernel::SafePointer< kernel::Entity_ABC >& entity, ModelObserver_ABC& modelObserver );

}  // namespace longname

}

#endif // __LongNameHelper_h_
