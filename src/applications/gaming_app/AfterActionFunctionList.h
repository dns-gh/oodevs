// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionFunctionList_h_
#define __AfterActionFunctionList_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"

namespace kernel
{
    class Controllers;
}

class Param_ABC;
class AfterActionParameter;
class AfterActionFunction;
class AfterActionFunctions;
class QToolBox;

// =============================================================================
/** @class  AfterActionFunctionList
    @brief  AfterActionFunctionList
*/
// Created: AGE 2007-09-21
// =============================================================================
class AfterActionFunctionList : public QVBox
                              , public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< AfterActionFunctions >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AfterActionFunctionList();
    //@}

    //! @name Operations
    //@{
    void Display( const AfterActionFunction& function, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

signals:
    //! @name Signals
    //@{
    void NewFunction();
    void EditFunction( const AfterActionFunction* function );
    void CreateRequest( const AfterActionFunction* function );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void EditFunction();
    void Request();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionFunctionList( const AfterActionFunctionList& );            //!< Copy constructor
    AfterActionFunctionList& operator=( const AfterActionFunctionList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AfterActionFunctions& );
    void CreateRequestButton();
    void CreateParameter( const AfterActionParameter& parameter );
    boost::shared_ptr< Param_ABC > CreateParameter( const std::string& type, const QString& name );
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Param_ABC > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ListDisplayer< AfterActionFunctionList >* functions_;
    QGroupBox* parameters_;
    QPushButton* request_;
    T_Parameters paramList_;
    //@}
};

#endif // __AfterActionFunctionList_h_
