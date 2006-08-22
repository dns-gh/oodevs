// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPanel_h_
#define __ObjectPanel_h_

#include "InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
}

class CampAttributes;
class CrossingSiteAttributes;
class LogisticRouteAttributes;
class NBCAttributes;
class RotaAttributes;

namespace gui
{
    class DisplayBuilder;
    class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class ObjectPanel : public InfoPanel_ABC
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< kernel::Object_ABC >
                  , public kernel::ElementObserver_ABC< CampAttributes >
                  , public kernel::ElementObserver_ABC< CrossingSiteAttributes >
                  , public kernel::ElementObserver_ABC< LogisticRouteAttributes >
                  , public kernel::ElementObserver_ABC< NBCAttributes >
                  , public kernel::ElementObserver_ABC< RotaAttributes >
                  , public kernel::SelectionObserver< kernel::Object_ABC >

{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Object_ABC* object );
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

    virtual void NotifyUpdated( const kernel::Object_ABC& );
    virtual void NotifyDeleted( const kernel::Object_ABC& );
    virtual void NotifyUpdated( const CampAttributes& attributes );
    virtual void NotifyUpdated( const CrossingSiteAttributes& attributes );
    virtual void NotifyUpdated( const LogisticRouteAttributes& attributes );
    virtual void NotifyUpdated( const NBCAttributes& attributes );
    virtual void NotifyUpdated( const RotaAttributes& attributes );

    template< typename Extension >
    bool ShouldUpdate( const Extension& extension );
    template< typename T >
    void UpdateExtension( const kernel::Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    DisplayBuilder* display_;
    kernel::SafePointer< kernel::Object_ABC > selected_;

//    QPushButton* pApplyButton_;
//    QPushButton* pCancelButton_;
    //@}
};

}

#endif // __ObjectPanel_h_
