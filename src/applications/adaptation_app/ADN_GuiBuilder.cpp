// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GuiBuilder.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 14:49 $
// $Revision: 8 $
// $Workfile: ADN_GuiBuilder.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ApplyButton.h"
#include "ADN_EditLine_ABC.h"
#include "ADN_Type_ABC.h"
#include "ADN_FileChooser.h"
#include "ADN_Project_Data.h"
#include "ADN_Gfx_ABC.h"
#include "ADN_GoToButton.h"
#include "ADN_Validator.h"

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder constructor
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_GuiBuilder::ADN_GuiBuilder( const QString& name )
    : pCurrentFieldWidget1_ ( 0 )
    , pCurrentFieldWidget2_ ( 0 )
    , pCurrentFieldGfx2_    ( 0 )
    , pCurrentFieldWidget3_ ( 0 )
    , name_                 ( name )
    , nameCount_            ( 0 )
{
    assert( !name_.isEmpty() );
    gui::ObjectNameManager::getInstance()->AddSubLevel( name );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder destructor
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_GuiBuilder::~ADN_GuiBuilder()
{
    gui::ObjectNameManager::getInstance()->RemoveSubLevel();
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddFieldHolder
// Created: APE 2005-03-23
// -----------------------------------------------------------------------------
QWidget* ADN_GuiBuilder::AddFieldHolder( QWidget* pParent )
{
    QWidget* pHolder = new QWidget( pParent );
    new Q3GridLayout( pHolder, 0, 3, 5, 10 );
    return pHolder;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddFileField
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
ADN_FileChooser* ADN_GuiBuilder::AddFileField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szFilter /* = "(*.*)"*/ )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_FileChooser* pChooser = new ADN_FileChooser( pParent, szFilter );
    pChooser->setObjectName( GetChildName( objectName ) );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pChooser;
    pCurrentFieldGfx2_ = 0;
    pCurrentFieldWidget3_ = 0;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pChooser, 0 );

    pGuiConnector = &pChooser->GetConnector(ADN_FileChooser::eFile);
    pChooser->GetConnector(ADN_FileChooser::eDirectory).Connect( &ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory() );
    return pChooser;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetToolTip
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetToolTip( const char* szToolTip )
{
    if( pCurrentFieldWidget1_ != 0 )
        QToolTip::add( pCurrentFieldWidget1_, szToolTip );
    if( pCurrentFieldWidget2_ != 0 )
        QToolTip::add( pCurrentFieldWidget2_, szToolTip );
    if( pCurrentFieldWidget3_ != 0 && !dynamic_cast< ADN_GoToButton* >( pCurrentFieldWidget3_ ) )
        QToolTip::add( pCurrentFieldWidget3_, szToolTip );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetValidator
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetValidator( QValidator* pValidator )
{
    if( pCurrentFieldWidget2_ == 0 || ! pCurrentFieldWidget2_->inherits( "QLineEdit" ) )
        return;

    ((QLineEdit*)pCurrentFieldWidget2_)->setValidator( pValidator );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetEnabled
// Created: APE 2005-04-05
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetEnabled( bool bEnabled )
{
    if( pCurrentFieldWidget2_ == 0 || pCurrentFieldGfx2_ == 0 )
        return;

    // Expects that the previously created field inherited from ADN_Gfx_ABC
    pCurrentFieldGfx2_->SetAutoEnabled( bEnabled );
    pCurrentFieldWidget2_->setEnabled( bEnabled );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoFieldLayout
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::DoFieldLayout( QWidget* pParent, QWidget* pWidget1, QWidget* pWidget2, QWidget* pWidget3 )
{
    // Lay them out if necessary.
    assert( pParent->layout() != 0 );
    if( pParent->layout()->inherits( "QGridLayout" ) )
    {
        Q3GridLayout* pLayout = (Q3GridLayout*)pParent->layout();
        int nRow = pLayout->numRows();
        pLayout->setColStretch( 0, 0 );
        pLayout->setColStretch( 1, 500 );
        pLayout->setColStretch( 2, 0 );
        if( pWidget3 != 0 )
        {
            pLayout->addWidget( pWidget1, nRow, 0 );
            pLayout->addWidget( pWidget2, nRow, 1 );
            pLayout->addWidget( pWidget3, nRow, 2 );
        }
        else
        {
            pLayout->addWidget( pWidget1, nRow, 0 );
            pLayout->addMultiCellWidget( pWidget2, nRow, nRow, 1, 2 );
        }
    }
    else if( pParent->inherits( "QGroupBox" ) )
    {
        if( pWidget3 == 0 )
            ((Q3GroupBox*)pParent)->addSpace( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddStretcher
// Created: APE 2005-03-25
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::AddStretcher( QWidget* pParent, Qt::Orientation nOrientation )
{
    QSpacerItem* pSpacer = new QSpacerItem( 1, 1, (nOrientation == Qt::Vertical) ? QSizePolicy::Minimum : QSizePolicy::Expanding, (nOrientation == Qt::Vertical) ? QSizePolicy::Expanding : QSizePolicy::Minimum );
    pParent->layout()->addItem( pSpacer );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddStretcher
// Created: APE 2005-03-25
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::AddStretcher( QLayout* pLayout, Qt::Orientation nOrientation )
{
    QSpacerItem* pSpacer = new QSpacerItem( 1, 1, (nOrientation == Qt::Vertical) ? QSizePolicy::Minimum : QSizePolicy::Expanding, (nOrientation == Qt::Vertical) ? QSizePolicy::Expanding : QSizePolicy::Minimum );
    pLayout->addItem( pSpacer );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddGroupBox
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
ADN_GroupBox* ADN_GuiBuilder::AddGroupBox( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, int strips /* = -1 */, Qt::Orientation orientation /* = Qt::Horizontal */ )
{
    ADN_GroupBox* groupBox = 0;
    if( strips > 0 )
        groupBox = new ADN_GroupBox( strips, orientation, szName, pParent );
    else
        groupBox = new ADN_GroupBox( szName, pParent );

    groupBox->setObjectName( GetChildName( objectName ) );
    pGuiConnector = &groupBox->GetConnector();
    return groupBox;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddGroupBox
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
ADN_GroupBox* ADN_GuiBuilder::AddGroupBox( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, int strips /* = -1 */, Qt::Orientation orientation /* = Qt::Horizontal */ )
{
    ADN_GroupBox* groupBox = 0;
    if( strips > 0 )
        groupBox = new ADN_GroupBox( strips, orientation, szName, pParent );
    else
        groupBox = new ADN_GroupBox( szName, pParent );

    groupBox->setObjectName( GetChildName( objectName ) );
    ADN_Connector_ABC* pConnector = &groupBox->GetConnector();
    assert( pConnector );
    pConnector->Connect( &itemConnector );
    return groupBox;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::GetName
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
const QString& ADN_GuiBuilder::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::PushSubName
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::PushSubName( QString subName )
{
    assert( !subName.isEmpty() );
    gui::ObjectNameManager::getInstance()->AddSubLevel( subName );
    ++nameCount_;
    name_ += "_";
    name_ += subName;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::PopSubName
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::PopSubName()
{
    if( nameCount_ > 0 )
    {
        --nameCount_;
        int index = name_.lastIndexOf( '_' );
        if( index != -1 )
            name_.truncate( index );
        gui::ObjectNameManager::getInstance()->RemoveSubLevel();
    }
    else
        std::cerr << "ADN_GuiBuilder: Error: The number of pushes ands pops are different: " << name_.toStdString() << std::endl;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::GetChildName
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
QString ADN_GuiBuilder::GetChildName( QString childName ) const
{
    return name_ + "_" + childName;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddLocalizedField
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
ADN_EditLine_ABC* ADN_GuiBuilder::AddLocalizedField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit /*= 0*/, E_Validator nValidator /*= eNone*/ )
{
    ADN_ApplyButton* applyButton = new ADN_ApplyButton();
    ADN_EditLine_ABC* field = AddField< ADN_EditLine_LocalizedString >( pParent, objectName, szName, pGuiConnector, szUnit, nValidator, applyButton );
    applyButton->Connect( field );
    return field;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddLocalizedField
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
ADN_EditLine_ABC* ADN_GuiBuilder::AddLocalizedField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit /*= 0*/, E_Validator nValidator /*= eNone*/ )
{
    ADN_ApplyButton* applyButton = new ADN_ApplyButton();
    ADN_EditLine_ABC* field = AddField< ADN_EditLine_LocalizedString >( pParent, objectName, szName, itemConnector, szUnit, nValidator, applyButton );
    applyButton->Connect( field );
    return field;
}
