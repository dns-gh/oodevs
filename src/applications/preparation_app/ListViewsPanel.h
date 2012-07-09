// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ListViewsPanel_h
#define ListViewsPanel_h

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class HierarchyListView_ABC;
    class ItemFactory_ABC;
    class EntitySymbols;
    class SymbolIcons;
    class SearchListView_ABC;
    class AggregateToolbar;
}

class ModelBuilder;
class Model;
class StaticModel;

// =============================================================================
/** @class  ListViewsPanel
    @brief  List views panel
*/
// Created: LGY 2012-06-26
// =============================================================================
class ListViewsPanel : public QTabWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ListViewsPanel( kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons,
                             ModelBuilder& modelBuilder, Model& model, std::vector< gui::SearchListView_ABC* >& listViews,
                             gui::SymbolIcons& symbols, StaticModel& staticModel, const gui::AggregateToolbar& aggregateToolbar );
    virtual ~ListViewsPanel();
    //@}

public:
    //! @name Operations
    //@{
    void SetVisible( bool value );
    //@}

public slots:
    void FocusIn( gui::HierarchyListView_ABC* );

private:
    //! @name Helpers
    //@{
    void CreateUnitTabWidget( QTabWidget* parent, QTabWidget* tabWidget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                              gui::EntitySymbols& icons, ModelBuilder& modelBuilder, Model& model, StaticModel& staticModel,
                              std::vector< gui::SearchListView_ABC* >& listViews, const gui::AggregateToolbar& aggregateToolbar, bool first );
    void Configure( gui::SearchListView_ABC* searchListView, std::vector< gui::SearchListView_ABC* >& listViews,
                    const gui::AggregateToolbar& aggregateToolbar, int readOnlyMode = -1 );
    //@}

private:
    //! @name Member Data
    //@{
    QTabWidget* pSecondAgentsTabWidget_;
    std::vector< gui::HierarchyListView_ABC* > firstUnitViews_;
    std::vector< gui::HierarchyListView_ABC* > secondUnitViews_;
    //@}
};

#endif // ListViewsPanel_h
