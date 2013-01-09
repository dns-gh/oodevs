// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameterType.h"
#include "moc_ADN_MissionParameterType.cpp"
#include "ADN_Tr.h"
#include "ADN_Missions_GUI.h"

namespace
{
    class ADN_Private_EnumConnector : public ADN_Connector_TableItem_EnumComboBox
    {
    public:
                 ADN_Private_EnumConnector( ADN_MissionParameterType& combo ) : ADN_Connector_TableItem_EnumComboBox( &combo ) {}
        virtual ~ADN_Private_EnumConnector() {}

        virtual std::string GetItem( void* obj )
        {
            return ADN_Tr::ConvertFromMissionParameterType( *(E_MissionParameterType*)obj, ENT_Tr_ABC::eToTr );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterType constructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterType::ADN_MissionParameterType( ADN_Table* parent, void* data, T_ConnectorVector& itemConnectors )
    : ADN_TableItem_ComboBox( parent, data )
    , itemConnectors_( itemConnectors )
{
    pConnector_ = new ADN_Private_EnumConnector( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterType destructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterType::~ADN_MissionParameterType()
{
    Disconnect();
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterType::Update
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_MissionParameterType::Update()
{
    DoValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterType::DoValueChanged
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_MissionParameterType::DoValueChanged()
{
    ADN_TableItem_ComboBox::DoValueChanged();
    ADN_Missions_Data::MissionParameter* param = static_cast< ADN_Missions_Data::MissionParameter* >( pData_ );
    bool isEnum = param->type_.GetData() == eMissionParameterTypeEnumeration;
    if( !isEnum )
        param->values_.Clear();
    itemConnectors_[ADN_Missions_GUI::eParameterValues]->Connect( &param->values_, isEnum );
    bool isChoice = param->type_.GetData() == eMissionParameterTypeLocationComposite;
    itemConnectors_[ADN_Missions_GUI::eChoiceValues]->Connect( &param->choices_, isChoice );
    bool isNumeric = param->type_.GetData() == eMissionParameterTypeNumeric;
    itemConnectors_[ADN_Missions_GUI::eMinValue]->Connect( &param->minValue_, isNumeric );
    itemConnectors_[ADN_Missions_GUI::eMaxValue]->Connect( &param->maxValue_, isNumeric );
    bool isObjectKnowledge = param->type_.GetData() == eMissionParameterTypeObjectKnowledge;
    itemConnectors_[ADN_Missions_GUI::eKnowledgeObjects]->Connect( &param->knowledgeObjects_, isObjectKnowledge );

    emit TypeChanged( param->type_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterType::Disconnect
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
void ADN_MissionParameterType::Disconnect()
{
    ADN_Missions_Data::MissionParameter* param = static_cast< ADN_Missions_Data::MissionParameter* >( pData_ );
    if( param )
    {
        itemConnectors_[ADN_Missions_GUI::eParameterValues]->Disconnect( &param->values_ );
        itemConnectors_[ADN_Missions_GUI::eChoiceValues]->Disconnect( &param->choices_ );
        itemConnectors_[ADN_Missions_GUI::eMinValue]->Disconnect( &param->minValue_ );
        itemConnectors_[ADN_Missions_GUI::eMaxValue]->Disconnect( &param->maxValue_ );
        itemConnectors_[ADN_Missions_GUI::eKnowledgeObjects]->Disconnect( &param->knowledgeObjects_ );
    }
}
