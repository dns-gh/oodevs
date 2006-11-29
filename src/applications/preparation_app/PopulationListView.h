// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationListView_h_
#define __PopulationListView_h_

#include "clients_gui/PopulationListView.h"
#include "clients_kernel/SafePointer.h"

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: SBO 2006-10-16
// =============================================================================
class PopulationListView : public gui::PopulationListView
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~PopulationListView();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationListView( const PopulationListView& );            //!< Copy constructor
    PopulationListView& operator=( const PopulationListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void keyPressEvent( QKeyEvent* key );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __PopulationListView_h_
