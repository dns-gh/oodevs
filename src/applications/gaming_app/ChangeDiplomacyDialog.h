// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#ifndef __ChangeDiplomacyDialog_h_
#define __ChangeDiplomacyDialog_h_

#include "clients_gui/DiplomacyDialog_ABC.h"

namespace actions
{
    class ActionsModel;
}

class ActionPublisher;
class Simulation;
class StaticModel;

// =============================================================================
/** @class  ChangeDiplomacyDialog
    @brief  ChangeDiplomacyDialog
*/
// Created: SBO 2006-09-07
// =============================================================================
class ChangeDiplomacyDialog : public gui::DiplomacyDialog_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, const kernel::Profile_ABC& profile );
    virtual ~ChangeDiplomacyDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SetDiplomacy( const kernel::Team_ABC& team1, const kernel::Team_ABC& team2, const kernel::Karma& diplomacy ) const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ChangeDiplomacyDialog( const ChangeDiplomacyDialog& );
    ChangeDiplomacyDialog& operator=( const ChangeDiplomacyDialog& );
    //@}

private:
    //! @name Member data
    //@{
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const Simulation& simulation_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
