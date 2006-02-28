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
#include "Observer_ABC.h"
#include "SelectionObserver_ABC.h"
#include "ElementObserver_ABC.h"

class QListViewItem;
class DisplayBuilder;
class Population;
class Controller;
class ActionController;
class ValuedListItem;
class PopulationPart_ABC;
template< typename T >
class ListDisplayer;
class Displayer_ABC;

// =============================================================================
// Created: HME 2005-10-03
// =============================================================================
class PopulationPanel : public InfoPanel_ABC
                      , public Observer_ABC
                      , public SelectionObserver< Population >
                      , public ElementObserver_ABC< Population >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~PopulationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const Population* popu );
    void Display( const PopulationPart_ABC& part, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Population& );
    virtual void NotifyDeleted( const Population& );
    void DisplayParts( const Population& population );
    //@}

private:
    const Population* selected_;
    DisplayBuilder* display_;
    ListDisplayer< PopulationPanel >* pPartsListView_;
};

#endif // __PopulationPanel_h_
