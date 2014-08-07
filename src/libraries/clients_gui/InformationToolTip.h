// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __gui_InformationToolTip_h_
#define __gui_InformationToolTip_h_

#include "NoLinkDisplayer.h"
#include "clients_kernel/Styles.h"

namespace gui
{

// =============================================================================
/** @class  InformationToolTip
    @brief  InformationToolTip
*/
// Created: NPT 2013-02-11
// =============================================================================
class InformationToolTip : public QDialog
                         , public NoLinkDisplayer
                         , public tools::Caller< QColor >
                         , public tools::Caller< QFont >
                         , public tools::Caller< kernel::Styles::Style >
                         , public tools::Caller< E_OperationalStatus >
                         , public tools::Caller< E_ForceRatioStatus >
                         , public tools::Caller< E_MeetingEngagementStatus >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InformationToolTip();
    virtual ~InformationToolTip();
    //@}

    //! @name Operations
    //@{
    void Draw();
    virtual operator kernel::Displayer_ABC& ();
    typedef std::function< void ( QPainter&, const QRect& ) > FrameDrawer;
    void SetFrameDrawer( const FrameDrawer& frameDrawer );
    QSize Size() const;
    virtual void paintEvent( QPaintEvent * event );
    virtual void Hide();
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

    void GenerateImage();
    QPixmap CreatePixmap();
    void RestoreAlpha();
    void DirtyImage();
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< QColor, QFont >    T_Style;
    typedef std::pair< QString, T_Style > T_Message;
    typedef std::vector< T_Message >      T_Messages;
    typedef T_Messages::const_iterator  CIT_Messages;
    //@}

    //! @name Member data
    //@{
    QString currentItem_;
    QColor color_;
    QFont font_;
    QString message_;
    T_Messages current_;
    T_Messages new_;
    QImage image_;
    FrameDrawer frameDrawer_;
    //@}
};

}
#endif // __gui_InformationToolTip_h_
