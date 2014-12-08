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
    class EntityTreeView_ABC;

namespace longname
{
    
std::string GetEntityLongName( const kernel::Entity_ABC& entity );
QString GetEntityDisplayName( const kernel::Entity_ABC& entity );

bool SetItemLongName( const kernel::Entity_ABC& entity, QStandardItem& item );
void ShowRenameDialog( QWidget* parent, const kernel::SafePointer< kernel::Entity_ABC >& entity );
void Rename( const kernel::SafePointer< kernel::Entity_ABC >& entity, const std::vector< gui::EntityTreeView_ABC* >& views );

}  // namespace longname

}

#endif // __LongNameHelper_h_
