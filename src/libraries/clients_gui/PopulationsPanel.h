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

#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/PopulationPrototype.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelLoaded;
    class PopulationType;
    class Team_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
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
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             PopulationsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory );
    virtual ~PopulationsPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationsPanel( const PopulationsPanel& );            //!< Copy constructor
    PopulationsPanel& operator=( const PopulationsPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void AfterSelection();
    virtual void BeforeSelection();
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Team_ABC& element );
    //@}

private slots:
    //!@name Slots
    //@{
    void OnStartDrag( const kernel::PopulationType* type );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    PopulationTypesListView* list_;
    QSpinBox* number_;
    const kernel::Entity_ABC* selected_;
    kernel::PopulationPrototype prototype_;
    //@}
};
}

#endif // __PopulationsPanel_h_
