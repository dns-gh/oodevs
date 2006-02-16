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

class Object_ABC;
class CampAttributes;
class CrossingSiteAttributes;
class LogisticRouteAttributes;
class NBCAttributes;
class RotaAttributes;

class Controller;
class ActionController;
class Display;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class ObjectPanel : public InfoPanel_ABC
                  , public Observer_ABC
                  , public ElementObserver_ABC< Object_ABC >
                  , public ElementObserver_ABC< CampAttributes >
                  , public ElementObserver_ABC< CrossingSiteAttributes >
                  , public ElementObserver_ABC< LogisticRouteAttributes >
                  , public ElementObserver_ABC< NBCAttributes >
                  , public ElementObserver_ABC< RotaAttributes >

{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( QWidget* pParent, Controller& controller, ActionController& actionController );
    virtual ~ObjectPanel();
    //@}

private:
    //! @name Operations
    //@{
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
    virtual void NotifySelected( const Object_ABC* object );
    //@}

private:
    //! @name Member data
    //@{
    Display* display_;
    QSpinBox*  pPercentBuiltEdit_;
    QSpinBox*  pPercentValueEdit_;
    QSpinBox*  pPercentAroundEdit_;
    QCheckBox* pIsUnderPrepCheckBox_;

//    QPushButton* pApplyButton_;
//    QPushButton* pCancelButton_;
    //@}
};

#endif // __ObjectPanel_h_
