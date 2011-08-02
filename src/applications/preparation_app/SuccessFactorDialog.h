// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorDialog_h_
#define __SuccessFactorDialog_h_

namespace gui
{
    class ItemFactory_ABC;
}

namespace kernel
{
    class Controllers;
}

class ScoresModel;
class SuccessFactor;
class SuccessFactorActionTypes;
class SuccessFactorsModel;

// =============================================================================
/** @class  SuccessFactorDialog
    @brief  SuccessFactorDialog
*/
// Created: SBO 2009-06-12
// =============================================================================
class SuccessFactorDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorDialog( QWidget* parent, kernel::Controllers& controllers, SuccessFactorsModel& model, gui::ItemFactory_ABC& factory, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores );
    virtual ~SuccessFactorDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTextChanged( const QString& text );
    void OnCreateButtonClicked();
    void OnDelete( const SuccessFactor& factor );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorDialog( const SuccessFactorDialog& );            //!< Copy constructor
    SuccessFactorDialog& operator=( const SuccessFactorDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    virtual void showEvent( QShowEvent* e );
    //@}

private:
    //! @name Member data
    //@{
    SuccessFactorsModel& model_;
    const ScoresModel& scores_;
    QLineEdit* editor_;
    QPushButton* createButton_;
    //@}
};

#endif // __SuccessFactorDialog_h_
