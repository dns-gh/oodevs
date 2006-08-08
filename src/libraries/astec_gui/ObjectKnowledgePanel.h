// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgePanel_h_
#define __ObjectKnowledgePanel_h_

#include "Types.h"
#include "InfoPanel_ABC.h"
#include "astec_gaming/TeamSelectionObserver.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/SafePointer.h"

template< typename T > class ListDisplayer;
class Team;
class ObjectKnowledges;
class ObjectKnowledge;
class CampAttributes;
class CrossingSiteAttributes;
class LogisticRouteAttributes;
class NBCAttributes;
class RotaAttributes;
class DisplayBuilder;
class ObjectKnowledges;
class Controllers;
class Displayer_ABC;
class ValuedListItem;
class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectKnowledgePanel
    @brief  ObjectKnowledgePanel
*/
// Created: APE 2004-05-04
// =============================================================================
class ObjectKnowledgePanel : public InfoPanel_ABC
                           , public Observer_ABC
                           , public ElementObserver_ABC< ObjectKnowledges >
                           , public ElementObserver_ABC< ObjectKnowledge >
                           , public ElementObserver_ABC< CampAttributes >
                           , public ElementObserver_ABC< CrossingSiteAttributes >
                           , public ElementObserver_ABC< LogisticRouteAttributes >
                           , public ElementObserver_ABC< NBCAttributes >
                           , public ElementObserver_ABC< RotaAttributes >
                           , public TeamSelectionObserver
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const ObjectKnowledge& k, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Agent_ABC* agent, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* );
    void ToggleDisplayOwnTeam();
    void OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ObjectKnowledgePanel( const ObjectKnowledgePanel& );
    ObjectKnowledgePanel& operator=( const ObjectKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const ObjectKnowledges& element );
    virtual void NotifyUpdated( const ObjectKnowledge& element );
    virtual void NotifyUpdated( const CampAttributes& element );
    virtual void NotifyUpdated( const CrossingSiteAttributes& element );
    virtual void NotifyUpdated( const LogisticRouteAttributes& element );
    virtual void NotifyUpdated( const NBCAttributes& element );
    virtual void NotifyUpdated( const RotaAttributes& element );
    template< typename T >
    void DisplayExtension( const T& extension );
    template< typename T >
    void UpdateExtension( const ObjectKnowledge& k );
    virtual void Select( const Team* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;

    SafePointer< Team > owner_;
    SafePointer< ObjectKnowledges > selected_;
    ListDisplayer< ObjectKnowledgePanel >* pKnowledgeListView_;

    QCheckBox* pOwnTeamCheckBox_;
    SafePointer< ObjectKnowledge > subSelected_;
    DisplayBuilder* display_;

    ListDisplayer< ObjectKnowledgePanel >* pPerceptionListView_;
    //@}
};

#endif // __ObjectKnowledgePanel_h_
