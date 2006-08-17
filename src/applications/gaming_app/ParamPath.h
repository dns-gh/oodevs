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
#include "clients_gui/LocationSerializer.h"

class ParametersLayer;
class CoordinateConverter_ABC;
class RichLabel;
class Entity_ABC;
class Location_ABC;
class Positions;

// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public QHBox, public Param_ABC
                , public ContextMenuObserver_ABC< geometry::Point2f >
                , private ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( QWidget* pParent, ASN1T_Itineraire& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent );
    virtual ~ParamPath();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, ContextMenu& );
    virtual void Handle( Location_ABC& location );
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
    const CoordinateConverter_ABC& converter_;
    ParametersLayer& layer_;
    const Positions& positions_;
    LocationSerializer serializer_;

    std::string menu_;
    RichLabel* pLabel_;
    QLabel*       pPosLabel_;
    Location_ABC* location_;
    //@}
};

#endif // __ParamPath_h_
