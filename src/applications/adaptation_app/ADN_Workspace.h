//*****************************************************************************
//
// $Created: JDY 03-06-30 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Workspace.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:40 $
// $Revision: 11 $
// $Workfile: ADN_Workspace.h $
//
//*****************************************************************************

#ifndef __ADN_Workspace_h_
#define __ADN_Workspace_h_

#include "ADN_Enums.h"

class ADN_ActiveProtections_Data;
class ADN_ActiveProtections_GUI;
class ADN_AiEngine_Data;
class ADN_AiEngine_GUI;
class ADN_Automata_Data;
class ADN_Automata_GUI;
class ADN_Breakdowns_Data;
class ADN_Breakdowns_GUI;
class ADN_Categories_Data;
class ADN_Categories_GUI;
class ADN_Communications_Data;
class ADN_Communications_GUI;
class ADN_Crowds_Data;
class ADN_Crowds_GUI;
class ADN_Data_ABC;
class ADN_Disasters_Data;
class ADN_Disasters_GUI;
class ADN_Drawings_Data;
class ADN_Drawings_GUI;
class ADN_Equipments_Data;
class ADN_Equipments_GUI;
class ADN_FileLoaderObserver;
class ADN_Fires_Data;
class ADN_Fires_GUI;
class ADN_GeneralConfig;
class ADN_GUI_ABC;
class ADN_HumanFactors_Data;
class ADN_HumanFactors_GUI;
class ADN_Inhabitants_Data;
class ADN_Inhabitants_GUI;
class ADN_KnowledgeGroups_Data;
class ADN_KnowledgeGroups_GUI;
class ADN_Languages_Data;
class ADN_Languages_GUI;
class ADN_Launchers_Data;
class ADN_Launchers_GUI;
class ADN_Logistic_Data;
class ADN_Logistic_GUI;
class ADN_MainWindow;
class ADN_Missions_Data;
class ADN_Missions_GUI;
class ADN_Models_Data;
class ADN_Models_GUI;
class ADN_NBC_Data;
class ADN_NBC_GUI;
class ADN_Objects_Data;
class ADN_Objects_GUI;
class ADN_ProgressBar;
class ADN_Project_Data;
class ADN_Ref_ABC;
class ADN_Reports_Data;
class ADN_Reports_GUI;
class ADN_ResourceNetworks_Data;
class ADN_ResourceNetworks_GUI;
class ADN_Resources_Data;
class ADN_Resources_GUI;
class ADN_Sensors_Data;
class ADN_Sensors_GUI;
class ADN_Symbols_Data;
class ADN_Symbols_GUI;
class ADN_Units_Data;
class ADN_Units_GUI;
class ADN_UnitSymbols_Data;
class ADN_UnitSymbols_GUI;
class ADN_Urban_Data;
class ADN_Urban_GUI;
class ADN_Weapons_Data;
class ADN_Weapons_GUI;
class ADN_WorkspaceElement_ABC;
template< typename DATA, typename GUI > class ADN_WorkspaceElement;

namespace ADN_NavigationInfos
{
    struct GoTo;
    struct UsedBy;
}

namespace kernel
{
    class Context;
    class LanguageController;
}

namespace tools
{
    class Loader_ABC;
}

#define DECLARE_WORKSPACE_ELEMENT_GETTER( ELEMENT )                                                 \
ADN_WorkspaceElement< ADN_##ELEMENT##_Data, ADN_##ELEMENT##_GUI >& Get##ELEMENT##();                \
const ADN_WorkspaceElement< ADN_##ELEMENT##_Data, ADN_##ELEMENT##_GUI >& Get##ELEMENT##() const;    \

