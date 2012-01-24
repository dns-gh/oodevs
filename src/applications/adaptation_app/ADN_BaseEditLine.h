// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_BaseEditLine_h_
#define __ADN_BaseEditLine_h_

#include <boost/noncopyable.hpp>
#include <QtGui/QLineEdit.h>

// $$$$ ABR 2012-01-19: Should be in client_gui, as well as all generic component

// =============================================================================
/** @class  ADN_SideWidget
    @brief  ADN_SideWidget
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_SideWidget : public QWidget
{
    Q_OBJECT

public:
    ADN_SideWidget( QWidget *parent = 0 );

signals:
    void sizeHintChanged();

protected:
    bool event( QEvent *event );
};


// =============================================================================
/** @class  ADN_BaseEditLine
    @brief  ADN_BaseEditLine
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_BaseEditLine : public QLineEdit
                       , private boost::noncopyable
{
    Q_OBJECT
    Q_PROPERTY( QString InactiveText READ InactiveText WRITE SetInactiveText )

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
    explicit ADN_BaseEditLine( QWidget* parent = 0, const char* name = 0 );
    explicit ADN_BaseEditLine( const QString& contents, QWidget* parent = 0, const char* name = 0 );
    virtual ~ADN_BaseEditLine();
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
    virtual void resizeEvent( QResizeEvent *event );
    bool event( QEvent* event );
    //@}

protected slots:
    //! @name Slots
    //@{
    void UpdateTextMargins();
    //@}

private:
    //! @name Helpers
    //@{
    void Init();
    void UpdateSideWidgetLocations();
    //@}

private:
    //! @name Member data
    //@{
    ADN_SideWidget* leftWidget_;
    ADN_SideWidget* rightWidget_;
    QHBoxLayout*    leftLayout_;
    QHBoxLayout*    rightLayout_;
    QString         inactiveText_;
    //@}
};

#endif // __ADN_BaseEditLine_h_
