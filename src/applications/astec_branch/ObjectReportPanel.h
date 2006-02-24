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
#include "Observer_ABC.h"

class Object_ABC;
class FireResultListView;
class Controller;
class ActionController;

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public InfoPanel_ABC
                        , public Observer_ABC
                        , public ElementObserver_ABC< Object_ABC > // $$$$ AGE 2006-02-23: 
                        , public SelectionObserver< Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectReportPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~ObjectReportPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectReportPanel( const ObjectReportPanel& );
    ObjectReportPanel& operator=( const ObjectReportPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Object_ABC& );
    virtual void NotifyDeleted( const Object_ABC& );
    virtual void NotifySelected( const Object_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    const Object_ABC* selected_;
    //@}
};

#endif // __ObjectReportPanel_h_
