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
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class ObjectType;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class Automat_ABC;
    class Controller;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {
        class ParamLocation;
        class ParamNumericField;
        class ParamAutomat;

// =============================================================================
/** @class  ParamObstacle
    @brief  ParamObstacle
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacle : public QObject
                    , public Param_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacle( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller );
    virtual ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool InternalCheckValidity() const;

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
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
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::ObjectTypes& objectTypes_;
    ::gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    ::gui::ValuedComboBox< const kernel::ObjectType* >* typeCombo_;
    ::gui::ValuedComboBox< unsigned int >* obstacleTypeCombo_;
    ParamLocation* location_;
    ParamNumericField* density_;
    ParamAutomat* tc2_;
    //@}
};

    }
}

#endif // __ParamObstacle_h_
