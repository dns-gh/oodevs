// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SELFTRAINING_TERRAINS_WIDGET_H
#define SELFTRAINING_TERRAINS_WIDGET_H

#include <boost/noncopyable.hpp>

class Config;

class TerrainsWidget: public QObject
                    , public boost::noncopyable
{
    Q_OBJECT

public:
             TerrainsWidget( QWidget* parent, const Config& config );
    virtual ~TerrainsWidget();

    QWidget* GetMainWidget();

signals:
    void ButtonChanged( bool enable, const QString& text );

public slots:
    void OnButtonChanged();

public:
    void Update();
    void OnDelete();

private:
    QListWidgetItem* CurrentItem() const;

private:
    QWidget* parent_;
    const Config& config_;
    QListWidget* terrains_;
};

#endif // SELFTRAINING_TERRAINS_WIDGET_H
