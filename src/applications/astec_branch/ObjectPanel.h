// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectPanel.h $
// $Author: Age $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: ObjectPanel.h $
//
// *****************************************************************************

#ifndef __ObjectPanel_h_
#define __ObjectPanel_h_

#include "InfoPanel_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"

class Object;
class CampAttributes;
class CrossingSiteAttributes;
class LogisticRouteAttributes;
class NBCAttributes;
class RotaAttributes;

class Controller;
class ActionController;
class DisplayBuilder;
class InfoPanels;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class ObjectPanel : public InfoPanel_ABC
                  , public Observer_ABC
                  , public ElementObserver_ABC< Object >
                  , public ElementObserver_ABC< CampAttributes >
                  , public ElementObserver_ABC< CrossingSiteAttributes >
                  , public ElementObserver_ABC< LogisticRouteAttributes >
                  , public ElementObserver_ABC< NBCAttributes >
                  , public ElementObserver_ABC< RotaAttributes >
                  , public SelectionObserver< Object >

{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( InfoPanels* pParent, Controller& controller, ActionController& actionController );
    virtual ~ObjectPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const Object* object );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ObjectPanel( const ObjectPanel& );
    ObjectPanel& operator=( const ObjectPanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );

    virtual void NotifyUpdated( const Object& );
    virtual void NotifyDeleted( const Object& );
    virtual void NotifyUpdated( const CampAttributes& attributes );
    virtual void NotifyUpdated( const CrossingSiteAttributes& attributes );
    virtual void NotifyUpdated( const LogisticRouteAttributes& attributes );
    virtual void NotifyUpdated( const NBCAttributes& attributes );
    virtual void NotifyUpdated( const RotaAttributes& attributes );

    template< typename Extension >
    bool ShouldUpdate( const Extension& extension );
    //@}

private:
    //! @name Member data
    //@{
    DisplayBuilder* display_;
    const Object* selected_;

//    QPushButton* pApplyButton_;
//    QPushButton* pCancelButton_;
    //@}
};

#endif // __ObjectPanel_h_
