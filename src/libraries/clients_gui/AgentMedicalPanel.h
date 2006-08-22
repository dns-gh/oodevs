// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentMedicalPanel_h_
#define __AgentMedicalPanel_h_

#include "LogisticPanel.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/LogMedicalConsign.h"
#include "clients_kernel/Agent_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

class MedicalStates;
class Availability;

namespace gui
{
    class ValuedListItem;
    class DisplayBuilder;

// =============================================================================
/** @class  AgentMedicalPanel
    @brief  Agent_ABC medical panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMedicalPanel : public LogisticPanel< AgentMedicalPanel, LogMedicalConsign >
                        , public kernel::ElementObserver_ABC< MedicalStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMedicalPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, ItemFactory_ABC& factory );
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
    void NotifySelected( const kernel::Agent_ABC& agent );
    void NotifyUpdated( const MedicalStates& consigns );

    virtual void DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentMedicalPanel >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentMedicalPanel >* list );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    DisplayBuilder* display_;
    ListDisplayer< AgentMedicalPanel >* dispoReleveAmbulances_;
    ListDisplayer< AgentMedicalPanel >* dispoDispoRamassageAmbulances_;
    ListDisplayer< AgentMedicalPanel >* dispoDispoDoctors_;
    //@}
};

}

#endif // __AgentMedicalPanel_h_
