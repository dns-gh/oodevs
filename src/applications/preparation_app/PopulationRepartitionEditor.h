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

#include "clients_gui/PropertyDialog.h"
#include "clients_kernel/ValueEditor.h"

namespace kernel
{
    class Controller;
}

namespace gui
{
    class RichPushButton;
    class RichLineEdit;
}

class PopulationRepartition;

// =============================================================================
/** @class  PopulationRepartitionEditor
    @brief  Population repartition editor
*/
// Created: JSR 2011-03-08
// =============================================================================
class PopulationRepartitionEditor : public gui::PropertyDialog
                                  , public kernel::ValueEditor< PopulationRepartition* >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PopulationRepartitionEditor( QWidget* parent, kernel::Controller& controller );
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
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    PopulationRepartition** value_;
    gui::RichLineEdit* male_;
    gui::RichLineEdit* female_;
    gui::RichLineEdit* children_;
    gui::RichPushButton* okBtn_;
    QLabel* warning_;
    //@}
};

#endif // __PopulationRepartitionEditor_h_
