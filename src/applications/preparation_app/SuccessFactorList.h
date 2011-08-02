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
#include "clients_gui/ListDisplayer.h"

namespace kernel
{
    class Controllers;
}

class ScoresModel;
class SuccessFactor;
class SuccessFactorActionTypes;
class SuccessFactorEditor;

// =============================================================================
/** @class  SuccessFactorList
    @brief  SuccessFactorList
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorList : public Q3VBox
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< SuccessFactor >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores );
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
    //! @name Copy/Assignment
    //@{
    SuccessFactorList( const SuccessFactorList& );            //!< Copy constructor
    SuccessFactorList& operator=( const SuccessFactorList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const SuccessFactor& element );
    virtual void NotifyUpdated( const SuccessFactor& element );
    virtual void NotifyDeleted( const SuccessFactor& element );
    void Display( const SuccessFactor& factor, gui::ValuedListItem* item );
    SuccessFactor* FindSelected() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::ListDisplayer< SuccessFactorList >* factors_;
    SuccessFactorEditor* editor_;
    QPushButton* editButton_;
    QPushButton* deleteButton_;
    //@}
};

#endif // __SuccessFactorList_h_
