// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PresetDialog_h_
#define __PresetDialog_h_

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class Gradient;
    class RichPushButton;
    class RichLineEdit;
}

namespace gui
{
// =============================================================================
/** @class  PresetDialog
    @brief  PresetDialog
*/
// Created: LGY 2010-09-24
// =============================================================================
class PresetDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PresetDialog( QWidget* parent, Gradient& gradient, kernel::OptionsController& options,
                           const std::vector< std::string >& presets );
    virtual ~PresetDialog();
    //@}

private slots:
    //! @name slots
    //@{
    void Accept();
    void Reject();
    //@}

private:
    //! @name helpers
    //@{
    void CreateContent();
    void CreateButton();
    //@}

private:
    //! @name Member data
    //@{
    Q3VBoxLayout* pLayout_;
    RichLineEdit* pLineEdit_;
    RichPushButton* pAcceptButton_;
    RichPushButton* pRejectButton_;
    Gradient& gradient_;
    kernel::OptionsController& options_;
    const std::vector< std::string >& presets_;
    //@}
};
}
#endif // __PresetDialog_h_
