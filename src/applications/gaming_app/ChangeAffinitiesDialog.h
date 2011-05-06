// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ChangeAffinitiesDialog_h_
#define __ChangeAffinitiesDialog_h_

#include "clients_kernel/SafePointer.h"
#include "gaming/AffinitiesVisitor_ABC.h"

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

// =============================================================================
/** @class  ChangeAffinitiesDialog
    @brief  Change affinities dialog
*/
// Created: ABR 2011-01-25
// =============================================================================
class ChangeAffinitiesDialog : public QDialog
                             , private AffinitiesVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~ChangeAffinitiesDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent * e );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Visit( unsigned long id,const std::string& team, float& affinity );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate() = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChangeAffinitiesDialog( const ChangeAffinitiesDialog& );            //!< Copy constructor
    ChangeAffinitiesDialog& operator=( const ChangeAffinitiesDialog& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}

private:
    //! @name Member data
    //@{
    QVBoxLayout* mainLayout_;
    QGrid* affinitiesGrid_;
    QHBox* buttonLayout_;
    std::map< unsigned long, gui::DecimalSpinBoxAndSlider* > affinitiesSpinboxs_;
    //@}
};

#endif // __ChangeAffinitiesDialog_h_
