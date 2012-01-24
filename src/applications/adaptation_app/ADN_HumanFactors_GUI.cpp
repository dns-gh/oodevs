// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HumanFactors_GUI.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 17:49 $
// $Revision: 6 $
// $Workfile: ADN_HumanFactors_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HumanFactors_GUI.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Weapons_GUI.h"

// -----------------------------------------------------------------------------
// Name: ADN_PH_EditLine_Double
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
class ADN_PH_EditLine_Double : public ADN_EditLine_Double
{
public:
    explicit ADN_PH_EditLine_Double(QWidget* parent, const char * name = 0)
        : ADN_EditLine_Double( parent, name ) {}

protected:
    virtual void TextChanged(const QString& string)
    {
        ADN_EditLine_Double::TextChanged( string );
        ADN_Workspace::GetWorkspace().GetWeapons().GetGui().UpdateModifiers();
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HumanFactors_GUI::ADN_HumanFactors_GUI( ADN_HumanFactors_Data& data )
    : ADN_GUI_ABC( "ADN_HumanFactors_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HumanFactors_GUI::~ADN_HumanFactors_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_HumanFactors_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Experience
    Q3GroupBox* pExperienceBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Experience modifiers" ), pMainWidget_ );
    {
        BuildModifiers( pExperienceBox, data_.newbieModifiers_, tr( "Newbie" ) );
        BuildModifiers( pExperienceBox, data_.xpModifiers_, tr( "Experienced" ) );
        BuildModifiers( pExperienceBox, data_.veteranModifiers_, tr( "Veteran" ) );
    }
    // Tiredness
    Q3GroupBox* pTirednessBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Tiredness modifiers" ), pMainWidget_ );
    {
        BuildModifiers( pTirednessBox, data_.normalModifiers_, tr( "Not tired" ) );
        BuildModifiers( pTirednessBox, data_.tirednessModifiers_, tr( "Tired" ) );
        BuildModifiers( pTirednessBox, data_.exhaustedModifiers_, tr( "Exhausted" ) );
    }
    // Stress
    Q3GroupBox* pStressBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Stress modifiers" ), pMainWidget_ );
    {
        BuildModifiers( pStressBox, data_.calmModifiers_, tr( "Calm" ) );
        BuildModifiers( pStressBox, data_.worriedModifiers_, tr( "Worried" ) );
        BuildModifiers( pStressBox, data_.stressedModifiers_, tr( "Stressed" ) );
    }
    // Evolution
    Q3GroupBox* pEvolutionBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Automatic evolution" ), pMainWidget_ );
    {
        Q3GroupBox* vBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Tiredness" ), pEvolutionBox );
        BuildThresholds( vBox, data_.tirednessThresholds_, tr( "Thresholds" ), tr( "Tiredness" ), tr( "Exhaustion" ) );
        {
            Q3GroupBox* hBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Evolution (per hour)" ), vBox );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Parked, engine stopped" ), data_.tirednessEngineStopped_ );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Parked, engine running" ), data_.tirednessEngineRunning_ );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Moving" ), data_.tirednessMoving_ );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Working" ), data_.tirednessWorking_ );
        }
        vBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Stress" ), pEvolutionBox );
        BuildThresholds( vBox, data_.stressThresholds_, tr( "Thresholds" ), tr( "Worry" ), tr( "Stress" ) );
        {
            Q3GroupBox* hBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Evolution" ), vBox );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Increase per fire" ), data_.stressIncPerShot_, 0, eGreaterEqualZero );
            builder.AddField< ADN_EditLine_Int >( hBox, tr( "Decrease per hour" ), data_.stressDecPerHour_, 0, eLowerEqualZero  );
        }
    }

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pExperienceBox );
    pContentLayout->addWidget( pTirednessBox );
    pContentLayout->addWidget( pStressBox );
    pContentLayout->addWidget( pEvolutionBox );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI::BuildModifiers
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
QWidget* ADN_HumanFactors_GUI::BuildModifiers( QWidget* pParent, ADN_HumanFactors_Data::ModifiersInfo& modifiers, const QString& szName )
{
    ADN_GuiBuilder builder;
    Q3GroupBox* pGroupBox = new Q3GroupBox( 3, Qt::Horizontal, szName, pParent );
    builder.AddField< ADN_EditLine_Double >( pGroupBox, tr( "Effect on movement speed" ), modifiers.rSpeedModifier_, 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pGroupBox, tr( "Effect on reloading duration" ), modifiers.rReloadModifier_, 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pGroupBox, tr( "Effect on stance changes duration" ), modifiers.rStanceModifier_, 0, eGreaterZero );
    builder.AddField< ADN_PH_EditLine_Double >( pGroupBox, tr( "Effect on PH" ), modifiers.rPHModifier_, 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pGroupBox, tr( "Effect on detection ranges" ), modifiers.rSensorsModifier_, 0, eGreaterZero );
    return pGroupBox;
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI::BuildThresholds
// Created: ABR 2011-12-08
// -----------------------------------------------------------------------------
QWidget* ADN_HumanFactors_GUI::BuildThresholds( QWidget* pParent, ADN_HumanFactors_Data::ThresholdsInfo& thresholds, const QString& szName, const QString& firstThresholdName, const QString& secondThresholdName )
{
    ADN_GuiBuilder builder;
    Q3GroupBox* pGroupBox = new Q3GroupBox( 3, Qt::Horizontal, szName, pParent );
    builder.AddField<ADN_EditLine_Int>( pGroupBox, firstThresholdName, thresholds.firstThreshold_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Int>( pGroupBox, secondThresholdName, thresholds.secondThreshold_, 0, eGreaterZero );
    return pGroupBox;
}
