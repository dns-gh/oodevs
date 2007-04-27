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

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "EntityParameter.h"

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

class ParamLocation;
class ParamNumericField;
class ActionParameter;

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
             ParamObstacle( QObject* parent, const QString& name, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, bool optional );
    virtual ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;

    void CommitTo( ASN1T_MissionGenObject& object ) const;
    void CommitTo( ActionParameter_ABC& parameter ) const;
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

private:
    //! @name Member data
    //@{
    const kernel::ObjectTypes& objectTypes_;
    const kernel::OrderParameter* parameter_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    ParamLocation* location_;

    gui::ValuedComboBox< const kernel::ObjectType* >* typeCombo_;
    QComboBox* preliminaryCombo_;
    ParamNumericField* density_;
    EntityParameter< kernel::Automat_ABC >* tc2_;
    bool optional_;
    //@}
};

#endif // __ParamObstacle_h_
