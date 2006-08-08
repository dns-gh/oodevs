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

#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/SelectionObserver_ABC.h"
#include "astec_kernel/OptionsObserver_ABC.h"

class Controllers;
class Population_ABC;
class Team;
class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public QListView
                         , public Observer_ABC
                         , public ElementObserver_ABC< Population_ABC >
                         , public SelectionObserver_Base< Population_ABC > // $$$$ AGE 2006-08-07: SelectionObserver_Base ????
                         , public OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~PopulationListView();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnSelectionChange( QListViewItem* item );
    void OnRequestCenter();
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
    virtual void NotifyCreated( const Population_ABC& popu );
    virtual void NotifyDeleted( const Population_ABC& popu );
    virtual void Select( const Population_ABC& popu );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    ItemFactory_ABC& factory_;
    QPopupMenu* pPopupMenu_;
    const Team* currentTeam_;
    //@}
};

#endif // __PopulationListView_h_
