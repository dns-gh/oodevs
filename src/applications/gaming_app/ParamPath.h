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

#include "gaming/ASN_Types.h"
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
class ParamPath : public QHBox, public Param_ABC
                , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                , private gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( QWidget* pParent, ASN1T_Itineraire& asn, const std::string label, const std::string menu, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamPath();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const kernel::GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void Handle( kernel::Location_ABC& location );
    void CommitTo( ASN1T_Itineraire& destination );
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
    const kernel::CoordinateConverter_ABC& converter_;
    gui::ParametersLayer& layer_;
    const kernel::Positions& positions_;
    LocationSerializer serializer_;

    std::string menu_;
    gui::RichLabel* pLabel_;
    QLabel*       pPosLabel_;
    kernel::Location_ABC* location_;
    //@}
};

#endif // __ParamPath_h_
