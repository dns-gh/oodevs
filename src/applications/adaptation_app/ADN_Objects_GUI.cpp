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
#include "ADN_Objects_Data.h"
#include "ADN_ListView_Objects.h"
#include "ADN_Table_Objects_LocationScore.h"
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
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::ADN_Objects_GUI( ADN_Objects_Data& data )
    : ADN_GUI_ABC( "ADN_Objects_GUI" )
    , data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::~ADN_Objects_GUI()
{
}

namespace
{
    class ADN_ComboBox_List
        : public ADN_ComboBox
    {
    private:
        
        class ADN_CCB
            : public ADN_Connector_EnumComboBox
        {
        public:
            explicit ADN_CCB( ADN_ComboBox_List& parent ) : ADN_Connector_EnumComboBox( &parent ) {}
            virtual ~ADN_CCB() {}
            
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
        virtual ~ADN_ComboBox_List() {}

    protected:
        void UpdateEnableState()
        {
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
        builder.AddEnumField< E_SpeedImpact >( mobility, tr( "Speed impact"), vInfosConnectors[ eMobilityCapacity_SpeedModifier ], ADN_Tr::ConvertFromSpeedImpact );
        builder.AddField< ADN_EditLine_Double >( mobility, tr( "Max agent speed"), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        // connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );
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

    // Detection
	ADN_GroupBox* detection = new ADN_GroupBox( 3, Qt::Horizontal, tr( ADN_Objects_Data::ADN_CapacityInfos_Detection::DISPLAY_NAME.c_str() ), hBox );        
    {
        vInfosConnectors[ eDetectionCapacityPresent ] = & detection->GetConnector();
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Detection duration" ), vInfosConnectors[eHasDetectionTime], vInfosConnectors[eDetectionTime] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Recognition duration" ), vInfosConnectors[eHasRecoTime], vInfosConnectors[eRecoTime] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Identification duration" ), vInfosConnectors[eHasIdentificationTime], vInfosConnectors[eIdentificationTime] );
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

	ADN_GroupBox* healable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Healable" ), hBox );        
    {
        vInfosConnectors[ eHealableCapacityPresent ] = & healable->GetConnector();
    }

	ADN_GroupBox* interference = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Interference" ), hBox );        
    {
        vInfosConnectors[ eInterferenceCapacityPresent ] = & interference->GetConnector();
    }

	ADN_GroupBox* interactWithEnemy = new ADN_GroupBox( 3, Qt::Horizontal, tr( "InteractWithEnemy" ), hBox );        
    {
        vInfosConnectors[ eInteractWithEnemyCapacityPresent ] = & interactWithEnemy->GetConnector();
    }

	ADN_GroupBox* protection = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Protection" ), hBox );        
    {
        vInfosConnectors[ eProtectionCapacityPresent ] = & protection->GetConnector();
        builder.AddField< ADN_EditLine_Int >( protection, tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
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
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
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
    if( pSpeedImpactCombo_->currentText() == ADN_Tr::ConvertFromSpeedImpact( eSpeedImpact_VitesseMaxAgent ).c_str() )
        pMaxAgentSpeed_->setEnabled( true );
    else
        pMaxAgentSpeed_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::Enable( bool enable )
{
    pGroup_->setEnabled( enable );
}
