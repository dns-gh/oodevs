// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineListView_h_
#define __TimelineListView_h_

#include "tools/ElementObserver_ABC.h"
#include <qlistview.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace actions
{
    class Action_ABC;
    class ActionsFilter_ABC;
}

// =============================================================================
/** @class  TimelineListView
    @brief  TimelineListView
*/
// Created: SBO 2008-04-22
// =============================================================================
class TimelineListView : public QListView
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< actions::Action_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelineListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~TimelineListView();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnSelectionChange( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineListView( const TimelineListView& );            //!< Copy constructor
    TimelineListView& operator=( const TimelineListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update();
    bool ShouldDisplay( const kernel::Entity_ABC& entity ) const;
    virtual void NotifyCreated( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual void setContentsPos( int x, int y );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const actions::Action_ABC* >        T_Actions;
    typedef std::map< const kernel::Entity_ABC*, T_Actions > T_EntityActions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_EntityActions actions_;
    const actions::ActionsFilter_ABC* filter_;
    //@}
};

#endif // __TimelineListView_h_
