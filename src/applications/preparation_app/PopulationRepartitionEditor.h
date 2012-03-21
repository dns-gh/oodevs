// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationRepartitionEditor_h_
#define __PopulationRepartitionEditor_h_

#include "clients_gui/ModalDialog.h"
#include "clients_kernel/ValueEditor.h"

namespace kernel
{
    class Controller;
}

class PopulationRepartition;

// =============================================================================
/** @class  PopulationRepartitionEditor
    @brief  PopulationRepartitionEditor
*/
// Created: JSR 2011-03-08
// =============================================================================
class PopulationRepartitionEditor : public ModalDialog
                                  , public kernel::ValueEditor< PopulationRepartition* >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PopulationRepartitionEditor( QDialog*& self, QWidget* parent, kernel::Controller& controller );
    virtual ~PopulationRepartitionEditor();
    //@}

    //! @name Operations
    //@{
    void SetValue( PopulationRepartition*& value );
    virtual PopulationRepartition* GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged();
    void OnAccept();
    void OnReject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationRepartitionEditor( const PopulationRepartitionEditor& );            //!< Copy constructor
    PopulationRepartitionEditor& operator=( const PopulationRepartitionEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    QDialog*& self_;
    kernel::Controller& controller_;
    PopulationRepartition** value_;
    QLineEdit* male_;
    QLineEdit* female_;
    QLineEdit* children_;
    QPushButton* okBtn_;
    QLabel* warning_;
    //@}
};

#endif // __PopulationRepartitionEditor_h_
