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
#include "actions_gui/ParamInterface_ABC.h"
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
        class InterfaceBuilder_ABC;
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
                              , public actions::gui::ParamInterface_ABC
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< Simulation >
                              , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model,
                                      actions::gui::InterfaceBuilder_ABC& interfaceBuilder );
    virtual ~AfterActionFunctionList();
    //@}

    //! @name Operations
    //@{
    void Display( const AfterActionFunction& function, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( actions::gui::Param_ABC* param ) const;
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
    actions::gui::InterfaceBuilder_ABC& builder_;
    kernel::Controllers& controllers_;
    AfterActionModel& model_;
    gui::ListDisplayer< AfterActionFunctionList >* functions_;
    Q3GroupBox* parameters_;
    Q3VGroupBox* timeGroup_;
    QSpinBox* firstTick_;
    QSpinBox* duration_;
    QPushButton* request_;
    T_Parameters paramList_;
    QString title_;
    //@}
};

#endif // __AfterActionFunctionList_h_
