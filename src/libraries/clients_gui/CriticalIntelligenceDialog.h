// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_CriticalIntelligenceDialog_h_
#define __gui_CriticalIntelligenceDialog_h_

#include "PropertyDialog.h"

#include "clients_kernel/SubTypes.h"
#include "clients_kernel/ValueEditor.h"

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
// =============================================================================
/** @class  CriticalIntelligenceDialog
    @brief  Critical intelligence dialog
*/
// Created: ABR 2012-07-05
// =============================================================================
class CriticalIntelligenceDialog : public gui::PropertyDialog
                                 , public kernel::ValueEditor< kernel::CriticalIntelligenceType >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligenceDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~CriticalIntelligenceDialog();
    //@}

    //! @name Operations
    //@{
    void SetValue( kernel::CriticalIntelligenceType& value );
    virtual kernel::CriticalIntelligenceType GetValue();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnAccept();
    virtual void OnReject();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers&              controllers_;
    kernel::CriticalIntelligenceType* criticalIntelligence_;
    QTextEdit*                        textEdit_;
    //@}
};

}

#endif // __gui_CriticalIntelligenceDialog_h_
