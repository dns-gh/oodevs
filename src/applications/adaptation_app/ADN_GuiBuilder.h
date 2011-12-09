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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GuiBuilder.h $
// $Author: Ape $
// $Modtime: 14/04/05 17:14 $
// $Revision: 9 $
// $Workfile: ADN_GuiBuilder.h $
//
// *****************************************************************************

#ifndef __ADN_GuiBuilder_h_
#define __ADN_GuiBuilder_h_

#include <boost/noncopyable.hpp>
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_CheckBox.h"
#include "ADN_TimeField.h"
#include "ADN_Validator.h"

class ADN_FileChooser;
class ADN_Table;
class ADN_Gfx_ABC;

enum E_Validator
{
    eNone,
    eGreaterZero,
    eGreaterEqualZero,
    eLowerZero,
    eLowerEqualZero,
    ePercentage,
    eZeroOne,
    eDegrees,
    eVarName
};


// =============================================================================
/** @class  ADN_GuiBuilder
    @brief  ADN_GuiBuilder
*/
// Created: APE 2005-03-11
// =============================================================================
class ADN_GuiBuilder : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_GuiBuilder();
    virtual ~ADN_GuiBuilder();
    //@}

    //! @name Field Operations
    //@{
    QWidget* AddFieldHolder( QWidget* pParent );

    template< class T >
        T* AddField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit = 0, E_Validator nValidator = eNone );

    template< class T >
        T* AddField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit = 0, E_Validator nValidator = eNone );

    template< class T >
        T* AddOptionnalField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiOptionConnector, ADN_Connector_ABC*& pGuiConnector, const char* szUnit = 0, E_Validator nValidator = eNone );

    template< class T >
        T* AddOptionnalField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemOptionConnector, ADN_Connector_ABC& itemConnector, const char* szUnit = 0, E_Validator nValidator = eNone );

    template< typename T >
        ADN_ComboBox_Enum<T>* AddEnumField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, typename ADN_ComboBox_Enum<T>::T_Converter converter );

    template< typename T >
        ADN_ComboBox_Enum<T>* AddEnumField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemConnector, typename ADN_ComboBox_Enum<T>::T_Converter converter );


    ADN_FileChooser* AddFileField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szFilter = "(*.*)" );
    ADN_FileChooser* AddFileField( QWidget* pParent, const char* szName, ADN_Type_String& strFileNameConnector, const char* szFilter = "(*.*)" );

    void AddStretcher( QWidget* pParent, Qt::Orientation nOrientation );
    void AddStretcher( QLayout* pLayout, Qt::Orientation nOrientation );

    void SetEnabled( bool bEnabled );
    void SetToolTip( const char* szToolTip );
    void SetValidator( QValidator* pValidator );
    //@}

    //! @name Table Operations
    //@{
    ADN_Table* CreateTable( QWidget* pParent );

    template< class T >
        T* AddTableCell( ADN_Table* pParent, void* pData, int nRow, int nCol, ADN_Connector_ABC& itemConnector, E_Validator nValidator = eNone, Q3TableItem::EditType nEditType = Q3TableItem::OnTyping );

    template< class T >
        T* AddTableCell( ADN_Table* pParent, void* pData, int nRow, int nCol, int nRowSpan, int nColSpan, ADN_Connector_ABC& itemConnector, E_Validator nValidator = eNone, Q3TableItem::EditType nEditType = Q3TableItem::OnTyping );

    Q3TableItem* AddTableCell( ADN_Table* pTable, int nRow, int nCol, const char* strText = 0, Q3TableItem::EditType nEditType = Q3TableItem::Never );
    Q3TableItem* AddTableCell( ADN_Table* pTable, int nRow, int nCol, int nRowSpan, int nColSpan, const char* strText = 0, Q3TableItem::EditType nEditType = Q3TableItem::Never );
    //@}

