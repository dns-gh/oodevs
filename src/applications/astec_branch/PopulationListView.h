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

#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "OptionsObserver_ABC.h"

class Controllers;
class Population;
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
                         , public ElementObserver_ABC< Population >
                         , public SelectionObserver_Base< Population >
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
    virtual void NotifyCreated( const Population& popu );
    virtual void NotifyDeleted( const Population& popu );
    virtual void Select( const Population& popu );
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
