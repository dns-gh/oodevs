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

class ScoresModel;
class SuccessFactor;
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
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorEditor( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores );
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorEditor( const SuccessFactorEditor& );            //!< Copy constructor
    SuccessFactorEditor& operator=( const SuccessFactorEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    SuccessFactor* current_;
    QLineEdit* name_;
    SuccessFactorProfileList* profiles_;
    SuccessFactorConditionsEditor* conditions_;
    SuccessFactorActionsEditor* actions_;
    QPushButton* ok_;
    //@}
};

#endif // __SuccessFactorEditor_h_
