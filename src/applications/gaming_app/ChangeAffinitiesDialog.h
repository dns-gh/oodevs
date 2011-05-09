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
             ChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, bool optional = false );
    virtual ~ChangeAffinitiesDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Validate();
    void Activated( bool value );
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

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, gui::DecimalSpinBoxAndSlider* > T_SpinBoxs;
    typedef std::map< unsigned long, float > T_Affinities;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}

private:
    //! @name Member data
    //@{
    bool optional_;
    QVBoxLayout* mainLayout_;
    QCheckBox* checkBox_;
    QGrid* affinitiesGrid_;
    QHBox* buttonLayout_;
    T_SpinBoxs affinitiesSpinboxs_;
    T_Affinities affinities_;
    //@}
};

#endif // __ChangeAffinitiesDialog_h_
