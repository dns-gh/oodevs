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

#include "actions_gui/ParamInterface_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace gui
{
    class RichDateTimeEdit;
}

namespace kernel
{
    class Controllers;
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
class AfterActionModel;
class Simulation;

// =============================================================================
/** @class  AfterActionFunctionList
    @brief  AfterActionFunctionList
*/
// Created: AGE 2007-09-21
// =============================================================================
class AfterActionFunctionList : public QWidget
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
    void ActivateParameters( bool activate );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( int index );
    void OnDateChanged();
    void Request();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    void CreateParameter( const AfterActionParameter& parameter );
    boost::shared_ptr< actions::gui::Param_ABC > CreateParameter( const std::string& type, const QString& name );
    void ClearParameters();
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
    QLineEdit* name_;
    QComboBox* functions_;
    QLabel* description_;
    QGroupBox* timeGroup_;
    gui::RichDateTimeEdit* startDate_;
    gui::RichDateTimeEdit* endDate_;
    QGroupBox* parameters_;
    QPushButton* request_;
    T_Parameters paramList_;
    QDateTime startTime_;
    QDateTime endTime_;
    bool firstUpdate_;
    //@}
};

#endif // __AfterActionFunctionList_h_
