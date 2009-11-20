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
    class KnowledgeGroup_ABC;
}

class ModelBuilder;

// =============================================================================
/** @class  CommunicationListView
    @brief  CommunicationListView
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                            , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                            , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             CommunicationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder );
    virtual ~CommunicationListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
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
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgegroup, kernel::ContextMenu& menu );

    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual void keyPressEvent( QKeyEvent* event );
   //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ModelBuilder& modelBuilder_;
    //@}
};

#endif // __CommunicationListView_h_
