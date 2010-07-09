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
    class TableItemDisplayer;
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
             PropertiesPanelBase( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~PropertiesPanelBase();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesPanelBase( const PropertiesPanelBase& );            //!< Copy constructor
    PropertiesPanelBase& operator=( const PropertiesPanelBase& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::EditorFactory_ABC > editorFactory_;
    std::auto_ptr< gui::TableItemDisplayer > tableItemDisplayer_;
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
             PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~PropertiesPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesPanel( const PropertiesPanel& );            //!< Copy constructor
    PropertiesPanel& operator=( const PropertiesPanel& ); //!< Assignment operator
    //@}
};

#endif // __PropertiesPanel_h_
