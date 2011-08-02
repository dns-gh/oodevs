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
#include <boost/noncopyable.hpp>

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
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace indicators
{
    class Element_ABC;
}

class StaticModel;
class Q3VGroupBox;

// =============================================================================
/** @class  ScoreVariableCreationWizard
    @brief  ScoreVariableCreationWizard
*/
// Created: SBO 2009-04-21
// =============================================================================
class ScoreVariableCreationWizard : public QDialog
                                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariableCreationWizard( QWidget* parent, kernel::Controllers& controllers, gui::ParametersLayer& layer,
                                          const StaticModel& staticModel, const kernel::GlTools_ABC& tools );
    virtual ~ScoreVariableCreationWizard();
    //@}

    //! @name Operations
    //@{
    void Create();
    void Draw( kernel::Viewport_ABC& viewport );
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
    QPushButton* ok_;
    gui::ValuedComboBox< std::string >* type_;
    Q3VGroupBox* paramBox_;
    boost::shared_ptr< actions::gui::Param_ABC > parameter_;
    const kernel::GlTools_ABC& tools_;
    //@}
};

#endif // __ScoreVariableCreationWizard_h_
