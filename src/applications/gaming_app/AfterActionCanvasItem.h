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

#include "gaming/AfterActionBuilder_ABC.h"
#include <qcanvas.h>

class AfterActionItem_ABC;
class AfterActionCanvasConnection;
class AfterActionCanvas;

// =============================================================================
/** @class  AfterActionCanvasItem
    @brief  AfterActionCanvasItem
*/
// Created: AGE 2007-09-18
// =============================================================================
class AfterActionCanvasItem : public QCanvasRectangle
                            , private AfterActionBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionCanvasItem( AfterActionCanvas& canvas, AfterActionItem_ABC& item, const QPoint& pos );
    virtual ~AfterActionCanvasItem();
    //@}

    //! @name Operations
    //@{
    void Reconnect();
    AfterActionCanvasConnection* StartConnection( const QPoint& point );
    bool EndConnection( AfterActionCanvasConnection& connection, const QPoint& point );
    void Remove( AfterActionCanvasConnection* connection );
    bool Holds( AfterActionItem_ABC* item ) const;

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
    virtual void Connect( AfterActionItem_ABC* target, int targetSlot );

    void Polish( QCanvasItem* subItem, double x, double y );
    void SpreadInputs();

    bool IsOnOutput( const QPoint& point ) const;
    bool IsOnInput ( int i, const QPoint& point ) const;
    double InputPosition( int i ) const;

    static bool Connect( AfterActionCanvasConnection& connection, AfterActionCanvasItem* from, AfterActionCanvasItem* to, int i, bool alreadyConnected = false );
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
    AfterActionCanvas& canvas_;
    AfterActionItem_ABC& item_;
    T_Items subItems_;
    T_Items inputs_;
    QCanvasItem* output_;
    T_Connections connections_;
    //@}
};

#endif // __AfterActionCanvasItem_h_
