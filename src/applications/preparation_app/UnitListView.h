// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitListView_h_
#define __UnitListView_h_

#include "clients_gui/ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class AgentTypes;
    class AutomatComposition;
    class AutomatType;
    class ModelLoaded;
    class ModelUnLoaded;
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  UnitListView
    @brief  UnitListView
*/
// Created: SBO 2006-08-28
// =============================================================================
class UnitListView : public gui::ListView< UnitListView >
                   , public kernel::Observer_ABC
                   , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                   , public kernel::ElementObserver_ABC< kernel::ModelUnLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             UnitListView( QWidget* parent, kernel::Controllers& controllers, kernel::AgentTypes& list, gui::ItemFactory_ABC& factory );
    virtual ~UnitListView();
    //@}

    //! @name Operations
    //@{
    void SetOpen( bool open );
    void SetSorting( const std::string& nature );
    void Display( const kernel::AutomatComposition& type, gui::ValuedListItem* );
    void Display( const kernel::AutomatType& type, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UnitListView( const UnitListView& );            //!< Copy constructor
    UnitListView& operator=( const UnitListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void DisplayList();
    virtual QDragObject* dragObject();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::AgentTypes& types_;
    std::string sorting_;
    //@}
};

#endif // __UnitListView_h_
