// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderBrowser_h_
#define __OrderBrowser_h_

// =============================================================================
/** @class  OrderBrowser
    @brief  OrderBrowser
*/
// Created: ZEBRE 2007-05-11
// =============================================================================
class OrderBrowser : public QDockWindow
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OrderBrowser( QMainWindow* parent, const QString& directory );
    virtual ~OrderBrowser();
    //@}

public slots:
    //! @name Slots
    //@{
    void Save();
    void Load();
    //@}

signals:
    //! @name Signals
    //@{
    void OrderOpened( const QString& file );
    void OrderSaved ( const QString& file );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    void OnSelectionChanged( QListViewItem* item );
    void OnDoubleClicked   ( QListViewItem* item );
    void OnTextChanged( const QString& text );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderBrowser( const OrderBrowser& );            //!< Copy constructor
    OrderBrowser& operator=( const OrderBrowser& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void RefreshFileList();
    void EmitAndClose( const QString& file );
    QString Translate( const QString& base );
    //@}

private:
    //! @name Member data
    //@{
    QString      directory_;
    QListView*   files_;
    QComboBox*   fileLabel_;
    QPushButton* ok_;
    QPixmap      pixmap_;

    bool saving_;
    //@}
};

#endif // __OrderBrowser_h_
