// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DATA_WIDGET_H_
#define DATA_WIDGET_H_

#include <boost/noncopyable.hpp>

class Config;

// =============================================================================
/** @class  DataWidget
    @brief  Data page
*/
// Created: LGY 2012-02-28
// =============================================================================
class DataWidget : public QObject
                 , public boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DataWidget( QWidget* parent, QTabWidget* tabs, const Config& config );
    virtual ~DataWidget();
    //@}

signals:
    //! @name public
    //@{
    void ButtonChanged( bool enable, const QString& text );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnButtonChanged();
    //@}

public:
    //! @name Methods
    //@{
    void Update();
    void OnDelete();
    //@}

private:
    //! @name Helpers
    //@{
    QListWidgetItem* CurrentItem() const;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    const Config& config_;
    QTabWidget* mainTabs_;
    QListWidget* terrains_;
    QListWidget* models_;
    //@}
};

#endif // DATA_WIDGET_H_
