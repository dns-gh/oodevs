// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DragAndDropObserver_ABC_h_
#define __DragAndDropObserver_ABC_h_

namespace gui
{

// =============================================================================
/** @class  DragAndDropObserver_ABC
    @brief  DragAndDropObserver_ABC
*/
// Created: JSR 2012-09-07
// =============================================================================
class DragAndDropObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DragAndDropObserver_ABC() {}
    virtual ~DragAndDropObserver_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual QStringList MimeTypes() const { return QStringList(); }
    virtual void Drop( const QString& /*mimeType*/, void* /*data*/, QStandardItem& /*target*/ ) {}
    virtual QMimeData* MimeData( const QModelIndexList& /*indexes*/, bool& /*overriden*/ ) const { return 0; }
    //@}
};

} //! namespace gui

#endif // __DragAndDropObserver_ABC_h_
