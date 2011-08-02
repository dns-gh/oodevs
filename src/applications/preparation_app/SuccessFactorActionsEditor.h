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
class SuccessFactorActionsEditor : public Q3ScrollView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActionsEditor( QWidget* parent, const SuccessFactorActionTypes& actionTypes );
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
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionsEditor( const SuccessFactorActionsEditor& );            //!< Copy constructor
    SuccessFactorActionsEditor& operator=( const SuccessFactorActionsEditor& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< SuccessFactorActionItem* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    const SuccessFactorActionTypes& actionTypes_;
    Q3VBox* mainWidget_;
    T_Items items_;
    //@}
};

#endif // __SuccessFactorActionsEditor_h_
