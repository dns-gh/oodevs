// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationCreator_h_
#define __LocationCreator_h_

#include "ASN_Types.h"
#include "ContextMenuObserver_ABC.h"

class ParametersLayer;
class GlTools_ABC;
class ShapeHandler_ABC;

// =============================================================================
/** @class  LocationCreator
    @brief  LocationCreator
*/
// Created: APE 2004-05-06
// =============================================================================
class LocationCreator : public QObject
                      , public Observer_ABC
                      , public ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LocationCreator( QWidget* parent, const std::string& menu, ParametersLayer& layer, ShapeHandler_ABC& handler );
             LocationCreator( QWidget* parent, ParametersLayer& layer, ShapeHandler_ABC& handler );
    virtual ~LocationCreator();
    //@}

    //! @name Operations
    //@{
    void AddLocationType( const QString& message, ASN1T_EnumTypeLocalisation type );
    void Draw( const T_PointVector& points, ASN1T_EnumTypeLocalisation type, const GlTools_ABC& tools );
    virtual void NotifyContextMenu( const geometry::Point2f&, QPopupMenu& );
    void StartPoint();
    //@}

    //! @name Accessors
    //@{
    ASN1T_EnumTypeLocalisation GetCurrentType() const; // $$$$ AGE 2006-04-03: hié
    //@}

private slots:
    //! @name Slots
    //@{
    void Start( int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationCreator( const LocationCreator& );
    LocationCreator& operator=( const LocationCreator& );
    //@}

private:
    //! @name Member data
    //@{
    ParametersLayer& layer_;
    ShapeHandler_ABC& handler_;
    
    std::string menu_;
    QPopupMenu* pPopupMenu_;

    geometry::Point2f popupPoint_;
    ASN1T_EnumTypeLocalisation type_;
    //@}
};

#endif // __LocationCreator_h_
