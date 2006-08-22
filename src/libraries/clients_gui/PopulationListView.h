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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Population_ABC;
    class Team_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public QListView
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< kernel::Population_ABC >
                         , public kernel::SelectionObserver_Base< kernel::Population_ABC > // $$$$ AGE 2006-08-07: SelectionObserver_Base ????
                         , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
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
    virtual void NotifyCreated( const kernel::Population_ABC& popu );
    virtual void NotifyDeleted( const kernel::Population_ABC& popu );
    virtual void Select( const kernel::Population_ABC& popu );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    QPopupMenu* pPopupMenu_;
    const kernel::Team_ABC* currentTeam_;
    //@}
};

}

#endif // __PopulationListView_h_
