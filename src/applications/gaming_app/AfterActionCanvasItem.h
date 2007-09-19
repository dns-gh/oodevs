// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionCanvasItem_h_
#define __AfterActionCanvasItem_h_

#include "gaming/AfterActionItemBuilder_ABC.h"
#include <qcanvas.h>

class AfterActionItem;
class AfterActionCanvasConnection;

// =============================================================================
/** @class  AfterActionCanvasItem
    @brief  AfterActionCanvasItem
*/
// Created: AGE 2007-09-18
// =============================================================================
class AfterActionCanvasItem : public QCanvasRectangle
                            , private AfterActionItemBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionCanvasItem( QCanvas* canvas, const QPalette& palette, const AfterActionItem& item, const QPoint& pos, unsigned id );
    virtual ~AfterActionCanvasItem();
    //@}

    //! @name Operations
    //@{
    AfterActionCanvasConnection* StartConnection( const QPoint& point );
    bool EndConnection( AfterActionCanvasConnection* connection, const QPoint& point );
    void Remove( AfterActionCanvasConnection* connection );

    virtual QRect boundingRect() const;
    virtual void setSelected( bool yes );
    virtual void setVisible( bool yes );
    virtual void moveBy ( double dx, double dy );
    virtual void drawShape( QPainter& p );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionCanvasItem( const AfterActionCanvasItem& );            //!< Copy constructor
    AfterActionCanvasItem& operator=( const AfterActionCanvasItem& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Start( const std::string& name );
    virtual void AddInput( const std::string& type );
    virtual void AddOutput( const std::string& type );
    virtual void AddParameter( const std::string& type, const std::string& name );

    AfterActionCanvasConnection* StartConnection( const QCanvasItem& item, const QPoint& point, int index );
    bool EndConnection( AfterActionCanvasConnection* connection, const QCanvasItem& item, const QPoint& point, int index );
    void Polish( QCanvasItem* subItem, double x, double y );
    void AdjustInputs();
    //@}

    //! @name Types
    //@{
    typedef std::vector< QCanvasItem* >  T_Items;
    typedef T_Items::iterator           IT_Items;
    typedef std::vector< AfterActionCanvasConnection* >  T_Connections;
    typedef T_Connections::iterator                     IT_Connections;
    //@}

private:
    //! @name Member data
    //@{
    const QPalette& palette_;
    const unsigned id_;
    T_Items subItems_;
    T_Items inputs_;
    T_Items outputs_;
    T_Connections connections_;
    //@}
};

#endif // __AfterActionCanvasItem_h_
