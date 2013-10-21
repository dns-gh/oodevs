// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_Intox_GUI.h"
#include "moc_ADN_NBC_Intox_GUI.cpp"

#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_NBC_Data.h"
#include "ADN_GroupBox.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Tools.h"

typedef ADN_NBC_Data::NbcAgentInfos NbcAgentInfos;

namespace
{
    class ADN_NBC_Intox_Connector : public ADN_Connector_ABC
    {
    public:
        explicit ADN_NBC_Intox_Connector( T_ConnectorVector& itemConnectors ) : vItemConnectors_( itemConnectors ) {}
        virtual ~ADN_NBC_Intox_Connector() {}

        virtual void Connect( ADN_Ref_ABC* pTarget, bool bConnect )
        {
            ADN_NBC_Data::NbcIntoxInfos* pInfos = (ADN_NBC_Data::NbcIntoxInfos*)pTarget;
            if( !pTarget )
                return;

            ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_NBC_Intox_GUI::eNbrGuiElements );

            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrOk]->Connect( &pInfos->rNbAlivedHumans_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrHurt1]->Connect( &pInfos->rNbHurtedHumans1_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrHurt2]->Connect( &pInfos->rNbHurtedHumans2_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrHurt3]->Connect( &pInfos->rNbHurtedHumans3_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrHurt4]->Connect( &pInfos->rNbHurtedHumansE_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eNbrDead]->Connect( &pInfos->rNbDeadHumans_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eContaminationPresent]->Connect( &pInfos->bContaminationPresent_, bConnect );
            vItemConnectors_[ADN_NBC_Intox_GUI::eIntoxPresent]->Connect( &pInfos->bIntoxPresent_, bConnect );
        }

    protected:
        ADN_NBC_Intox_Connector& operator=( const ADN_NBC_Intox_Connector& );

        void SetDataPrivate( void* /*pData*/ )
        {
            throw MASA_EXCEPTION( "Not to be called" );
        }

    private:
        T_ConnectorVector& vItemConnectors_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Intox_GUI constructor
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Intox_GUI::ADN_NBC_Intox_GUI( QWidget* pParent, const QString& objectName )
    : ADN_Gfx_ABC()
    , Q3VBox( pParent )
    , vInfosConnectors_( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) )
{
    setObjectName( objectName );
    ADN_GuiBuilder builder( objectName );

    ADN_GroupBox* pIntoxGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Poisoning effect" ), this, "nbc-intox" );
    vInfosConnectors_[eIntoxPresent] = &pIntoxGroup->GetConnector();

    ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pIntoxGroup, builder, builder.GetChildName( "poisoning-effect" ) );
    pMultiPercentage->AddLine( tr( "Unwounded" ),                   vInfosConnectors_[ eNbrOk ]    , "ok" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors_[ eNbrHurt1 ] , "hurt1" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors_[ eNbrHurt2 ] , "hurt2" );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors_[ eNbrHurt3 ] , "hurt3" );
    pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors_[ eNbrHurt4 ] , "hurt4" );
    pMultiPercentage->AddLine( tr( "Killed" ),                      vInfosConnectors_[ eNbrDead ]  , "dead" );
    pMultiPercentage->AddWarning();

    QWidget* pHolder = builder.AddFieldHolder( this );
    builder.AddField< ADN_CheckBox >( pHolder, "contamination", tr( "Contamination" ), vInfosConnectors_[eContaminationPresent] );

    pConnector_ = new ADN_NBC_Intox_Connector( vInfosConnectors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Intox_GUI destructor
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Intox_GUI::~ADN_NBC_Intox_GUI()
{
    delete pConnector_;
}

