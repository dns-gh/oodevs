// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineToolBar_h_
#define __TimelineToolBar_h_

namespace tools
{
    class ExerciseConfig;
}

// =============================================================================
/** @class  TimelineToolBar
    @brief  TimelineToolBar
*/
// Created: ABR 2013-05-28
// =============================================================================
class TimelineToolBar : public QToolBar
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit TimelineToolBar( const tools::ExerciseConfig& config );
    virtual ~TimelineToolBar();
    //@}

    //! @name Filters
    //@{
    const std::string& GetEntityFilter() const;
    std::string GetServicesFilter() const;
    bool GetEngagedFilter() const;
    void SetEntityFilter( const std::string& filter );
    std::string GetKeywordFilter() const;
    //@}

public:
    //! @name Copy constructor
    //@{
    TimelineToolBar( const TimelineToolBar& other );
    //@}

signals:
    //! @name Signals
    //@{
    void SetLayoutOrientation( bool horizontal );

    void AddView();
    void CenterView();
    void RemoveCurrentView();

    void LoadOrderFileRequest( const tools::Path& path );
    void SaveOrderFileRequest( const tools::Path& path );

    void LoadTimelineSessionFileRequest( const tools::Path& path );
    void SaveTimelineSessionFileRequest( const tools::Path& path );

    void EngagedFilterToggled( bool checked );
    void ServicesFilterChanged( const std::string& services );
    void KeywordFilterChanged( const std::string& keyword );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnSwitchView();
    void OnLoadOrderFile();
    void OnSaveOrderFile();
    void OnEngagedFilterToggled( bool toggled );
    void OnEventFilterToggled( bool toggled );
    void OnTaskFilterToggled( bool toggled );
    void OnFilterKeyword( const QString& keyword );
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    QAction* horizontalView_;
    std::string entityFilter_;
    std::string keywordFilter_;
    QString filters_;
    bool displayEngaged_;
    bool displayEvents_;
    bool displayTasks_;
    bool horizontalMode_;
    QMenu* filterMenu_;
    //@}
};

#endif // __TimelineToolBar_h_
