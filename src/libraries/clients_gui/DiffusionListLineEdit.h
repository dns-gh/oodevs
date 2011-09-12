// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListLineEdit_h_
#define gui_DiffusionListLineEdit_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
}

namespace gui
{
    class DiffusionListDialog;

// =============================================================================
/** @class  DiffusionListLineEdit
    @brief  DiffusionListLineEdit
*/
// Created: ABR 2011-04-29
// =============================================================================
class DiffusionListLineEdit : public QLineEdit
                            , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListLineEdit( QWidget* parent, kernel::SafePointer< kernel::Entity_ABC > selected, DiffusionListDialog& diffusionDialog_, const std::string extensionName, const char* name = 0 );
    virtual ~DiffusionListLineEdit();
    //@}

    //! @name Operations
    //@{
    virtual void focusInEvent( QFocusEvent * );
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnAccepted( const QString& );
    void OnRejected();
    //@}

private:
    //! @name Member data
    //@{
    DiffusionListDialog& dialog_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const std::string extensionName_;
    //@}
};

}

#endif // gui_DiffusionListLineEdit_h_
