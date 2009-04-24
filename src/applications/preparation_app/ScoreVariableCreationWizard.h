// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreVariableCreationWizard_h_
#define __ScoreVariableCreationWizard_h_

class IndicatorElement_ABC;

// =============================================================================
/** @class  ScoreVariableCreationWizard
    @brief  ScoreVariableCreationWizard
*/
// Created: SBO 2009-04-21
// =============================================================================
class ScoreVariableCreationWizard : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScoreVariableCreationWizard( QWidget* parent );
    virtual ~ScoreVariableCreationWizard();
    //@}

    //! @name Operations
    //@{
    void Create();
    //@}

signals:
    //! @name Signals
    //@{
    void VariableCreated( const IndicatorElement_ABC& variable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreVariableCreationWizard( const ScoreVariableCreationWizard& );            //!< Copy constructor
    ScoreVariableCreationWizard& operator=( const ScoreVariableCreationWizard& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* name_;
    QComboBox* type_;
    QLineEdit* value_;
    //@}
};

#endif // __ScoreVariableCreationWizard_h_
