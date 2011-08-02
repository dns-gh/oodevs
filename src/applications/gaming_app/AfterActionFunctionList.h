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
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {
        class Param_ABC;
    }
}

class AfterActionParameter;
class AfterActionFunction;
class AfterActionModel;
class Simulation;
class StaticModel;

// =============================================================================
/** @class  AfterActionFunctionList
    @brief  AfterActionFunctionList
*/
// Created: AGE 2007-09-21
// =============================================================================
class AfterActionFunctionList : public Q3VBox
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< Simulation >
                              , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model,
                                      gui::ParametersLayer& layer, const StaticModel& staticModel );
    virtual ~AfterActionFunctionList();
    //@}

    //! @name Operations
    //@{
    void Display( const AfterActionFunction& function, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( Q3ListViewItem* item );
    void Request();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    void CreateRequestButton();
    void CreateParameter( const AfterActionParameter& parameter );
    boost::shared_ptr< actions::gui::Param_ABC > CreateParameter( const std::string& type, const QString& name );
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< actions::gui::Param_ABC > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    AfterActionModel& model_;
    gui::ParametersLayer& layer_;
    const StaticModel& staticModel_;
    gui::ListDisplayer< AfterActionFunctionList >* functions_;
    Q3GroupBox* parameters_;
    Q3VGroupBox* timeGroup_;
    QSpinBox* firstTick_;
    QSpinBox* duration_;
    QPushButton* request_;
    T_Parameters paramList_;
    //@}
};

#endif // __AfterActionFunctionList_h_