private:
    //! @name Helpers
    //@{
    void DoFieldLayout( QWidget* pParent, QWidget* pWidget1, QWidget* pWidget2, QWidget* pWidget3 );
    template< class T >
        void DoValidator( T* pWidget, E_Validator nValidator );
    template< class T >
        void SetValidator( T& validator, E_Validator nValidator );
    //@}

private:
    //! @name Member data
    //@{
    QWidget* pCurrentFieldWidget1_;
    QWidget* pCurrentFieldWidget2_;  //!< Same object as below
    ADN_Gfx_ABC* pCurrentFieldGfx2_; //!< Same object as above
    QWidget* pCurrentFieldWidget3_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    QLabel* pUnitLabel = 0;
    if( szUnit != 0 )
        pUnitLabel = new QLabel( szUnit, pParent );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = pUnitLabel;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, pUnitLabel );
    this->DoValidator( pField, nValidator );

    pGuiConnector = &pField->GetConnector();
    return pField;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    QLabel* pUnitLabel = 0;
    if( szUnit != 0 )
        pUnitLabel = new QLabel( szUnit, pParent );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = pUnitLabel;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, pUnitLabel );
    this->DoValidator( pField, nValidator );

    ADN_Connector_ABC* pConnector = &pField->GetConnector();
    assert( pConnector );
    pConnector->Connect( &itemConnector );
    return pField;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddOptionnalField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddOptionnalField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiOptionConnector, ADN_Connector_ABC*& pGuiConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    ADN_CheckBox* pCheckbox = new ADN_CheckBox( szName, pParent );
    T* pField = new T( pParent );
    pField->SetAutoEnabled( false );
    QLabel* pUnitLabel = 0;
    if( szUnit != 0 )
        pUnitLabel = new QLabel( szUnit, pParent );

    pCurrentFieldWidget1_ = pCheckbox;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = pUnitLabel;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pCheckbox, pField, pUnitLabel );
    this->DoValidator( pField, nValidator );

    pGuiOptionConnector = &pCheckbox->GetConnector();
    pGuiConnector = &pField->GetConnector();
    qApp->connect( pCheckbox, SIGNAL( toggled( bool ) ), pField, SLOT( setEnabled( bool ) ) );

    return pField;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddOptionnalField
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_GuiBuilder::AddOptionnalField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemOptionConnector, ADN_Connector_ABC& itemConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    ADN_CheckBox* pCheckbox = new ADN_CheckBox( szName, pParent );
    T* pField = new T( pParent );
    pField->SetAutoEnabled( false );
    QLabel* pUnitLabel = 0;
    if( szUnit != 0 )
        pUnitLabel = new QLabel( szUnit, pParent );

    pCurrentFieldWidget1_ = pCheckbox;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = pUnitLabel;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pCheckbox, pField, pUnitLabel );
    this->DoValidator( pField, nValidator );

    pCheckbox->GetConnector().Connect( &itemOptionConnector );
    pField->GetConnector().Connect( &itemConnector );
    qApp->connect( pCheckbox, SIGNAL( toggled( bool ) ), pField, SLOT( setEnabled( bool ) ) );

    return pField;
}



// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddEnumField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< typename T >
ADN_ComboBox_Enum<T>* ADN_GuiBuilder::AddEnumField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, typename ADN_ComboBox_Enum<T>::T_Converter converter )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_ComboBox_Enum<T>* pField = new ADN_ComboBox_Enum<T>( converter, pParent );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = 0;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, 0 );
    pGuiConnector = &pField->GetConnector();
    return pField;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddEnumField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< typename T >
