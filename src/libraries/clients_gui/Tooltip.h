// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Tooltip_h_
#define __Tooltip_h_

#include "NoLinkDisplayer.h"
#include "clients_kernel/Styles.h"

namespace gui
{
// =============================================================================
/** @class  Tooltip
    @brief  Tooltip
*/
// Created: AGE 2006-06-29
// =============================================================================
class Tooltip : public NoLinkDisplayer
              , public tools::Caller< QColor >
              , public tools::Caller< QFont >
              , public tools::Caller< kernel::Styles::Style >
              , public tools::Caller< E_OperationalStatus >
              , public tools::Caller< E_ForceRatioStatus >
              , public tools::Caller< E_MeetingEngagementStatus >
{
public:
             Tooltip();
    virtual ~Tooltip();

    typedef std::function< void ( QPainter&, const QRect& ) > FrameDrawer;
    void SetFrameDrawer( const FrameDrawer& frameDrawer );

protected:
    QImage GenerateImage( unsigned int width, unsigned int height );

private:
    virtual void Call( const QColor& value );
    virtual void Call( const QFont& font );
    virtual void Call( const kernel::Styles::Style& value );
    virtual void Call( const E_OperationalStatus& value );
    virtual void Call( const E_ForceRatioStatus& value );
    virtual void Call( const E_MeetingEngagementStatus& value );

    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    QPixmap CreatePixmap( unsigned int width, unsigned int height ) const;
    void RestoreAlpha( QImage& image ) const;
    virtual void DirtyImage() = 0;

private:
    typedef std::pair< QColor, QFont >    T_Style;
    typedef std::pair< QString, T_Style > T_Message;
    typedef std::vector< T_Message >      T_Messages;

private:
    QString currentItem_;
    QColor color_;
    QFont font_;
    QString message_;
    T_Messages current_, new_;
    FrameDrawer frameDrawer_;
};

}

#endif // __Tooltip_h_
