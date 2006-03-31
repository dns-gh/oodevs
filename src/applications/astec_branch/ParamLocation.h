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
#include "ContextMenuObserver_ABC.h"
#include "ShapeHandler_ABC.h"

class ParametersLayer;
class CoordinateConverter;
class MT_ParameterLabel;

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public QHBox, public Param_ABC
                    , public ContextMenuObserver_ABC< geometry::Point2f >
                    , private ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( QWidget* pParent, ASN1T_Localisation& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter& converter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, QPopupMenu& );
    virtual void Handle( const T_PointVector& points );
    //@}

private slots:
    //! @name Slots
    //@{
    void Start( int );
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
    ParametersLayer& layer_;
    const CoordinateConverter& converter_;
    ASN1T_Localisation& asn_;
    
    std::string menu_;
    MT_ParameterLabel* pLabel_;
    QLabel* pShapeLabel_;
    QPopupMenu* pPopupMenu_;

    geometry::Point2f popupPoint_;
    T_PointVector points_;
    ASN1T_CoordUTM* pUMTCoords_;
    ASN1T_EnumTypeLocalisation nType_;
    //@}
};

#endif // __ParamLocation_h_
