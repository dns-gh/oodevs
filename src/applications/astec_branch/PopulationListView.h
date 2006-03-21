// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationListView_h_
#define __PopulationListView_h_

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"

class Controller;
class Population;
class ActionController;

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public QListView
                         , public Observer_ABC
                         , public ElementObserver_ABC< Population >
                         , public SelectionObserver_Base< Population >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, Controller& controller, ActionController& actionController );
    virtual ~PopulationListView();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    PopulationListView( const PopulationListView& );
    PopulationListView& operator=( const PopulationListView& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Population& popu );
    virtual void NotifyUpdated( const Population& popu );
    virtual void NotifyDeleted( const Population& popu );
    virtual void Select( const Population& popu );
    //@}

private:
    //! @name Member data
    //@{
    ActionController& actionController_;
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __PopulationListView_h_
