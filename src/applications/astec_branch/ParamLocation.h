// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLocation.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 8 $
// $Workfile: ParamLocation.h $
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ShapeHandler_ABC.h"

class ParametersLayer;
class CoordinateConverter;
class RichLabel;
class LocationCreator;

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public QHBox, public Param_ABC, private ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter& converter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void Handle( const T_PointVector& points );
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
    ASN1T_Localisation& asn_;
    const CoordinateConverter& converter_;

    LocationCreator* creator_;
    RichLabel* pLabel_;
    QLabel* pShapeLabel_;
    T_PointVector points_;
    ASN1T_CoordUTM* pUMTCoords_;
    ASN1T_EnumTypeLocalisation nType_;
    ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    //@}
};

#endif // __ParamLocation_h_
