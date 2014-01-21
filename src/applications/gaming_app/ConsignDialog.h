// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ConsignDialog_h_
#define __ConsignDialog_h_

#include "clients_gui/RichWidget.h"

namespace actions
{
    class ActionsModel;
}

class LogisticsConsign_ABC;

// =============================================================================
/** @class  ConsignDialog
    @brief  ConsignDialog
*/
// Created: LGY 2014-07-08
// =============================================================================
class ConsignDialog : public gui::RichWidget< QDialog >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ConsignDialog( const QString& objectName, QWidget* parent,
                            actions::ActionsModel& actionsModel );
    virtual ~ConsignDialog();
    //@}

    //! @name Operations
    //@{
    void Show( const LogisticsConsign_ABC& consign );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnApply();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __ConsignDialog_h_
