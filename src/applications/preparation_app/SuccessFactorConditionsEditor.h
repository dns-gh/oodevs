// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorConditionsEditor_h_
#define __SuccessFactorConditionsEditor_h_

class ScoresModel;
class SuccessFactorConditions;
class SuccessFactorConditionItem;

// =============================================================================
/** @class  SuccessFactorConditionsEditor
    @brief  SuccessFactorConditionsEditor
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorConditionsEditor : public QVBoxLayout
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorConditionsEditor( const QString& objectName, const ScoresModel& scores );
    virtual ~SuccessFactorConditionsEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const SuccessFactorConditions& conditions );
    void CommitTo( SuccessFactorConditions& conditions ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    SuccessFactorConditionItem* CreateItem();
    void OnDelete( SuccessFactorConditionItem& item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SuccessFactorConditionItem* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    const ScoresModel& scores_;
    QRadioButton* orButton_;
    QRadioButton* andButton_;
    QScrollArea* scrollArea_;
    T_Items items_;
    //@}
};

#endif // __SuccessFactorConditionsEditor_h_
