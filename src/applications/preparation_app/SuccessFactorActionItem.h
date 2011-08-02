// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionItem_h_
#define __SuccessFactorActionItem_h_

#include "clients_gui/ValuedComboBox.h"

class SuccessFactorAction;
class SuccessFactorActions;
class SuccessFactorActionType;
class SuccessFactorActionTypes;
class SuccessFactorActionTypeParameter;

// =============================================================================
/** @class  SuccessFactorActionItem
    @brief  SuccessFactorActionItem
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorActionItem : public Q3VBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActionItem( QWidget* parent, const SuccessFactorActionTypes& actions );
    virtual ~SuccessFactorActionItem();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const SuccessFactorAction& action );
    void CommitTo( SuccessFactorActions& actions ) const;
    void EnableDeletion( bool enable );
    //@}

signals:
    //! @name Signals
    //@{
    void Add();
    void Deleted( SuccessFactorActionItem& item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionItem( const SuccessFactorActionItem& );            //!< Copy constructor
    SuccessFactorActionItem& operator=( const SuccessFactorActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddParameter( const QString& name, const QString& value );
    void SetParameter( const QString& name, const QString& value );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QLineEdit* > T_ParameterEditors;
    //@}

private:
    //! @name Member data
    //@{
    gui::ValuedComboBox< const SuccessFactorActionType* >* type_;
    Q3GroupBox* parameters_;
    T_ParameterEditors editors_;
    QPushButton* deleteButton_;
    //@}
};

#endif // __SuccessFactorActionItem_h_
