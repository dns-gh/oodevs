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
#include "ADN_NBC_Datas.h"
#include "ADN_GroupBox.h"
#include "ADN_MultiPercentage.h"

typedef ADN_NBC_Datas::NbcAgentInfos NbcAgentInfos;

namespace
{
    class ADN_NBC_Intox_Connector : public ADN_Connector_ABC
    {
    public:
        explicit ADN_NBC_Intox_Connector( T_ConnectorVector& itemConnectors ) : vItemConnectors_( itemConnectors ) {}
        virtual ~ADN_NBC_Intox_Connector() {}

        virtual void Connect( ADN_Ref_ABC* pTarget, bool bConnect )
        {
            ADN_NBC_Datas::NbcIntoxInfos* pInfos = (ADN_NBC_Datas::NbcIntoxInfos*)pTarget;
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
            throw std::runtime_error( "Not to be called" );
        }

    private:
        T_ConnectorVector& vItemConnectors_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Intox_GUI constructor
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Intox_GUI::ADN_NBC_Intox_GUI( QWidget* pParent )
    : ADN_Gfx_ABC()
    , Q3VBox( pParent )
    , vInfosConnectors_( eNbrGuiElements, (ADN_Connector_ABC*)0 )
{
    ADN_GuiBuilder builder;

    ADN_GroupBox* pIntoxGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Poisoning effect" ), this );
    vInfosConnectors_[eIntoxPresent] = &pIntoxGroup->GetConnector();

    ADN_MultiPercentage* pMultiPercentage = new ADN_MultiPercentage( pIntoxGroup, builder );
    pMultiPercentage->AddLine( tr( "Unwounded" ),                   vInfosConnectors_[ eNbrOk ] );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors_[ eNbrHurt1 ] );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors_[ eNbrHurt2 ] );
    pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors_[ eNbrHurt3 ] );
    pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors_[ eNbrHurt4 ] );
    pMultiPercentage->AddLine( tr( "Killed" ),                      vInfosConnectors_[ eNbrDead ] );
    pMultiPercentage->AddWarning();

    QWidget* pHolder = builder.AddFieldHolder( this );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Contamination" ), vInfosConnectors_[eContaminationPresent] );

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


