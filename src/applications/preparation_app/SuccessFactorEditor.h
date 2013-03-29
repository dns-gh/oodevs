// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorEditor_h_
#define __SuccessFactorEditor_h_

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class RichPushButton;
    class RichLineEdit;
}

class ScoresModel;
class SuccessFactor;
class SuccessFactorsModel;
class SuccessFactorActionsEditor;
class SuccessFactorActionTypes;
class SuccessFactorConditionsEditor;
class SuccessFactorProfileList;

// =============================================================================
/** @class  SuccessFactorEditor
    @brief  SuccessFactorEditor
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorEditor : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorEditor( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores, SuccessFactorsModel& success );
    virtual ~SuccessFactorEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( SuccessFactor& factor );
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void Cancel();
    void NameChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    SuccessFactorsModel& success_;
    SuccessFactor* current_;
    gui::RichLineEdit* name_;
    SuccessFactorProfileList* profiles_;
    SuccessFactorConditionsEditor* conditions_;
    SuccessFactorActionsEditor* actions_;
    gui::RichPushButton* ok_;
    bool nameChanged_;
    //@}
};

#endif // __SuccessFactorEditor_h_
