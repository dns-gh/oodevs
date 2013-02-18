// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Properties_h_
#define __Properties_h_

#include "clients_gui/RichDockWidget.h"

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


// =============================================================================
/** @class  PropertiesBase
    @brief  Properties panel base member class
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesBase
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesBase();
    virtual ~PropertiesBase();
    //@}

protected:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::EditorFactory_ABC > editorFactory_;
    std::auto_ptr< gui::PropertyDisplayer > propertyDisplayer_;
    std::auto_ptr< gui::PropertyDisplayer > propertyComparator_;
    //@}
};

// =============================================================================
/** @class  Properties
    @brief  Properties
*/
// Created: SBO 2006-10-19
// =============================================================================
class Properties : private PropertiesBase
                 , public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             Properties( QWidget* parent, kernel::Controllers& controllers, const gui::GlProxy& glProxy );
    virtual ~Properties();
    //@}
};

#endif // __Properties_h_
