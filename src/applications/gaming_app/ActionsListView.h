// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsListView_h_
#define __ActionsListView_h_

#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class DisplayExtractor_ABC;
}

namespace actions
{
    class Action_ABC;
    class Parameter_ABC;
}

// =============================================================================
/** @class  ActionsListView
    @brief  ActionsListView
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsListView : public QTreeWidget
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< actions::Action_ABC >
                      , public tools::SelectionObserver< actions::Action_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ActionsListView( QWidget* parent, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor );
    virtual ~ActionsListView();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const actions::Action_ABC& action );
    virtual void NotifySelected( const actions::Action_ABC* action );
    void DisplayParameter( const actions::Parameter_ABC& param, QTreeWidgetItem* item );
    template< typename T >
    void RecursiveDisplay( const T& element, QTreeWidgetItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::DisplayExtractor_ABC& extractor_;
    QIcon parameter_;
    //@}
};

#endif // __ActionsListView_h_
