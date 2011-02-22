// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCreationPanel_h_
#define __ObjectCreationPanel_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
    class Viewport_ABC;
}

namespace gui
{
    class ObjectPrototype_ABC;
    class PanelStack_ABC;
    class ParametersLayer;
}

class StaticModel;
class ObjectsModel;

// =============================================================================
/** @class  ObjectCreationPanel
    @brief  Object creation panel
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectCreationPanel : public gui::InfoPanel_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                          , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools, const tools::GeneralConfig& config );
    virtual ~ObjectCreationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( kernel::Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    gui::ObjectPrototype_ABC* created_;
    //@}
};

#endif // __ObjectCreationPanel_h_
