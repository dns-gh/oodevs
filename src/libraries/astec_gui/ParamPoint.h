// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPoint_h_
#define __ParamPoint_h_

#include "astec_gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "astec_kernel/ContextMenuObserver_ABC.h"

class CoordinateConverter_ABC;
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
             ParamPoint( QWidget* pParent, ASN1T_Point& asn, const std::string label, const std::string menu, const CoordinateConverter_ABC& converter );
    virtual ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, ContextMenu& );
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
    const CoordinateConverter_ABC& converter_;

    std::string        menu_;
    RichLabel*         pLabel_;
    QLabel*            pPosLabel_;

    ASN1T_CoordUTM    asnPoint_;
    geometry::Point2f popupPoint_;
    //@}
};

#endif // __ParamPoint_h_
