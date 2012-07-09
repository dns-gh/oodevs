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

#include <boost/noncopyable.hpp>
#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/SubTypes.h"
#include "ModalDialog.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  CriticalIntelligenceDialog
    @brief  CriticalIntelligenceDialog
*/
// Created: ABR 2012-07-05
// =============================================================================
class CriticalIntelligenceDialog : public QDialog
                                 , public kernel::ValueEditor< kernel::CriticalIntelligence >
                                 , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligenceDialog( QWidget* parent, kernel::Controllers& controllers );
             CriticalIntelligenceDialog( QDialog*& self, QWidget* parent, kernel::Controllers& controllers );
    virtual ~CriticalIntelligenceDialog();
    //@}

    //! @name Operations
    //@{
    void SetValue( kernel::CriticalIntelligence& value );
    virtual kernel::CriticalIntelligence GetValue();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnAccept();
    virtual void OnReject();
    virtual void OnShow();
    //@}

private:
    //! @name Helper
    //@{
    void CreateGUI();
    //@}

protected:
    //! @name Member data
    //@{
    QDialog**                     self_;
    kernel::Controllers&          controllers_;
    kernel::CriticalIntelligence* criticalIntelligence_;
    QTextEdit*                    textEdit_;
    //@}
};

} //! namespace gui

#endif // __gui_CriticalIntelligenceDialog_h_
