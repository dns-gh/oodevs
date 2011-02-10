//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Objects_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:37 $
// $Revision: 18 $
// $Workfile: ADN_Objects_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Objects_GUI.h"
#include "moc_ADN_Objects_GUI.cpp"
#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_CommonGfx.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_Objects_Data.h"
#include "ADN_ListView_Objects.h"
#include "ADN_Table_Objects_LocationScore.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_GroupBox.h"
#include "ADN_RadioButton.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_EquipementSelector.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ENT/ENT_Tr.h"
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qbuttongroup.h>
#include <qgrid.h>


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::ADN_Objects_GUI( ADN_Objects_Data& data )
    : ADN_GUI_ABC( "ADN_Objects_GUI" )
    , data_( data )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::~ADN_Objects_GUI()
{
    // NOTHING
}

namespace
{
    class ADN_ComboBox_List : public ADN_ComboBox
    {
    private:
        class ADN_CCB : public ADN_Connector_EnumComboBox
        {
        public:
            explicit ADN_CCB( ADN_ComboBox_List& parent )
                : ADN_Connector_EnumComboBox( &parent )
            {
                // NOTHING
            }
            virtual ~ADN_CCB()
            {
                // NOTHING
            }
            std::string GetItem(void * obj)
            {
                return *static_cast< std::string* >( obj );
            }
        };
    public:
        explicit ADN_ComboBox_List( QWidget* parent = 0, const char * name = 0 )
            : ADN_ComboBox( parent, name )
        {
            pConnector_ = new ADN_CCB( *this );
        }
        virtual ~ADN_ComboBox_List()
        {
            // NOTHING
        }
    protected:
        void UpdateEnableState()
        {
            // NOTHING
        }
    };
}
//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Build
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Objects_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // Create the object listview.
    ADN_ListView_Objects* pList = new ADN_ListView_Objects( pMainWidget_ );
    pList->GetConnector().Connect( &data_.GetObjectInfos() );
    T_ConnectorVector vInfosConnectors ( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Create the object panel.
    pGroup_ = new QGroupBox( 1, Qt::Horizontal, tr( "Object" ), pMainWidget_ );
    QWidget* pHolder = builder.AddFieldHolder( pGroup_ );

    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name"),  vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Geometry"), vInfosConnectors[ eGeometry ] );
    builder.SetEnabled( false );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Symbol"), vInfosConnectors[ eSymbol ] );
    builder.SetEnabled( false );

    QGroupBox* hBox = new QGroupBox( 2, Qt::Horizontal, tr( "Capacities" ), pGroup_ );

    ADN_GroupBox* constructor = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Constructor" ), hBox );
    {
        vInfosConnectors[ eConstructorCapacityPresent ] = & constructor->GetConnector();
        // Comsumption
        builder.AddEnumField< E_ConsumptionType >( constructor, tr( "Default consumption" ), vInfosConnectors[ eConstructorCapacity_DefaultConsumption ], ADN_Tr::ConvertFromConsumptionType );
        builder.AddField< ADN_ComboBox_List >( constructor, tr( "Model"), vInfosConnectors[ eConstructorCapacity_UnitType ] );
        // Buildable
        ADN_GroupBox* buildable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Buildable" ), constructor );
        {
            vInfosConnectors[ eBuildableCapacityPresent ] = & buildable->GetConnector();

            ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, buildable, false );
            vInfosConnectors[ eBuildableCapacity_Dotation ] = &pDotations->GetConnector();
        }

        // Improvable
        ADN_GroupBox* improvable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Improvable" ), constructor );
        {
            vInfosConnectors[ eImprovableCapacityPresent ] = & improvable->GetConnector();

            ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, improvable, false );
            vInfosConnectors[ eImprovableCapacity_Dotation ] = &pDotations->GetConnector();
        }
    }

    ADN_GroupBox* heuristic = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Terrain Heuristic" ), hBox );
    {
        vInfosConnectors[ eTerrainHeuristicCapacityPresent ] = & heuristic->GetConnector();
        ADN_Table_Objects_LocationScore* pScoreLocation = new ADN_Table_Objects_LocationScore( heuristic );
        vInfosConnectors[eTerrainHeuristicCapacity_LocationScore] = &pScoreLocation->GetConnector();
    }

    // Avoidable
    ADN_GroupBox* avoidable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Avoidable" ), hBox );
    {
        vInfosConnectors[ eAvoidableCapacityPresent ] = & avoidable->GetConnector();
        // Distance
        builder.AddField< ADN_EditLine_Double >( avoidable, tr( "Distance" ), vInfosConnectors[ eAvoidableCapacity_Distance ], tr( "m" ) );
    }

    // Bypassable
    ADN_GroupBox* bypassable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Bypassable" ), hBox );
    {
        vInfosConnectors[ eBypassableCapacityPresent ] = & bypassable->GetConnector();
        // Distance
        builder.AddField< ADN_EditLine_Double >( bypassable, tr( "Bypass Speed" ), vInfosConnectors[ eBypassableCapacity_Speed ], tr( "km/h" ) );
    }

    // Activable
    ADN_GroupBox* activable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Activable" ), hBox );
    {
        vInfosConnectors[ eActivableCapacityPresent ] = & activable->GetConnector();
    }

    // Logistic
    ADN_GroupBox* logistic = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Logistic" ), hBox );
    {
        vInfosConnectors[ eLogisticCapacityPresent ] = & logistic->GetConnector();
    }

    // Supply route
    ADN_GroupBox* supplyRoute = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Supply-Route" ), hBox );
    {
        vInfosConnectors[ eSupplyRouteCapacityPresent ] = & supplyRoute->GetConnector();
    }

    // Mobility
    ADN_GroupBox* mobility = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Mobility" ), hBox );
    {
        vInfosConnectors[ eMobilityCapacityPresent ] = & mobility->GetConnector();

        // Default speed
        builder.AddField< ADN_EditLine_Double >( mobility, tr( "Default speed"), vInfosConnectors[ eMobilityCapacity_DefaultSpeed ], tr( "km/h" ) );
        pSpeedImpactCombo_ = builder.AddEnumField< E_SpeedImpact >( mobility, tr( "Speed impact"), vInfosConnectors[ eMobilityCapacity_SpeedModifier ], ADN_Tr::ConvertFromSpeedImpact );
        pMaxAgentSpeed_ = builder.AddField< ADN_EditLine_Double >( mobility, tr( "Max agent speed"), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );
    }

    ADN_GroupBox* workable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Workable" ), hBox );
    {
        vInfosConnectors[ eWorkableCapacityPresent ] = & workable->GetConnector();
        builder.AddField< ADN_EditLine_Int >( workable, tr( "Max Animator: " ), vInfosConnectors[ eWorkableCapacity_Size ], tr( "agents" ) );
    }

    ADN_GroupBox* attrition = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Attrition" ), hBox );
    {
        vInfosConnectors[ eAttritionCapacityPresent ] = & attrition->GetConnector();
        ADN_GroupBox* dotation = new ADN_GroupBox( 2, Qt::Horizontal, tr( "Use ammunition" ), attrition );
        vInfosConnectors[ eAttritionCapacityUseDotation ] = & dotation->GetConnector();
        builder.AddField< ADN_ComboBox_Vector<ADN_Equipement_Data::AmmoCategoryInfo> >( dotation, tr( "Dotation" ), vInfosConnectors[ eAttritionDotation ] );
    }

    // NBC
    QGroupBox* gNBC = new QGroupBox( 2, Qt::Horizontal, tr( "NBC" ), hBox );
    // Contamination
    ADN_GroupBox* contamination = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Contamination" ), gNBC );
    {
        vInfosConnectors[ eContaminationCapacityPresent ] = & contamination->GetConnector();
        builder.AddField< ADN_EditLine_Int >( contamination, tr( "Max Toxic" ), vInfosConnectors[ eContaminationCapacity_MaxToxic ], tr( "items" ) );
    }

    // Intoxication
    ADN_GroupBox* intoxication = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Intoxication" ), gNBC );
    {
        vInfosConnectors[ eIntoxicationCapacityPresent ] = & intoxication->GetConnector();
        builder.AddField< ADN_EditLine_Int >( intoxication, tr( "Max Toxic" ), vInfosConnectors[ eIntoxicationCapacity_MaxToxic ], tr( "items" ) );
    }

    // Decontamination
    ADN_GroupBox* decontamination = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Decontamination" ), hBox );
    {
        vInfosConnectors[ eDecontaminationCapacityPresent ] = & decontamination->GetConnector();
    }

    // Population filter
    ADN_GroupBox* populationFilter = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Population" ), hBox );
    {
        vInfosConnectors[ ePopulationCapacityPresent ] = & populationFilter->GetConnector();
        builder.AddField< ADN_EditLine_Double >( populationFilter, tr( "Density: " ), vInfosConnectors[ ePopulationFilter_Density ] );
    }

    // Detection
    ADN_GroupBox* detection = new ADN_GroupBox( 3, Qt::Horizontal, tr( ADN_Objects_Data::ADN_CapacityInfos_Detection::DISPLAY_NAME.c_str() ), hBox );
    {
        vInfosConnectors[ eDetectionCapacityPresent ] = & detection->GetConnector();
    // LTO begin
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Detection duration" ), vInfosConnectors[eHasDetectionTime], vInfosConnectors[eDetectionTime] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Recognition duration" ), vInfosConnectors[eHasRecoTime], vInfosConnectors[eRecoTime] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Identification duration" ), vInfosConnectors[eHasIdentificationTime], vInfosConnectors[eIdentificationTime] );
    // LTO end
    }

    // Spawn
    ADN_GroupBox* spawn = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Spawn" ), hBox );
    {
        vInfosConnectors[ eSpawnCapacityPresent ] = & spawn->GetConnector();
        builder.AddField<ADN_EditLine_Double>( spawn, tr( "Action range" ), vInfosConnectors[eActionRange], tr( "m" ) );
        builder.AddField<ADN_EditLine_String>( spawn, tr( "Object type" ), vInfosConnectors[eObjectType] );
    }

    ADN_GroupBox* extinguishable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Extinguishable" ), hBox );
    {
        vInfosConnectors[ eExtinguishableCapacityPresent ] = & extinguishable->GetConnector();
    }

    ADN_GroupBox* medical = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Medical" ), hBox );
    {
        vInfosConnectors[ eMedicalCapacityPresent ] = & medical->GetConnector();
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Doctor night rate" ), vInfosConnectors[ eMedicalCapacity_NightRate ] );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency doctor rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyDoctorRate ] );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency bed rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyBedRate ] );
    }

    ADN_GroupBox* interference = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Interference" ), hBox );
    {
        vInfosConnectors[ eInterferenceCapacityPresent ] = & interference->GetConnector();
    }

    ADN_GroupBox* interactWithEnemy = new ADN_GroupBox( 3, Qt::Horizontal, tr( "InteractWithEnemy" ), hBox );
    {
        vInfosConnectors[ eInteractWithEnemyCapacityPresent ] = & interactWithEnemy->GetConnector();
    }

    ADN_GroupBox* heightInteraction = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Height interaction" ), hBox );
    {
        vInfosConnectors[ eHeightInteractionPresent ] = & heightInteraction->GetConnector();
        builder.AddField< ADN_EditLine_Double >( heightInteraction, tr( "Interaction max height" ), vInfosConnectors[ eHeightInteraction ], tr( "m" ) );
    }

    ADN_GroupBox* protection = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Protection" ), hBox );
    {
        vInfosConnectors[ eProtectionCapacityPresent ] = & protection->GetConnector();
        builder.AddField< ADN_EditLine_Int >( protection, tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new QIntValidator( 1, INT_MAX, this ) );
        builder.AddField< ADN_CheckBox >( protection, tr( "Genie prepared" ), vInfosConnectors[ eProtectionCapacity_GeniePrepared ] );
    }

    ADN_GroupBox* occupable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Occupable" ), hBox );
    {
        vInfosConnectors[ eOccupableCapacityPresent ] = & occupable->GetConnector();
    }

    ADN_GroupBox* supply = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Supply" ), hBox );
    {
        vInfosConnectors[ eSupplyCapacityPresent ] = & supply->GetConnector();
    }

    ADN_GroupBox* bridging = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Bridging" ), hBox );
    {
        vInfosConnectors[ eBridgingCapacityPresent ] = & bridging->GetConnector();
    }

    ADN_GroupBox* time_limited = new ADN_GroupBox( 3, Qt::Horizontal, tr( "TimeLimited" ), hBox );
    {
        vInfosConnectors[ eTimeLimitedCapacityPresent ] = & time_limited->GetConnector();
    }

    ADN_GroupBox* propagation = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Propagation" ), hBox );
    {
        vInfosConnectors[ ePropagationCapacityPresent ] = & propagation->GetConnector();
        builder.AddField< ADN_ComboBox_List >( propagation, tr( "Model"), vInfosConnectors[ ePropagationCapacity_ModelType ] );
    }

    ADN_GroupBox* attitudeModifier = new ADN_GroupBox( 3, Qt::Horizontal, tr( "AttitudeModifier" ), hBox );
    {
        vInfosConnectors[ eAttitudeModifierCapacityPresent ] = & attitudeModifier->GetConnector();
        builder.AddEnumField< E_PopulationAttitude >( attitudeModifier, tr( "Attitude" ), vInfosConnectors[eAttitude], ENT_Tr::ConvertFromPopulationAttitude );
    }

    ADN_GroupBox* perception = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Perception" ), hBox );
    {
        vInfosConnectors[ ePerceptionCapacityPresent ] = & perception->GetConnector();
        builder.AddField<ADN_CheckBox>( perception, tr( "Aveuglant" ), vInfosConnectors[eBlinded] );
    }

    ADN_GroupBox* scattering = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Scattering" ), hBox );
    {
        vInfosConnectors[ eScatteringCapacityPresent ] = & scattering->GetConnector();
        builder.AddField<ADN_EditLine_Int>( scattering, tr( "Nombre d'humain par pas de simulation" ), vInfosConnectors[eHumanByTimeStep] );
        builder.SetValidator( new QIntValidator( 1, INT_MAX, this ) );
    }

    ADN_GroupBox* delay = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Delay time" ), hBox );
    {
        vInfosConnectors[ eDelayCapacityPresent ] = & delay->GetConnector();
    }

    ADN_GroupBox* structural = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Structural state" ), hBox );
    {
        vInfosConnectors[ eStructuralStateCapacityPresent ] = & structural->GetConnector();
        builder.AddField< ADN_EditLine_Int >( structural, tr( "Initial value"), vInfosConnectors[ eStructuralStateCapacity_Value ], tr( "%" ), ePercentage );
    }

    ADN_GroupBox* sealOff = new ADN_GroupBox( 3, Qt::Horizontal, tr( "SealOff" ), hBox );
    {
        vInfosConnectors[ eSealOffCapacityPresent ] = & sealOff->GetConnector();
    }

    ADN_GroupBox* firePropagationModifier = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Fire propagation modifier" ), hBox );
    {
        vInfosConnectors[ eFirePropagationModifierPresent ] = & firePropagationModifier->GetConnector();
        ADN_Table_Objects_FirePropagationModifier* pFirePropagationModifierTable = new ADN_Table_Objects_FirePropagationModifier( firePropagationModifier );
        vInfosConnectors[eFirePropagationModifier_Modifiers] = &pFirePropagationModifierTable->GetConnector();
    }

    ADN_GroupBox* burn = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Burn" ), hBox );
    {
        vInfosConnectors[ eBurnCapacityPresent ] = & burn->GetConnector();
    }

    ADN_GroupBox* flood = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Flood" ), hBox );
    {
        vInfosConnectors[ eFloodCapacityPresent ] = & flood->GetConnector();
        QWidget* pInjuriesHolder = builder.AddFieldHolder( flood );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 1" ), vInfosConnectors[ eFloodCapacity_HurtHumans1 ], tr( "%" ), ePercentage  );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 2" ), vInfosConnectors[ eFloodCapacity_HurtHumans2 ], tr( "%" ), ePercentage  );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 3" ), vInfosConnectors[ eFloodCapacity_HurtHumans3 ], tr( "%" ), ePercentage  );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded extreme seriousness" ), vInfosConnectors[ eFloodCapacity_HurtHumansE ], tr( "%" ), ePercentage  );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Killed" ), vInfosConnectors[ eFloodCapacity_DeadHumans ], tr( "%" ), ePercentage  );
    }

    ADN_GroupBox* universal = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Universal" ), hBox );
    {
        vInfosConnectors[ eUniversalCapacityPresent ] = & universal->GetConnector();
    }

    // Connect the list to the interface.
    pList->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( pMainWidget_, 10, 10 );

    // Main horizontal layout
    QBoxLayout* pListAndDataLayout = new QHBoxLayout( 0, 10, 10 );
    pListAndDataLayout->addWidget( pList, 1 );
    pListAndDataLayout->addWidget( pGroup_, 2 );
    pMainLayout->addLayout( pListAndDataLayout, 2 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnSpeedImpactComboChanged
// Created: APE 2005-04-29
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnSpeedImpactComboChanged()
{
    pMaxAgentSpeed_->setEnabled( pSpeedImpactCombo_->currentText() == ADN_Tr::ConvertFromSpeedImpact( eSpeedImpact_VitesseMaxAgent, ENT_Tr_ABC::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::Enable( bool enable )
{
    pGroup_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Objects/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Objects" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = data_.GetObjectInfos();
    indexBuilder.BeginTable( objects.size()+1 , 4 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ), true );
    indexBuilder.TableItem( 0, 2, tr( "Geometry" ), true );
    indexBuilder.TableItem( 0, 3, tr( "Capacities" ), true );
    int n = 1;
    for( ADN_Objects_Data::IT_ObjectsInfos_Vector it = objects.begin(); it != objects.end(); ++it, ++n )
    {
        ADN_Objects_Data::ObjectInfos& object = **it;
        indexBuilder.TableItem( n, 0, object.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, object.strType_.GetData().c_str() );
        indexBuilder.TableItem( n, 2, object.geometries_.GetData().c_str() );

        ADN_HtmlBuilder listBuilder;
        listBuilder.BeginList();
        for( ADN_Objects_Data::ObjectInfos::CIT_CapacityMap itCapacity = object.capacities_.begin(); itCapacity != object.capacities_.end(); ++itCapacity )
        {
            if( itCapacity->second->bPresent_.GetData() )
                listBuilder.ListItem( itCapacity->first.c_str() );
        }
        listBuilder.EndList();

        indexBuilder.TableItem( n, 3,  listBuilder.Stream().str().c_str() );

    }
    indexBuilder.EndTable();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "Objects/" ) + "index.htm\">" + tr( "Objects" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}
