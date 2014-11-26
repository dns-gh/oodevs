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

#include "tools/Observer_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class ActionController;
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class Event;
}

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
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< gui::Event >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineToolBar( kernel::Controllers& controllers,
                              const tools::ExerciseConfig& config,
                              const std::string& gamingUuid );
    virtual ~TimelineToolBar();
    //@}

    //! @name Filters
    //@{
    const std::string& GetEntityFilter() const;
    std::string GetServicesFilter() const;
    bool GetEngagedFilter() const;
    bool GetSelectedFilter() const;
    void SetFilteredEntity( const kernel::Entity_ABC* entity );
    const kernel::Entity_ABC* GetFilteredEntity() const;
    void SetEntityFilter( const std::string& filter );
    std::string GetKeywordFilter() const;
    const std::string& GetShowOnlyFilter() const;
    std::string GetHideHierarchiesFilter() const;
    //@}

public:
    //! @name Copy constructor
    //@{
    TimelineToolBar( const TimelineToolBar& other );
    //@}

signals:
    //! @name Signals
    //@{
    void ToggleLayoutOrientation();

    void AddView();
    void CenterView();
    void RemoveCurrentView();

    void LoadOrderFileRequest( const tools::Path& path );
    void SaveOrderFileRequest( const tools::Path& path );

    void LoadTimelineSessionFileRequest( const tools::Path& path );
    void SaveTimelineSessionFileRequest( const tools::Path& path );

    void EngagedFilterToggled( bool checked );
    void ReadOnlyFilterToggled( bool checked );
    void ServicesFilterChanged( const std::string& services );
    void KeywordFilterChanged( const std::string& keyword );
    void HideHierarchiesFilterChanged( const std::string& hierarchies );
    void ShowOnlyFilterChanged( const std::string& uuid, const std::string& name );
    void SelectedFilterChanged();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLoadOrderFile();
    void OnSaveOrderFile();
    void OnEngagedFilterToggled( bool toggled );
    void OnReadOnlyFilterToggled( bool toggled );
    void OnOrderFilterToggled( bool toggled );
    void OnTaskFilterToggled( bool toggled );
    void OnMarkerFilterToggled( bool toggled );
    void OnSelectedFilterToggled( bool toggled );
    void OnFilterKeyword( const QString& keyword );
    void OnAddShowOnlyFilter();
    void OnHideChildren();
    void OnShowChildren();
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    virtual void NotifyContextMenu( const gui::Event& event, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& eventActionsController_;
    const bool main_;
    const tools::ExerciseConfig& config_;
    const std::string gamingUuid_;
    QAction* horizontalView_;
    std::string entityFilter_;
    std::string keywordFilter_;
    QString filters_;
    bool displayEngaged_;
    bool displayReadOnly_;
    bool displayOrders_;
    bool displayTasks_;
    bool displayMarkers_;
    bool displaySelected_;
    QMenu* filterMenu_;
    QAction* engagedFilter_;
    kernel::SafePointer< gui::Event > contextMenuEvent_;
    kernel::SafePointer< kernel::Entity_ABC > filteredEntity_;
    std::string showOnlyFilter_;
    std::set< std::string > hideHierarchiesFilter_;
    //@}
};

#endif // __TimelineToolBar_h_
