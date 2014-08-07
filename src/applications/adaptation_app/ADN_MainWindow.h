// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MainWindow_h_
#define __ADN_MainWindow_h_

#include <map>

namespace gui
{
    class ConsistencyDialog_ABC;
    class GlContext;
}

enum E_WorkspaceElements;

class ADN_Config;
class ADN_GeneralConfig;
class ADN_ListView;
class ADN_MainTabWidget;
class ADN_ProgressBar;
class ADN_Table;

// =============================================================================
/** @class  ADN_MainWindow
    @brief  ADN_MainWindow
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    typedef std::function< QWidget*() > T_ConsistencyCallBack;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_MainWindow( const ADN_GeneralConfig& config );
    virtual ~ADN_MainWindow();
    //@}

    //! @name Operations
    //@{
    void AddPage( E_WorkspaceElements element, QWidget& page, const QString& title );
    void AddTable( const QString& strTableName, const T_ConsistencyCallBack& callback );
    void AddListView( const QString& strTableName, const T_ConsistencyCallBack& callback );
    void RemovePage( E_WorkspaceElements target );

    bool IsLoaded() const;
    void SetIsLoading( bool );
    void LoadStatusChanged( bool );
    ADN_ProgressBar& GetProgressBar() const;
    //@}

protected:
    //! @name QMainWindow override
    //@{
    virtual void closeEvent( QCloseEvent* e );
    virtual void mousePressEvent( QMouseEvent * event );
    virtual QMenu* createPopupMenu();
    //@}

signals:
    //! @name Signals
    //@{
    void ChangeTab( E_WorkspaceElements );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnNew();
    void OnOpen();
    bool OnClose();
    void OnSave();
    void OnSaveAs();
    void OnExportHtml();
    void OnAbout();
    void OnShowConsistencyTable( const QString& name ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void BuildGUI();
    void PurgeGUI();
    void UpdateTitle();
    //@}

private:
    //! @name Private types
    //@{
    typedef std::map< QString, T_ConsistencyCallBack >  T_ConsistencyViews;
    //@}

private:
    //! @name Member data
    //@{
    const ADN_GeneralConfig& config_;
    std::unique_ptr< gui::GlContext > openGLContext_;

    std::unique_ptr< gui::ConsistencyDialog_ABC > consistencyDialog_;
    std::unique_ptr< ADN_MainTabWidget > mainTabWidget_;
    std::unique_ptr< ADN_ProgressBar > progressBar_;
    std::unique_ptr< QSignalMapper > consistencyMapper_;

    QLayout* mainLayout_;

    QAction* actionClose_;
    QAction* actionSave_;
    QAction* actionSaveAs_;
    QAction* actionOptional_;
    QAction* actionBack_;
    QAction* actionForward_;
    QAction* actionExportHtml_;
    QAction* actionConsistencyAnalysis_;

    QMenu* menuConsistencyTables_;
    QMenu* menuLanguages_;

    QToolBar* toolBar_;

    T_ConsistencyViews consistencyTables_;
    T_ConsistencyViews consistencyLists_;

    bool isLoaded_;
    //@}
};

#endif // __ADN_MainWindow_h_
