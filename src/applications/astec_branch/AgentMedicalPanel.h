// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMedicalPanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentMedicalPanel.h $
//
// *****************************************************************************

#ifndef __AgentMedicalPanel_h_
#define __AgentMedicalPanel_h_

#include "InfoPanel_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"

class DisplayBuilder;
class Controllers;
template< typename T > class ListDisplayer;
class Displayer_ABC;
class ValuedListItem;

class LogisticConsigns;
class MedicalStates;
class Agent;
class LogMedicalConsign;
class Availability;
class SubItemDisplayer;

// =============================================================================
/** @class  AgentMedicalPanel
    @brief  Agent medical panel
    // $$$$ AGE 2006-02-28: Factorisations entre panels logistiques !
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMedicalPanel : public InfoPanel_ABC
                        , private Observer_ABC
                        , public SelectionObserver< Agent >
                        , public ElementObserver_ABC< LogisticConsigns >
                        , public ElementObserver_ABC< MedicalStates >
                        , public ElementObserver_ABC< LogMedicalConsign >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMedicalPanel( InfoPanels* pParent, Controllers& controllers );
    virtual ~AgentMedicalPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogMedicalConsign* consign, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    void NotifySelected( const Agent* agent );
    void NotifyUpdated( const LogisticConsigns& consigns );
    void NotifyUpdated( const MedicalStates& consigns );
    void NotifyUpdated( const LogMedicalConsign& consign );
    template< typename Extension >
    bool ShouldUpdate( const Extension& e );
    void showEvent( QShowEvent* );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const Agent* selected_;
    ListDisplayer< AgentMedicalPanel >* pConsignListView_;
    ListDisplayer< AgentMedicalPanel >* pConsignHandledListView_;
    SubItemDisplayer* logDisplay_;

    DisplayBuilder* display_;

    ListDisplayer< AgentMedicalPanel >* dispoReleveAmbulances_;
    ListDisplayer< AgentMedicalPanel >* dispoDispoRamassageAmbulances_;
    ListDisplayer< AgentMedicalPanel >* dispoDispoDoctors_;
    //@}
};

#endif // __AgentMedicalPanel_h_
