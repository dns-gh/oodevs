// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __RichLineEdit_h_
#define __RichLineEdit_h_

namespace gui
{

// =============================================================================
/** @class  SideWidget
    @brief  SideWidget
*/
// Created: ABR 2012-01-19
// =============================================================================
class SideWidget : public QWidget
{
    Q_OBJECT

public:
    SideWidget( QWidget *parent = 0 );

signals:
    void sizeHintChanged();

protected:
    bool event( QEvent *event );
};

// =============================================================================
/** @class  RichLineEdit
    @brief  RichLineEdit
*/
// Created: ABR 2012-01-19
// =============================================================================
class RichLineEdit : public QLineEdit
                   , private boost::noncopyable
{
    Q_OBJECT
    Q_PROPERTY( QString InactiveText READ InactiveText WRITE SetInactiveText ) // $$$$ ABR 2012-01-26: Replace placeholder property to be drawn on the right place.

public:
    //! @name Types
    //@{
    enum WidgetPosition
    {
        LeftSide,
        RightSide
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichLineEdit( const QString& objectName, QWidget* parent = 0 );
    explicit RichLineEdit( const QString& objectName, const QString& contents, QWidget* parent = 0 );
    virtual ~RichLineEdit();
    //@}

    //! @name Operations
    //@{
    void AddWidget( QWidget* widget, WidgetPosition position );
    void RemoveWidget( QWidget* widget );
    void SetWidgetSpacing( int spacing );
    int WidgetSpacing() const;
    int TextMargin( WidgetPosition position ) const;

    QString InactiveText() const;
    void SetInactiveText( const QString& text );
    virtual void paintEvent( QPaintEvent* event );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void resizeEvent( QResizeEvent *event );
    bool event( QEvent* event );
    //@}

public slots:
    //! @name Slots
    //@{
    void SetText( const QString& text );
    void UpdateTextMargins();
    void clear();
    //@}

private:
    //! @name Helpers
    //@{
    void Init( const QString& objectName );
    void UpdateSideWidgetLocations();
    //@}

private:
    //! @name Member data
    //@{
    SideWidget* leftWidget_;
    SideWidget* rightWidget_;
    QHBoxLayout*    leftLayout_;
    QHBoxLayout*    rightLayout_;
    QString         inactiveText_;
    //@}
};

} //! namespace gui

#endif // __RichLineEdit_h_
