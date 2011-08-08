#ifndef QT_TRAYICON_H
#define QT_TRAYICON_H

#ifndef QT_H

#endif // QT_H

class TrayIcon : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString toolTip READ toolTip WRITE setToolTip )
    Q_PROPERTY( QPixmap icon READ icon WRITE setIcon )

public:
    TrayIcon( QObject *parent = 0, const char *name = 0 );
    TrayIcon( const QPixmap &, const QString &, Q3PopupMenu *popup = 0, QObject *parent = 0, const char *name = 0 );
    virtual ~TrayIcon();

    // Set a popup menu to handle RMB
    void        setPopup( Q3PopupMenu * );
    Q3PopupMenu*        popup() const;

    QPixmap        icon() const;
    QString        toolTip() const;

public slots:
    void        setIcon( const QPixmap &icon );
    void        setToolTip( const QString &tip );

    void        show();
    void        hide();

signals:
    void        clicked( const QPoint& );
    void        doubleClicked( const QPoint& );

protected:
    bool        event( QEvent * );
    virtual void    mouseMoveEvent( QMouseEvent *e );
    virtual void    mousePressEvent( QMouseEvent *e );
    virtual void    mouseReleaseEvent( QMouseEvent *e );
    virtual void    mouseDoubleClickEvent( QMouseEvent *e );

private:
    Q3PopupMenu *pop;
    QPixmap pm;
    QString tip;

    // system-dependent part
    class TrayIconPrivate;
    TrayIconPrivate *d;
    void sysInstall();
    void sysRemove();
    void sysUpdateIcon();
    void sysUpdateToolTip();
};

#endif //TRAYICON_H
