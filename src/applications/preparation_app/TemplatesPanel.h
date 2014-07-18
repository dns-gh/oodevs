// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TemplatesPanel_h_
#define __TemplatesPanel_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/InfoPanel_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class AgentTypes;
    class ModelLoaded;
    class ModelUnLoaded;
}

class AgentsModel;
class ColorController;
class FormationModel;
class GhostModel;
class TemplateListView;

// =============================================================================
/** @class  TemplatesPanel
    @brief  TemplatesPanel
*/
// Created: AGE 2007-05-30
// =============================================================================
class TemplatesPanel : public gui::InfoPanel_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                     , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TemplatesPanel( QWidget* parent,
                             gui::PanelStack_ABC& panel,
                             kernel::Controllers& controllers,
                             AgentsModel& agents,
                             FormationModel& formations,
                             GhostModel& ghosts,
                             const kernel::AgentTypes& types,
                             ColorController& colorController );
    virtual ~TemplatesPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTemplate();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyContextMenu( const kernel::Entity_ABC&, kernel::ContextMenu& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    TemplateListView* list_;
    kernel::SafePointer< kernel::Entity_ABC > menuEntity_;
    //@}
};

#endif // __TemplatesPanel_h_
