// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************


#ifndef __ChangeDiplomacyDialog_h_
#define __ChangeDiplomacyDialog_h_

#include "clients_gui/DiplomacyDialog_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Time_ABC;
}

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
             ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
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
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
