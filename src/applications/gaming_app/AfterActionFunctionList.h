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

#include "actions_gui/ParamInterface_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>

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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctionList( QWidget* parent, kernel::Controllers& controllers, AfterActionModel& model,
                                      actions::gui::InterfaceBuilder_ABC& interfaceBuilder );
    virtual ~AfterActionFunctionList();
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( actions::gui::Param_ABC* param ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange();
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
    QTreeWidget* functions_;
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
