// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorList_h_
#define __SuccessFactorList_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class RichPushButton;
    template< typename T > class RichWidget;
}

class ScoresModel;
class SuccessFactor;
class SuccessFactorActionTypes;
class SuccessFactorEditor;
class SuccessFactorsModel;

// =============================================================================
/** @class  SuccessFactorList
    @brief  SuccessFactorList
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorList : public QVBoxLayout
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< SuccessFactor >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorList( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores, SuccessFactorsModel& success );
    virtual ~SuccessFactorList();
    //@}

signals:
    //! @name Signals
    //@{
    void Deleted( const SuccessFactor& factor );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEdit();
    void OnDelete();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const SuccessFactor& element );
    virtual void NotifyUpdated( const SuccessFactor& element );
    virtual void NotifyDeleted( const SuccessFactor& element );
    SuccessFactor* FindSelected() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::RichWidget< QTreeWidget >* factors_;
    SuccessFactorEditor* editor_;
    gui::RichPushButton* editButton_;
    gui::RichPushButton* deleteButton_;
    //@}
};

#endif // __SuccessFactorList_h_
