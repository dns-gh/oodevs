// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_GUI_ABC_h_
#define __ADN_GUI_ABC_h_

#include "ADN_NavigationInfos.h"
#include "ADN_Enums.h"

class ADN_MainWindow;
class ADN_HtmlBuilder;
class ADN_ListView;

class ADN_BaseGui_ABC : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_BaseGui_ABC() {}
    virtual ~ADN_BaseGui_ABC() {}
    //@}

signals:
    //! @name Signals
    //@{
    void ApplyFilterList( const ADN_NavigationInfos::UsedBy& );
    //@}
};

struct OptionalTab
{
    OptionalTab( QTabWidget* tab, int index, QWidget* widget )
        : tab_( tab )
        , index_( index )
        , widget_( widget )
    {
        // NOTHING
    }
    OptionalTab()
    {
        // NOTHING
    }
    QTabWidget* tab_;
    int index_;
    QWidget * widget_;
};

// =============================================================================
/** @class  ADN_GUI_ABC
    @brief  ADN_GUI_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_GUI_ABC : public ADN_BaseGui_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_GUI_ABC( E_WorkspaceElements workspaceElement );
    virtual ~ADN_GUI_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Build() = 0;
    virtual void RegisterTable( ADN_MainWindow& /*mainWindow*/ ) {}
    virtual void ExportHtml( ADN_HtmlBuilder& /*mainIndexBuilder*/, const tools::Path& /*path*/ ) {}
    virtual void ChangeCurrentSubTab( int ) {}
    virtual void FindSubTabAndSelectItem( const QString& /*name*/ ) {}
    virtual bool SelectItem( const QString& name );
    virtual void UpdateOptionalFieldsVisibility( bool hide );

    void ApplyFilter( const ADN_NavigationInfos::UsedBy& usedByInfos );
    void ClearFilters();

    QWidget* GetMainWidget() const { return pMainWidget_; }
    ADN_ListView* GetListView() const { return pListView_; }
    virtual void AddListView( ADN_ListView* listView );
    QWidget* CreateScrollArea( const char* objectName, QWidget& content, QWidget* list = 0, bool paintSplitter = true, bool paintBackground = false, bool showFrameBorder = true, int margin = 10, int spacing = 10 );
    void DisconnectListView();
    //@}

protected:
    //! @name Member data
    //@{
    const QString strClassName_;
    QWidget* pMainWidget_;
    ADN_ListView* pListView_;
    std::vector< QWidget* > optionalWidgets_;
    std::map< QString, OptionalTab > optionalTabs_;
    //@}
};

// =============================================================================
/** @class  ADN_Tabbed_GUI_ABC
    @brief  ADN_Tabbed_GUI_ABC
*/
// Created: ABR 2012-03-05
// =============================================================================
class ADN_Tabbed_GUI_ABC : public ADN_GUI_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Tabbed_GUI_ABC( E_WorkspaceElements workspaceElement )
                 : ADN_GUI_ABC( workspaceElement )
                 , pTabWidget_( 0 ) {}
    virtual ~ADN_Tabbed_GUI_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void ChangeCurrentSubTab( int subTab );
    virtual bool SelectItem( const QString& name );
    virtual void FindSubTabAndSelectItem( const QString& name );
    virtual void AddListView( ADN_ListView* listView );
    //@}

protected:
    //! @name Member data
    //@{
    QTabWidget*                   pTabWidget_;
    std::vector< ADN_ListView* >  vListViews_;
    //@}
};

#endif // __ADN_GUI_ABC_h_
