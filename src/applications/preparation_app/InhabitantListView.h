// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantListView_h_
#define __InhabitantListView_h_

#include "clients_gui/InhabitantListView.h"
#include "clients_kernel/SafePointer.h"

class ModelBuilder;

// =============================================================================
/** @class  InhabitantListView
    @brief  Inhabitant list view
*/
// Created: SLG 2010-11-23
// =============================================================================
class InhabitantListView : public gui::InhabitantListView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~InhabitantListView();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantListView( const InhabitantListView& );            //!< Copy constructor
    InhabitantListView& operator=( const InhabitantListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QDragObject* dragObject();
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void keyPressEvent( QKeyEvent* key );
    //@}

private:
    //! @name Member data
    //@{
    ModelBuilder& modelBuilder_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __InhabitantListView_h_
