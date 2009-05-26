// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreVariableCreationWizard_h_
#define __ScoreVariableCreationWizard_h_

#include "clients_gui/ValuedComboBox.h"
#include <boost/shared_ptr.hpp>

namespace actions
{
    namespace gui
    {
        class Param_ABC;
    }
}

namespace gui
{
    class ParametersLayer;
}

namespace kernel
{
    class Controllers;
}

namespace indicators
{
    class Element_ABC;
}

class StaticModel;
class QVGroupBox;

// =============================================================================
/** @class  ScoreVariableCreationWizard
    @brief  ScoreVariableCreationWizard
*/
// Created: SBO 2009-04-21
// =============================================================================
class ScoreVariableCreationWizard : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariableCreationWizard( QWidget* parent, kernel::Controllers& controllers, gui::ParametersLayer& layer, const StaticModel& staticModel );
    virtual ~ScoreVariableCreationWizard();
    //@}

    //! @name Operations
    //@{
    void Create();
    //@}

signals:
    //! @name Signals
    //@{
    void VariableCreated( const indicators::Element_ABC& variable );
    void Closed();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnChangeName();
    void OnChangeType();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreVariableCreationWizard( const ScoreVariableCreationWizard& );            //!< Copy constructor
    ScoreVariableCreationWizard& operator=( const ScoreVariableCreationWizard& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< actions::gui::Param_ABC > CreateParameter( const std::string& type, const QString& name );
    virtual void hideEvent( QHideEvent* e );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ParametersLayer& layer_;
    const StaticModel& staticModel_;
    QLineEdit* name_;
    QButton* ok_;
    gui::ValuedComboBox< std::string >* type_;
    QVGroupBox* paramBox_;
    boost::shared_ptr< actions::gui::Param_ABC > parameter_;
    //@}
};

#endif // __ScoreVariableCreationWizard_h_
