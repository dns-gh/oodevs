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

// =============================================================================
/** @class  PropertiesBase
    @brief  Properties panel base member class
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesBase
{
public:
    //! @name Constructors/Destructors
    //@{
             PropertiesBase();
    virtual ~PropertiesBase();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesBase( const PropertiesBase& );            //!< Copy constructor
    PropertiesBase& operator=( const PropertiesBase& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::EditorFactory_ABC > editorFactory_;
    std::auto_ptr< gui::TableItemDisplayer > tableItemDisplayer_;
    //@}
};

// =============================================================================
/** @class  Properties
    @brief  Properties
*/
// Created: SBO 2006-10-19
// =============================================================================
class Properties : private PropertiesBase
                 , public gui::PropertiesPanel
{

public:
    //! @name Constructors/Destructor
    //@{
             Properties( QWidget* parent, kernel::Controllers& controllers );
    virtual ~Properties();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Properties( const Properties& );            //!< Copy constructor
    Properties& operator=( const Properties& ); //!< Assignment operator
    //@}
};

#endif // __Properties_h_
