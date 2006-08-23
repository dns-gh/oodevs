// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPanel_h_
#define __PopulationPanel_h_

#include "InfoPanel_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

class QListViewItem;

namespace kernel
{
    class Displayer_ABC;
    class Population_ABC;
    class PopulationPart_ABC;
    class Controllers;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class ItemFactory_ABC;
    class DisplayBuilder;
    class ValuedListItem;

// =============================================================================
// Created: HME 2005-10-03
// =============================================================================
class PopulationPanel : public InfoPanel_ABC
                      , public kernel::Observer_ABC
                      , public kernel::SelectionObserver< kernel::Population_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~PopulationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Population_ABC* popu );
    void Display( const PopulationPart_ABC& part, kernel::Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const kernel::Population_ABC& );
    virtual void NotifyDeleted( const kernel::Population_ABC& );
    void DisplayParts( const kernel::Population_ABC& population );
    //@}

private:
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Population_ABC > selected_;
    DisplayBuilder* display_;
    ListDisplayer< PopulationPanel >* pPartsListView_;
};

}

#endif // __PopulationPanel_h_
