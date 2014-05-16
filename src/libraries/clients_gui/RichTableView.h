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
#include "Filterable_ABC.h"
#include "RichWidget.h"
#include "StandardModel.h"

namespace gui
{
    class WidgetMenu;

// =============================================================================
/** @class  RichTableView
    @brief  RichTableView
*/
// Created: LGY 2014-01-09
// =============================================================================
class RichTableView : public RichWidget< QTableView >
                    , public Filterable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RichTableView( const QString& objectName, QWidget* parent );
    virtual ~RichTableView();
    //@}

    //! @name Filterable_ABC implementation
    //@{
    virtual void ApplyFilters( const std::map< int, std::vector< std::shared_ptr< Filter_ABC > > >& filters );
    virtual void CreateFilters( RichView_ABC& richView );
    virtual void Purge();
    virtual QHeaderView* GetHeader() const;
    //@}

protected:
    //! @name Member data
    //@{
    QSortFilterProxyModel proxyModel_;
    gui::StandardModel dataModel_;
    gui::CommonDelegate delegate_;
    //@}
};

} //! namespace gui

#endif // __RichTableView_h_
