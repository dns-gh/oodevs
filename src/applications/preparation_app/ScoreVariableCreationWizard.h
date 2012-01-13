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
#include "actions_gui/ParamInterface_ABC.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace actions
{
    namespace gui
    {
        class Param_ABC;
        class InterfaceBuilder_ABC;
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
                                  , public actions::gui::ParamInterface_ABC
                                  , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariableCreationWizard( QWidget* parent, kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
    virtual ~ScoreVariableCreationWizard();
    //@}

    //! @name Operations
    //@{
    void Create();
    void Draw( kernel::Viewport_ABC& viewport );
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( actions::gui::Param_ABC* param ) const;
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
    const kernel::GlTools_ABC& tools_;
    actions::gui::InterfaceBuilder_ABC& builder_;
    QLineEdit* name_;
    QPushButton* ok_;
    gui::ValuedComboBox< std::string >* type_;
    Q3VGroupBox* paramBox_;
    boost::shared_ptr< actions::gui::Param_ABC > parameter_;
    //@}
};

#endif // __ScoreVariableCreationWizard_h_
