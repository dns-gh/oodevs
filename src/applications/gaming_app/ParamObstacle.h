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
class ParamAgent;

// =============================================================================
/** @class  ParamObstacle
    @brief  ParamObstacle
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacle : public QGroupBox
                    , public Param_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacle( QWidget* parent, ASN1T_MissionGenObject*& asnObject, const QString& label, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual bool CheckValidity();
    virtual void Commit();
    void CommitTo( ASN1T_MissionGenObject& destination );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
    ASN1T_MissionGenObject* asnObject_;
    ParamLocation* location_;

    gui::ValuedComboBox< const kernel::ObjectType* >* typeCombo_;
    QComboBox* preliminaryCombo_;
    ParamNumericField* density_;
    EntityParameter< kernel::Automat_ABC >* tc2_;
    //@}
};

#endif // __ParamObstacle_h_
