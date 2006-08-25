// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_AgentListView_h_
#define __Gui_AgentListView_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Agent_ABC;
    class Controllers;
}

namespace gui
{
    class ValuedListItem;
    class ItemFactory_ABC;

// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public ListView< AgentListView >
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                    , public kernel::ElementObserver_ABC< kernel::KnowledgeGroup_ABC >
                    , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Team_ABC > // $$$$ AGE 2006-03-21: refactor these crap
                    , public kernel::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
                    , public kernel::SelectionObserver_ABC
                    , public kernel::ActivationObserver_ABC< kernel::Agent_ABC >
                    , public kernel::OptionsObserver_ABC
{
   Q_OBJECT;

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentListView();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Team_ABC& team,            ValuedListItem* item );
    virtual void Display( const kernel::KnowledgeGroup_ABC& group, ValuedListItem* item );
    virtual void Display( const kernel::Agent_ABC& agent,          ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentListView( const AgentListView& );
    AgentListView& operator=( const AgentListView& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    //@}

    //! @name Helpers
    //@{
private:
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );

    virtual void NotifyUpdated( const kernel::KnowledgeGroup_ABC& group );
    virtual void NotifyUpdated( const kernel::Agent_ABC& agent );

    virtual void Select( const kernel::Team_ABC& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void NotifyActivated( const kernel::Agent_ABC& element );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    template< typename ParentType, typename ChildType >
    void RecursiveDisplay( const ParentType& value, ValuedListItem* item );

    template< typename Type >
    void Update( const Type& value );

    QDragObject* dragObject();
    void dropEvent( QDropEvent* pEvent );
    void dragEnterEvent( QDragEnterEvent* pEvent );

    bool Drop( ValuedListItem& item, ValuedListItem& target );
    template< typename T >
    bool DoDrop( ValuedListItem& item, ValuedListItem& target );

    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::KnowledgeGroup_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::Team_ABC& target );
    virtual bool Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::KnowledgeGroup_ABC& target );
    virtual bool Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Team_ABC& target );
    virtual bool Drop( const kernel::Team_ABC& item, const kernel::Agent_ABC& target );
    virtual bool Drop( const kernel::Team_ABC& item, const kernel::KnowledgeGroup_ABC& target );
    virtual bool Drop( const kernel::Team_ABC& item, const kernel::Team_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Team_ABC* currentTeam_;
    //@}
};

}

#endif // __Gui_AgentListView_h_
