// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Properties_h_
#define __Properties_h_

#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesWidget;
}

// =============================================================================
/** @class  Properties
    @brief  Properties
*/
// Created: SBO 2006-10-19
// =============================================================================
class Properties : public QScrollView
                 , public kernel::Observer_ABC
                 , public kernel::SelectionObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Properties( QWidget* parent, kernel::Controllers& controllers );
    virtual ~Properties();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Properties( const Properties& );            //!< Copy constructor
    Properties& operator=( const Properties& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void insertChild ( QObject* pObj );
    virtual QLayout* layout();
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    QVBox* pBox_;
    kernel::Controllers& controllers_;
    gui::PropertiesWidget* table_;
    //@}
};

#endif // __Properties_h_
