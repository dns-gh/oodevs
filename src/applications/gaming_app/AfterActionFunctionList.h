// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionFunctionList_h_
#define __AfterActionFunctionList_h_

#include "clients_gui/ListDisplayer.h"

namespace kernel
{
    class Controllers;
}

class Param_ABC;
class AfterActionParameter;
class AfterActionFunction;
class AfterActionModel;
class QToolBox;

// =============================================================================
/** @class  AfterActionFunctionList
    @brief  AfterActionFunctionList
*/
// Created: AGE 2007-09-21
// =============================================================================
class AfterActionFunctionList : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model );
    virtual ~AfterActionFunctionList();
    //@}

    //! @name Operations
    //@{
    void Display( const AfterActionFunction& function, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
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
    AfterActionModel& model_;
    QGroupBox* parameters_;
    QPushButton* request_;
    T_Parameters paramList_;
    //@}
};

#endif // __AfterActionFunctionList_h_
