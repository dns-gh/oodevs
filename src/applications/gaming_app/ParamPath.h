// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPath_h_
#define __ParamPath_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "LocationSerializer.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class Location_ABC;
    class Positions;
    struct Nothing;
    class OrderParameter;
}

namespace gui
{
    class ParametersLayer;
    class RichLabel;
}

// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public QObject, public Param_ABC
                , public kernel::ContextMenuObserver_ABC< kernel::Nothing >
                , private gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( QObject* parent, const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent );
             ParamPath( QObject* parent, const QString& name, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent, bool optional );
    virtual ~ParamPath();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void Show();
    virtual void Hide();
    virtual void NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& );
    virtual void Handle( kernel::Location_ABC& location );
    void CommitTo( ASN1T_Itineraire& destination ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void StartPath();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPath( const ParamPath& );
    ParamPath& operator=( const ParamPath& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter* parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    gui::ParametersLayer& layer_;
    const kernel::Positions& positions_;
    gui::RichLabel*       pLabel_;
    QLabel*               pPosLabel_;
    kernel::Location_ABC* location_;
    bool optional_;
    //@}
};

#endif // __ParamPath_h_
