// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionsEditor_h_
#define __SuccessFactorActionsEditor_h_

class SuccessFactorActions;
class SuccessFactorActionItem;
class SuccessFactorActionTypes;

// =============================================================================
/** @class  SuccessFactorActionsEditor
    @brief  SuccessFactorActionsEditor
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorActionsEditor : public QVBoxLayout
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActionsEditor( const QString& objectName, const SuccessFactorActionTypes& actionTypes );
    virtual ~SuccessFactorActionsEditor();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const SuccessFactorActions& actions );
    void CommitTo( SuccessFactorActions& actions ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    SuccessFactorActionItem* CreateItem();
    void OnDelete( SuccessFactorActionItem& item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SuccessFactorActionItem* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    const SuccessFactorActionTypes& actionTypes_;
    T_Items items_;
    QScrollArea* scrollArea_;
    //@}
};

#endif // __SuccessFactorActionsEditor_h_
