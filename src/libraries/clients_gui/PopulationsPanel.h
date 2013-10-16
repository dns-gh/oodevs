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

#include "InfoPanel_ABC.h"

#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Formation_ABC;
    class ModelLoaded;
    class PopulationType;
    class Team_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    class PopulationTypesListView;

// =============================================================================
/** @class  PopulationsPanel
    @brief  PopulationsPanel
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationsPanel : public gui::InfoPanel_ABC
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                       , public tools::SelectionObserver_ABC
                       , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                       , public tools::SelectionObserver_Base< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types );
    virtual ~PopulationsPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void AfterSelection();
    virtual void BeforeSelection();
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Team_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    PopulationTypesListView* list_;
    QSpinBox* number_;
    //@}
};
}

#endif // __PopulationsPanel_h_
