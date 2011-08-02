// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorConditionItem_h_
#define __SuccessFactorConditionItem_h_

#include "clients_gui/ValuedComboBox.h"

class Score_ABC;
class ScoresModel;
class SuccessFactorCondition;
class SuccessFactorConditions;

// =============================================================================
/** @class  SuccessFactorConditionItem
    @brief  SuccessFactorConditionItem
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorConditionItem : public Q3HBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorConditionItem( QWidget* parent, const ScoresModel& scores );
    virtual ~SuccessFactorConditionItem();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const SuccessFactorCondition& condition );
    void CommitTo( SuccessFactorConditions& conditions ) const;
    void EnableDeletion( bool enable );
    //@}

signals:
    //! @name Signals
    //@{
    void Deleted( SuccessFactorConditionItem& item );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorConditionItem( const SuccessFactorConditionItem& );            //!< Copy constructor
    SuccessFactorConditionItem& operator=( const SuccessFactorConditionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FillOperators();
    //@}

private:
    //! @name Member data
    //@{
    gui::ValuedComboBox< const Score_ABC* >* score_;
    gui::ValuedComboBox< QString >* operator_;
    QLineEdit* value_;
    QPushButton* deleteButton_;
    //@}
};

#endif // __SuccessFactorConditionItem_h_
