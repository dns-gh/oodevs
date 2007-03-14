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

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class RichLabel;
}

// =============================================================================
/** @class  ParamPoint
    @brief  ParamPoint
*/
// Created: APE 2004-03-18
// =============================================================================
class ParamPoint : public QObject, public Param_ABC
                 , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPoint( QObject* parent, ASN1T_Point*& asn, const QString& name, const QString& menu, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void BuildInterface( QWidget* parent );
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
    ASN1T_Point*       asn_;
    const kernel::CoordinateConverter_ABC& converter_;

    QString         menu_;
    gui::RichLabel* pLabel_;
    QLabel*         pPosLabel_;

    ASN1T_CoordUTM    asnPoint_;
    geometry::Point2f popupPoint_;
    geometry::Point2f paramPoint_;
    //@}
};

#endif // __ParamPoint_h_
