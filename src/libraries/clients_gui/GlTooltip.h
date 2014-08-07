// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlTooltip_h_
#define __GlTooltip_h_

#include "NoLinkDisplayer.h"
#include "GlTooltip_ABC.h"
#include "clients_kernel/Styles.h"

namespace gui
{
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GlTooltip
    @brief  GlTooltip
*/
// Created: AGE 2006-06-29
// =============================================================================
class GlTooltip : public GlTooltip_ABC
                , public NoLinkDisplayer
                , public tools::Caller< QColor >
                , public tools::Caller< QFont >
                , public tools::Caller< kernel::Styles::Style >
                , public tools::Caller< E_OperationalStatus >
                , public tools::Caller< E_ForceRatioStatus >
                , public tools::Caller< E_MeetingEngagementStatus >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GlTooltip( TooltipsLayer_ABC& layer );
    virtual ~GlTooltip();
    //@}

    //! @name Operations
    //@{
    virtual operator kernel::Displayer_ABC& ();
    virtual void Draw( const geometry::Point2f& position, int width = 0, int height = 0, float factor = 1.f );
    virtual void Hide();
    //@}

public:
    //! @name Operations
    //@{
    typedef std::function< void ( QPainter&, const QRect& ) > FrameDrawer;
    void SetFrameDrawer( const FrameDrawer& frameDrawer );
    QSize Size() const;
    //@}

private:
    //! @name Helpers
    //@{
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

    void GenerateImage( unsigned int width, unsigned int height );
    QPixmap CreatePixmap( unsigned int width, unsigned int height );
    void RestoreAlpha();
    void DirtyImage();
    //@}

    //! @name Types
    //@{
    typedef std::pair< QColor, QFont >    T_Style;
    typedef std::pair< QString, T_Style > T_Message;
    typedef std::vector< T_Message >      T_Messages;
    typedef T_Messages::const_iterator  CIT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    TooltipsLayer_ABC& layer_;
    QString currentItem_;
    QColor color_;
    QFont font_;
    QString message_;
    T_Messages current_, new_;
    QImage image_;
    FrameDrawer frameDrawer_;
    //@}
};

}

#endif // __GlTooltip_h_
