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

#include "ADN_Callback.h"
#include <boost/noncopyable.hpp>
#include <qmainwindow.h>
#include <map>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

class ADN_Workspace;
class ADN_Config;
class ADN_Table;
class ADN_ListView;
class ADN_FileLoaderObserver;

class QWidget;
class QTabWidget;
class QPopupMenu;
class QAction;
class QToolButton;

// =============================================================================
/** @class  ADN_MainWindow
    @brief  ADN_MainWindow
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_MainWindow : public QMainWindow
                     , private boost::noncopyable
{
    Q_OBJECT

public:
    explicit ADN_MainWindow( ADN_Config& config );
    virtual ~ADN_MainWindow();

    void Build();
    void AddPage( const QString& strPageName, QWidget& page );
    void AddTable( const QString& strTableName, ADN_Callback_ABC<ADN_Table*>*    pCallback );
    void AddListView( const QString& strTableName, ADN_Callback_ABC<ADN_ListView*>* pCallback );

    void OpenProject( const std::string& filename, const bool isAdminMode = false );
    void SaveProjectAs( const std::string& filename );

protected:
    void closeEvent( QCloseEvent* e );

signals:
    void OpenModeToggled();

private slots:
    //! @name Slots
    //@{
    void NewProject();
    void OpenProject();
    void SaveProject();
    void SaveAsProject();
    void ExportHtml();
    void CloseProject();
    void TestData();
    void ConfigureDataTest();

    bool SelectOpenMode();

    void About();

    void ChangeSaveState( bool bNoCommand );

    void ShowCoheranceTable( int nId );
    //@}

private:
    void SetMenuEnabled( bool bEnable );
    bool OfferToSave();

private:
    typedef std::pair< QString, ADN_Callback_ABC<ADN_Table*>* > T_TableRegistrationItem;
    typedef std::map< int, T_TableRegistrationItem >            T_TableRegistrationMap;
    typedef T_TableRegistrationMap::iterator                   IT_TableRegistrationMap;

    typedef std::pair< QString, ADN_Callback_ABC<ADN_ListView*>* > T_ListViewRegistrationItem;
    typedef std::map< int, T_ListViewRegistrationItem >            T_ListViewRegistrationMap;
    typedef T_ListViewRegistrationMap::iterator                   IT_ListViewRegistrationMap;

private:
    std::auto_ptr< tools::GeneralConfig > generalConfig_;
    std::auto_ptr< ADN_FileLoaderObserver > fileLoaderObserver_;
    std::auto_ptr< tools::Loader_ABC > fileLoader_;
    const QString strAdminPassword_;
    ADN_Workspace& workspace_;
    ADN_Config& config_;
    QTabWidget* pTab_;
    QAction* pProjectLoadAction_;
    QPopupMenu* pProjectMenu_;
    QPopupMenu* pCoheranceTablesMenu_;
    QPopupMenu* pConfigurationMenu_;
    QPopupMenu* pHelpMenu_;
    QAction* pActionSave_;
    int rIdSaveAs_;
    int nIdSaveTable_;
    int nIdPrint_;
    int nIdChangeOpenMode_;
    bool bNeedSave_;
    T_TableRegistrationMap vTableRegistrations_;
    T_ListViewRegistrationMap vListViewRegistrations_;
};


#endif // __ADN_MainWindow_h_
