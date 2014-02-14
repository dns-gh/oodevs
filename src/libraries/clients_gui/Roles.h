// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __Roles_h_
#define __Roles_h_

namespace gui
{
    namespace Roles
    {
        enum E_Roles
        {
            DataRole     = Qt::UserRole,
            FilterRole   = Qt::UserRole + 1,
            MimeTypeRole = Qt::UserRole + 2,
            SafeRole     = Qt::UserRole + 3,
            OtherRole    = Qt::UserRole + 4,
            ExcelColorRole = Qt::UserRole + 5,
            SortRole       = Qt::UserRole + 6
        };
    }//! namespace Roles
}//! namespace gui

#endif // __Roles_h_
