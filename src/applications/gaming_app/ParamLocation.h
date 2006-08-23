// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "LocationSerializer.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class ParametersLayer;
    class RichLabel;
    class LocationCreator;
}

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public QHBox, public Param_ABC, private gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const QString& label, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const kernel::GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void Handle( kernel::Location_ABC& location );
    void CommitTo( ASN1T_Localisation& destination );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamLocation( const ParamLocation& );
    ParamLocation& operator=( const ParamLocation& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    LocationSerializer serializer_;
    gui::LocationCreator* creator_;
    gui::RichLabel* pLabel_;
    QLabel* pShapeLabel_;
    kernel::Location_ABC* location_;
    kernel::ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    //@}
};

#endif // __ParamLocation_h_