ADN_ComboBox_Enum<T>* ADN_GuiBuilder::AddEnumField( QWidget* pParent, const char* szName, ADN_Connector_ABC& itemConnector, typename ADN_ComboBox_Enum<T>::T_Converter converter )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_ComboBox_Enum<T>* pField = new ADN_ComboBox_Enum<T>( converter, pParent );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = 0;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, 0 );
    pField->GetConnector().Connect( &itemConnector );
    return pField;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddTableCell
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddTableCell( ADN_Table* pParent, void* pData, int nRow, int nCol, ADN_Connector_ABC& itemConnector, E_Validator nValidator, Q3TableItem::EditType nEditType )
{
    T* pItem = new T( pParent, pData, nEditType );
    pParent->setItem( nRow, nCol, pItem );
    pItem->GetConnector().Connect( & itemConnector );
    this->SetValidator( pItem->GetValidator(), nValidator );
    return pItem;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddTableCell
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddTableCell( ADN_Table* pParent, void* pData, int nRow, int nCol, int nRowSpan, int nColSpan, ADN_Connector_ABC& itemConnector, E_Validator nValidator, Q3TableItem::EditType nEditType )
{
    assert( nRowSpan > 0 );
    T* pItem = new T( pParent, pData, nEditType );
    pParent->setItem( nRow, nCol, pItem );
    pItem->setSpan( nRowSpan, nColSpan );
    pItem->GetConnector().Connect( & itemConnector );
    this->SetValidator( pItem->GetValidator(), nValidator );
    return pItem;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template< class T >
void ADN_GuiBuilder::SetValidator( T& /*validator*/, E_Validator nValidator )
{
    assert( nValidator == eNone );
}



// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::SetValidator( ADN_IntValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case eGreaterZero:
        validator.setRange( 1, INT_MAX );
        break;
    case eGreaterEqualZero:
        validator.setRange( 0, INT_MAX );
        break;
    case eLowerZero:
        validator.setRange( INT_MIN, -1 );
        break;
    case eLowerEqualZero:
        validator.setRange( INT_MIN, 0 );
        break;
    case ePercentage:
        validator.setRange( 0, 100 );
        break;
    default:
        assert( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::SetValidator( ADN_DoubleValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case eGreaterZero:
        validator.setRange( 0.001, INT_MAX, 3 );
        break;
    case eGreaterEqualZero:
        validator.setRange( 0, INT_MAX, 3 );
        break;
    case eLowerZero:
        validator.setRange( INT_MIN, -0.001 );
        break;
    case eLowerEqualZero:
        validator.setRange( INT_MIN, 0 );
        break;
    case ePercentage:
        validator.setRange( 0, 100, 2 );
        break;
    case eZeroOne:
        validator.setRange( 0, 1, 3 );
        break;
    case eDegrees:
        validator.setRange( 0, 360, 2 );
        break;
    default:
        assert( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::SetValidator( ADN_PercentageValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case ePercentage:
        validator.setRange( 0, 100, 2 );
        break;
    case eGreaterZero:
    case eGreaterEqualZero:
    case eLowerZero:
    case eLowerEqualZero:
    case eZeroOne:
    case eDegrees:
    default:
        assert( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::SetValidator( QRegExpValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    assert( nValidator == eVarName );
    validator.setRegExp( QRegExp( "([A-Z]|[a-z]|[0-9])*" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template< class T >
void ADN_GuiBuilder::DoValidator( T* /*pWidget*/, E_Validator /*nValidator*/ )
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::DoValidator< ADN_EditLine_String >( ADN_EditLine_String* pWidget, E_Validator nValidator )
{
    SetValidator( pWidget->GetValidator(), nValidator );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::DoValidator< ADN_EditLine_Int >( ADN_EditLine_Int* pWidget, E_Validator nValidator )
{
    SetValidator( pWidget->GetValidator(), nValidator );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::DoValidator< ADN_EditLine_Double >( ADN_EditLine_Double* pWidget, E_Validator nValidator )
{
    SetValidator( pWidget->GetValidator(), nValidator );
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::DoValidator< ADN_TimeField >( ADN_TimeField* pWidget, E_Validator nValidator )
{
    SetValidator( pWidget->GetValidator(), nValidator );
}

#endif // __ADN_GuiBuilder_h_
