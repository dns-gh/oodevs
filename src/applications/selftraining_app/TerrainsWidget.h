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
#include <boost/optional/optional_fwd.hpp>
#include <boost/shared_ptr.hpp>

class Application;
class Config;
class LogProgressPage;
class Page_ABC;
class QTableView;

namespace graphics
{
    class MapnikProcess_ABC;
}

class TerrainsWidget: public QObject
                    , public boost::noncopyable
{
    Q_OBJECT

public:
             TerrainsWidget( QWidget* parent, const Config& config, Application& app,
                QStackedWidget* pages, Page_ABC& parentPage );
    virtual ~TerrainsWidget();

    QWidget* GetMainWidget();
    void OnButtonChanged();

signals:
    void ButtonChanged( bool enable, const QString& text, bool upgrade );

public slots:
    void OnButtonChanged( const QModelIndex& index );
    void OnUpdate();
    void OnUpgrade();

public:
    void OnDelete();

private:
    boost::optional< tools::Path > GetSelectedTerrain() const;
    void UpdateButtons( bool remove, bool upgrade );

private:
    QWidget* parent_;
    const Config& config_;
    QStandardItemModel* model_;
    QTableView* terrains_;
    LogProgressPage* progress_;
    boost::shared_ptr< graphics::MapnikProcess_ABC > generator_;
};

#endif // SELFTRAINING_TERRAINS_WIDGET_H
