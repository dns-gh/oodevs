// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommunicationListView_h_
#define __CommunicationListView_h_

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_gui/HierarchyListView.h"

namespace kernel
{
    class ModelLoaded;
}

class ModelBuilder;

// =============================================================================
/** @class  CommunicationListView
    @brief  CommunicationListView
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                            , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                            , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             CommunicationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~CommunicationListView();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CommunicationListView( const CommunicationListView& );            //!< Copy constructor
    CommunicationListView& operator=( const CommunicationListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::Entity_ABC& );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ModelBuilder& modelBuilder_;
    //@}
};

#endif // __CommunicationListView_h_
