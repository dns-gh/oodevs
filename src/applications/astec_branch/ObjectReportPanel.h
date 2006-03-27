// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectReportPanel.h $
// $Author: Sbo $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: ObjectReportPanel.h $
//
// *****************************************************************************

#ifndef __ObjectReportPanel_h_
#define __ObjectReportPanel_h_

#include "InfoPanel_ABC.h"
#include "SelectionObserver_ABC.h"
#include "ElementObserver_ABC.h"

class FireResultListView;
class Object;
class Controllers;
class Explosions;
class Displayer_ABC;
class ValuedListItem;
class PopulationFireResult;
class AgentFireResult;
class Equipment;
class SubItemDisplayer;
class Casualties;

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public InfoPanel_ABC
                        , public Observer_ABC
                        , public ElementObserver_ABC< Explosions >
                        , public SelectionObserver< Object >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectReportPanel( InfoPanels* pParent, Controllers& controllers );
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
    virtual void NotifyDeleted( const Object& );
    virtual void NotifySelected( const Object* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Object* selected_;
    FireResultListView* reports_;
    //@}
};

#endif // __ObjectReportPanel_h_
