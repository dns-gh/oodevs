// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesPanel_h_
#define __PropertiesPanel_h_

#include "clients_gui/PropertiesPanel.h"

namespace kernel
{
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class GlProxy;
    class PropertyDisplayer;
}

namespace tools
{
    class GeneralConfig;
}
class Model;
class StaticModel;

// =============================================================================
/** @class  PropertiesPanelBase
    @brief  Properties panel base member class
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesPanelBase
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanelBase( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                                  const tools::GeneralConfig& config );
    virtual ~PropertiesPanelBase();
    //@}

protected:
    //! @name Member data
    //@{
    std::unique_ptr< gui::EditorFactory_ABC > editorFactory_;
    std::unique_ptr< gui::PropertyDisplayer > propertiesComparator_;
    std::unique_ptr< gui::PropertyDisplayer > propertiesDisplayer_;
    //@}
};

// =============================================================================
/** @class  PropertiesPanel
    @brief  PropertiesPanel
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertiesPanel : private PropertiesPanelBase
                      , public gui::PropertiesPanel
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                             const gui::GlProxy& glProxy, const tools::GeneralConfig& config );
    virtual ~PropertiesPanel();
    //@}
};

#endif // __PropertiesPanel_h_
