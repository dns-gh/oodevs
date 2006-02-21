// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentResourcesPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 11:38 $
// $Revision: 6 $
// $Workfile: AgentResourcesPanel.h $
//
// *****************************************************************************

#ifndef __AgentResourcesPanel_h_
#define __AgentResourcesPanel_h_

#include "Types.h"
#include "InfoPanel_ABC.h"
#include "ListView.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "Troops.h"

class Agent;
class Controller;
class ActionController;
class Dotation;
class Dotations;
class Equipments;
class Lends;
class Equipment;
class Lend;

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  Agent resources panel
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public InfoPanel_ABC
                          , private Observer_ABC
                          , public ElementObserver_ABC< Dotations >
                          , public ElementObserver_ABC< Equipments >
                          , public ElementObserver_ABC< Lends >
                          , public ElementObserver_ABC< Troops >
                          , public SelectionObserver< Agent >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentResourcesPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~AgentResourcesPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const Dotation& dotation, ValuedListItem* item );
    void Display( const Equipment& equipment, ValuedListItem* item );
    void Display( const Lend& lend, ValuedListItem* item );
    void Display( const Troops::Humans& humans, ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentResourcesPanel( const AgentResourcesPanel& );
    AgentResourcesPanel& operator=( const AgentResourcesPanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Dotations& a );
    virtual void NotifyUpdated( const Equipments& a );
    virtual void NotifyUpdated( const Lends& a );
    virtual void NotifyUpdated( const Troops& a );
    virtual void NotifySelected( const Agent* agent );

    template< typename T >
    bool ShouldUpdate( const T& a );
    //@}

    //! @name Types
    //@{
    typedef ListView< AgentResourcesPanel > T_ListView;
    //@}
    
private:
    //! @name Member Data
    //@{
    T_ListView* pEquipment_;
    T_ListView* pResources_;
    T_ListView* pTroops_;
    T_ListView* pLends_;
    const Agent* selected_;
    //@}
};

#endif // __AgentResourcesPanel_h_
