// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationsPanel_h_
#define __PopulationsPanel_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class PopulationType;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
}

class PopulationTypesListView;

// =============================================================================
/** @class  PopulationsPanel
    @brief  PopulationsPanel
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationsPanel : public gui::InfoPanel_ABC
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory );
    virtual ~PopulationsPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationsPanel( const PopulationsPanel& );            //!< Copy constructor
    PopulationsPanel& operator=( const PopulationsPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    PopulationTypesListView* list_;
    //@}
};

#endif // __PopulationsPanel_h_
