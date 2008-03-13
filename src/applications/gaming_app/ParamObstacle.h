// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObstacle_h_
#define __ParamObstacle_h_

#include "Param_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class ObjectType;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class Automat_ABC;
}

namespace gui
{
    class ParametersLayer;
}

class ActionParameter_ABC;
class ParamLocation;
class ParamNumericField;
class ParamAutomat;

// =============================================================================
/** @class  ParamObstacle
    @brief  ParamObstacle
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacle : public QObject, public Param_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacle( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();

    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void ToggleReservable( bool );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    //@}

private:
    //! @name Copy
    //@{
    ParamObstacle( const ParamObstacle& );
    ParamObstacle& operator=( const ParamObstacle& );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::ObjectTypes& objectTypes_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    gui::ValuedComboBox< const kernel::ObjectType* >* typeCombo_;
    gui::ValuedComboBox< unsigned int >* obstacleTypeCombo_;
    ParamLocation* location_;
    ParamNumericField* density_;
    ParamAutomat* tc2_;
    bool optional_;
    //@}
};

#endif // __ParamObstacle_h_
