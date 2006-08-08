// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPanel_h_
#define __PopulationPanel_h_

#include "InfoPanel_ABC.h"
#include "astec_kernel/SelectionObserver_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/SafePointer.h"

class QListViewItem;
class DisplayBuilder;
class Population_ABC;
class Controllers;
class ValuedListItem;
class PopulationPart_ABC;
template< typename T > class ListDisplayer;
class Displayer_ABC;
class ItemFactory_ABC;

// =============================================================================
// Created: HME 2005-10-03
// =============================================================================
class PopulationPanel : public InfoPanel_ABC
                      , public Observer_ABC
                      , public SelectionObserver< Population_ABC >
                      , public ElementObserver_ABC< Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~PopulationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const Population_ABC* popu );
    void Display( const PopulationPart_ABC& part, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Population_ABC& );
    virtual void NotifyDeleted( const Population_ABC& );
    void DisplayParts( const Population_ABC& population );
    //@}

private:
    Controllers& controllers_;
    SafePointer< Population_ABC > selected_;
    DisplayBuilder* display_;
    ListDisplayer< PopulationPanel >* pPartsListView_;
};

#endif // __PopulationPanel_h_