// =============================================================================
/** @class  ADN_Workspace
    @brief  ADN_Workspace
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_Workspace : public QObject
{
    Q_OBJECT

public:
    //! @name Singleton implementation
    //@{
    static void CreateWorkspace( ADN_MainWindow& mainWindow, const ADN_GeneralConfig& config );
    static bool HasWorkspace();
    static ADN_Workspace& GetWorkspace();
    static void DeleteWorkspace();
    //@}

private:
    //! @name Constructors/destructor
    //@{
    explicit ADN_Workspace( ADN_MainWindow& mainWindow, const ADN_GeneralConfig& config );
    virtual ~ADN_Workspace();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< E_WorkspaceElements, QStringList > T_UsingElements;
    //@}

    //! @name Data operations
    //@{
    void Purge();

    void New( const tools::Path& filename, bool loadGui );
    void Load( const tools::Path& filename, bool loadGui );
    bool SaveAs( const tools::Path& filename );
    //@}

    //! @name Gui Operations
    //@{
    void BuildGUI();
    void ExportHtml( const tools::Path& strPath );

    T_UsingElements GetElementThatWillBeDeleted( ADN_Ref_ABC* data );
    T_UsingElements GetElementThatUse( ADN_Ref_ABC* data );
    //@}

    //! @name Accessors
    //@{
    bool ApplyOnData( const boost::function< bool( ADN_Data_ABC& data ) >& functor );
    bool ApplyOnGui( const boost::function< bool( ADN_GUI_ABC& data ) >& functor );

    const boost::shared_ptr< kernel::Context >& GetContext( E_WorkspaceElements element, const std::string& context );
    const boost::shared_ptr< kernel::Context >& GetContext( E_WorkspaceElements element, int subElement, const std::string& context );

    bool IsNewBaseReadOnly( const tools::Path& filename ) const;

    const ADN_GeneralConfig& GetConfig() const;
    kernel::LanguageController& GetLanguageController();
    ADN_MainWindow& GetMainWindow() const;
    void SetMainWindowModified( bool isModified );
    void SetIsSwappingLanguage( bool isSwappingLanguage );

    ADN_Project_Data& GetProject();
    ADN_WorkspaceElement_ABC& GetWorkspaceElement( E_WorkspaceElements workspaceElement );

    DECLARE_WORKSPACE_ELEMENT_GETTER( Languages );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Categories );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Symbols );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Urban );
    DECLARE_WORKSPACE_ELEMENT_GETTER( NBC );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Launchers );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Resources );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Fires );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Drawings );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Disasters );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Objects );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Weapons );
    DECLARE_WORKSPACE_ELEMENT_GETTER( ActiveProtections );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Sensors );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Breakdowns );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Equipments );
    DECLARE_WORKSPACE_ELEMENT_GETTER( ResourceNetworks );
    DECLARE_WORKSPACE_ELEMENT_GETTER( AiEngine );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Missions );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Models );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Units );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Automata );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Communications );
    DECLARE_WORKSPACE_ELEMENT_GETTER( HumanFactors );
    DECLARE_WORKSPACE_ELEMENT_GETTER( KnowledgeGroups );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Crowds );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Inhabitants );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Reports );
    DECLARE_WORKSPACE_ELEMENT_GETTER( Logistic );
    //@}

signals:
    //! @name Signals
    //@{
    void ChangeTab( E_WorkspaceElements targetTab );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnUsersListRequested( const ADN_NavigationInfos::UsedBy& usedByInfo );
    void OnGoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );
    void OnChooseOptional( bool choice );
    //@}

private:
    //! @name Helpers
    //@{
    void AddPage( E_WorkspaceElements element );
    void Initialize();
    void LoadDefaultSymbols();
    //@}

private:
    //! @name Member data
    //@{
    ADN_MainWindow& mainWindow_;
    ADN_ProgressBar& progressIndicator_;
    const ADN_GeneralConfig& config_;
    std::unique_ptr< kernel::LanguageController > languageController_;
    std::unique_ptr< ADN_FileLoaderObserver > fileLoaderObserver_;
    std::unique_ptr< const tools::Loader_ABC > fileLoader_;
    std::unique_ptr< ADN_Project_Data > projectData_;
    std::unique_ptr< ADN_WorkspaceElement_ABC > elements_[ eNbrWorkspaceElements ];
    bool isSwappingLanguage_;
    //@}

    //! @name Singleton
    //@{
    static ADN_Workspace* pWorkspace_;
    //@}
};

#endif // __ADN_Workspace_h_
