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

#include "ParamLocationComposite.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ValuedComboBox.h"
#include <tools/ElementObserver_ABC.h>

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
        template< typename T > class ParamNumericField;
        class ParamAutomat;
        class ParamDateTime;
        class ParamStringField;
        class ParamBool;
        class ParamTime;
        class ParamQuantity;
        class ParamFireClass;

// =============================================================================
/** @class  ParamObstacle
    @brief  ParamObstacle
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacle : public ParamLocationComposite
                    , public tools::ElementObserver_ABC< Param_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacle( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn();
    virtual void RemoveFromController();
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool CheckValidity() const;
    virtual bool IsChecked() const;
    virtual bool InternalCheckValidity() const;
    virtual kernel::ContextMenu::T_MenuVariant CreateMenu( kernel::ContextMenu& menu );
    virtual void NotifyChanged( Param_ABC& param );
    virtual bool HasParameter( const Param_ABC& param ) const;

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::EngineerConstruction& param );

    virtual void Visit( const actions::parameters::Automat& param );
    virtual void Visit( const actions::parameters::Bool& param );
    virtual void Visit( const actions::parameters::FireClass& param );
    virtual void Visit( const actions::parameters::Location& param );
    virtual void Visit( const actions::parameters::Numeric& param );
    virtual void Visit( const actions::parameters::ObstacleType& param );
    virtual void Visit( const actions::parameters::Quantity& param );
    virtual void Visit( const actions::parameters::String& param );
    //@}

protected:
    //! @name Helpers
    //@{
    void NotifyUpdated( const Param_ABC& param );
    //@}

signals:
    //! @name Signals
    //@{
    void ToggleReservable( bool );
    //@}

private slots:
    //! @name Slots
    //@{
    // The parameter 'update' indicates if the call to OnTypeChanged needs to
    // inform the controller that this parameter has changed (always true,
    // except during the initialization)
    void OnTypeChanged( bool update = true );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&                                 controller_;
    const kernel::ObjectTypes&                          objectTypes_;
    ::gui::ParametersLayer&                             layer_;
    const kernel::CoordinateConverter_ABC&              converter_;
    ::gui::ValuedComboBox< const kernel::ObjectType* >* typeCombo_;
    QComboBox*                                          activatedCombo_;
    ParamLocation*                                      location_;
    ParamNumericField< float >*                         density_;
    ParamAutomat*                                       tc2_;
    ParamQuantity*                                      activityTime_;
    ParamQuantity*                                      activationTime_;
    ParamStringField*                                   name_;
    ParamQuantity*                                      altitudeModifier_;
    ParamTime*                                          timeLimit_;
    ParamBool*                                          mining_;
    ParamQuantity*                                      lodging_;
    ParamFireClass*                                     fireClass_;
    ParamQuantity*                                      maxCombustionEnergy_;
    //@}
};

    }
}

#endif // __ParamObstacle_h_
