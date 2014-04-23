// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __RichTableView_h_
#define __RichTableView_h_

#include "CommonDelegate.h"
#include "RichWidget.h"

namespace gui
{
// =============================================================================
/** @class  RichTableView
    @brief  RichTableView
*/
// Created: LGY 2014-01-09
// =============================================================================
class RichTableView : public RichWidget< QTableView >
{
public:
    //! @name Constructors/Destructor
    //@{
             RichTableView( const QString& objectName, QWidget* parent );
    virtual ~RichTableView();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    //@}

protected:
    //! @name Member data
    //@{
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    gui::CommonDelegate delegate_;
    //@}
};

} //! namespace gui

#endif // __RichTableView_h_
