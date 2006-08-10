// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectReportPanel_h_
#define __ObjectReportPanel_h_

#include "InfoPanel_ABC.h"
#include "astec_kernel/SelectionObserver_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/SafePointer.h"

class FireResultListView;
class Object_ABC;
class Controllers;
class Explosions;
class Displayer_ABC;
class ValuedListItem;
class PopulationFireResult;
class AgentFireResult;
class Equipment;
class SubItemDisplayer;
class Casualties;
class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public InfoPanel_ABC
                        , public Observer_ABC
                        , public ElementObserver_ABC< Explosions >
                        , public SelectionObserver< Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectReportPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectReportPanel();
    //@}

    //! @name Operations
    //@{
    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectReportPanel( const ObjectReportPanel& );
    ObjectReportPanel& operator=( const ObjectReportPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Explosions& );
    virtual void NotifyDeleted( const Object_ABC& );
    virtual void NotifySelected( const Object_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    SafePointer< Object_ABC > selected_;
    FireResultListView* reports_;
    //@}
};

#endif // __ObjectReportPanel_h_
