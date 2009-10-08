// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationTypesListView_h_
#define __PopulationTypesListView_h_

#include "clients_gui/ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class PopulationType;
    class ModelLoaded;
    class ModelUnLoaded;
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  PopulationTypesListView
    @brief  PopulationTypesListView
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationTypesListView : public gui::ListView< PopulationTypesListView >
                              , public tools::Observer_ABC
                              , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                              , public kernel::ElementObserver_ABC< kernel::ModelUnLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationTypesListView( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory );
    virtual ~PopulationTypesListView();
    //@}

    //! @name Operations
    //@{
    void Display( const kernel::PopulationType& type, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationTypesListView( const PopulationTypesListView& );            //!< Copy constructor
    PopulationTypesListView& operator=( const PopulationTypesListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual QDragObject* dragObject();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::PopulationType >& types_;
    //@}
};

#endif // __PopulationTypesListView_h_
