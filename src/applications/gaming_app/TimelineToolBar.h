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
             TimelineToolBar( QWidget* parent, const tools::ExerciseConfig& config, bool isMain,
                              const std::string& entityFilter );
    virtual ~TimelineToolBar();
    //@}

    //! @name Filters
    //@{
    const std::string& GetEntityFilter() const;
    void SetEntityFilter( const std::string& filter );
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
    //@}

public slots:
    //! @name Slots
    //@{
    void OnSwitchView();
    void OnFilterSelection();
    void OnLoadOrderFile();
    void OnSaveOrderFile();
    void OnEngagedFilterToggled( bool toggled );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    QAction* horizontalView_;
    std::string entityFilter_;
    QString filters_;
    bool engaged_;
    bool horizontalMode_;
    //@}
};

#endif // __TimelineToolBar_h_
