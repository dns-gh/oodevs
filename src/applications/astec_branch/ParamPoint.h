// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPoint.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 8 $
// $Workfile: ParamPoint.h $
//
// *****************************************************************************

#ifndef __ParamPoint_h_
#define __ParamPoint_h_

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ContextMenuObserver_ABC.h"

class CoordinateConverter;
class RichLabel;

// =============================================================================
/** @class  ParamPoint
    @brief  ParamPoint
*/
// Created: APE 2004-03-18
// =============================================================================
class ParamPoint : public QHBox, public Param_ABC
                 , public ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPoint( QWidget* pParent, ASN1T_Point& asn, const std::string label, const std::string menu, const CoordinateConverter& converter );
    virtual ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, QPopupMenu& );
    //@}

private slots:
    //! @name Operations
    //@{
    void AcceptPopupMenuPoint();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPoint( const ParamPoint& );
    ParamPoint& operator=( const ParamPoint& );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_Point&       asn_;
    const CoordinateConverter& converter_;

    std::string        menu_;
    RichLabel*         pLabel_;
    QLabel*            pPosLabel_;

    ASN1T_CoordUTM    asnPoint_;
    geometry::Point2f popupPoint_;
    //@}
};

#endif // __ParamPoint_h_
