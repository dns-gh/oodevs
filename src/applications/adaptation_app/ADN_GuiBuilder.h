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
#include "ADN_GroupBox.h"
#include "ADN_SearchListView.h"

class ADN_FileChooser;
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
    eDIAType
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
    explicit ADN_GuiBuilder( const QString& name );
    virtual ~ADN_GuiBuilder();
    //@}

    //! @name Name operations
    //@{
    const QString& GetName() const;
    QString GetChildName( QString childName ) const;

    void PushSubName( QString subName );
    void PopSubName();
    //@}

    //! @name Field operations
    //@{
    QWidget* AddFieldHolder( QWidget* pParent );

    typedef std::vector< QWidget* > T_optionalWidgets;

    // $$$$ ABR 2013-02-07: TODO Change ADN_Table constructor (remove objectName parameter) so it can use the following helpers
    template< class T >
    T* AddWidget( const char* objectName );
    template< class T, class Param1 >
    T* AddWidget( const char* objectName, Param1 param1 );
    template< class T, class Param1, class Param2 >
    T* AddWidget( const char* objectName, Param1 param1, Param2 param2 );
    template< class T, class Param1, class Param2, class Param3 >
    T* AddWidget( const char* objectName, Param1 param1, Param2 param2, Param3 param3 );

    ADN_GroupBox* AddGroupBox( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, int strips = -1, Qt::Orientation orientation = Qt::Horizontal );
    ADN_GroupBox* AddGroupBox( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, int strips = -1, Qt::Orientation orientation = Qt::Horizontal );

    template< class T >
    ADN_SearchListView* AddSearchListView( ADN_GUI_ABC* gui, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );
    template< class T, class Param1 >
    ADN_SearchListView* AddSearchListView( ADN_GUI_ABC* gui, Param1& param1, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );
    template< class T, class Param1, class Param2 >
    ADN_SearchListView* AddSearchListView( ADN_GUI_ABC* gui, Param1& param1, Param2& param2, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );

    template< class T >
    T* AddField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit = 0, E_Validator nValidator = eNone, QWidget* sideWidget = 0 );
    template< class T >
    T* AddField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit = 0, E_Validator nValidator = eNone, QWidget* sideWidget = 0 );

    template< class T >
    T* AddOptionalField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, T_optionalWidgets& optionalFields, const char* szUnit = 0, E_Validator nValidator = eNone, QWidget* sideWidget = 0 );
    template< class T >
    T* AddOptionalField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, T_optionalWidgets& optionalFields, const char* szUnit = 0, E_Validator nValidator = eNone, QWidget* sideWidget = 0 );

    template< class T >
    T* AddCheckableField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiOptionConnector, ADN_Connector_ABC*& pGuiConnector, const char* szUnit = 0, E_Validator nValidator = eNone );
    template< class T >
    T* AddCheckableField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemOptionConnector, ADN_Connector_ABC& itemConnector, const char* szUnit = 0, E_Validator nValidator = eNone );

    ADN_ComboBox_Enum* AddEnumField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector );
    ADN_ComboBox_Enum* AddEnumField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector );

    ADN_FileChooser* AddFileField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szFilter = "(*.*)" );

    ADN_EditLine_ABC* AddLocalizedField( ADN_Ref_ABC& ref, QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit = 0, E_Validator nValidator = eNone );
    ADN_EditLine_ABC* AddLocalizedField( ADN_Ref_ABC& ref, QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit = 0, E_Validator nValidator = eNone );
    ADN_EditLine_ABC* AddLocalizedOptionalField( ADN_Ref_ABC& ref, QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, T_optionalWidgets& optionalFields, const char* szUnit = 0, E_Validator nValidator = eNone );
    ADN_EditLine_ABC* AddLocalizedOptionalField( ADN_Ref_ABC& ref, QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, T_optionalWidgets& optionalFields, const char* szUnit = 0, E_Validator nValidator = eNone );

    void AddStretcher( QWidget* pParent, Qt::Orientation nOrientation );
    void AddStretcher( QLayout* pLayout, Qt::Orientation nOrientation );

    void SetEnabled( bool bEnabled );
    void SetToolTip( const char* szToolTip );
    void SetValidator( QValidator* pValidator );
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
    QString name_;
    int nameCount_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddWidget
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddWidget( const char* objectName )
{
    T* pField = new T();
    pField->setObjectName( GetChildName( objectName ) );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddWidget
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
template< class T, class Param1 >
T* ADN_GuiBuilder::AddWidget( const char* objectName, Param1 param1 )
{
    T* pField = new T( param1 );
    pField->setObjectName( GetChildName( objectName ) );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddWidget
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
template< class T, class Param1, class Param2 >
T* ADN_GuiBuilder::AddWidget( const char* objectName, Param1 param1, Param2 param2 )
{
    T* pField = new T( param1, param2 );
    pField->setObjectName( GetChildName( objectName ) );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddWidget
// Created: ABR 2013-02-05
// -----------------------------------------------------------------------------
template< class T, class Param1, class Param2, class Param3 >
T* ADN_GuiBuilder::AddWidget( const char* objectName, Param1 param1, Param2 param2, Param3 param3 )
{
    T* pField = new T( param1, param2, param3 );
    pField->setObjectName( GetChildName( objectName ) );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddSearchListView
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
template< class T >
ADN_SearchListView* ADN_GuiBuilder::AddSearchListView( ADN_GUI_ABC* gui, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /* = 0 */ )
{
    T* pListView = new T( parent );
    ADN_SearchListView* searchListView = new ADN_SearchListView( GetChildName( "list" ), gui, *pListView, reference, connector, subTab, parent );
    gui->AddListView( pListView );
    return searchListView;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddSearchListView
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
template< class T, class Param1 >
ADN_SearchListView* ADN_GuiBuilder::AddSearchListView( ADN_GUI_ABC* gui, Param1& param1, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /* = 0 */ )
{
    T* pListView = new T( parent, param1 );
    ADN_SearchListView* searchListView = new ADN_SearchListView( GetChildName( "list" ), gui, *pListView, reference, connector, subTab, parent );
    gui->AddListView( pListView );
    return searchListView;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddSearchListView
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
template< class T, class Param1, class Param2 >
ADN_SearchListView* ADN_GuiBuilder::AddSearchListView( ADN_GUI_ABC* gui, Param1& param1, Param2& param2, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /* = 0 */ )
{
    T* pListView = new T( parent, param1, param2 );
    ADN_SearchListView* searchListView = new ADN_SearchListView( GetChildName( "list" ), gui, *pListView, reference, connector, subTab, parent );
    gui->AddListView( pListView );
    return searchListView;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szUnit, E_Validator nValidator, QWidget* sideWidget )
{
    assert( szUnit == 0 || sideWidget == 0 );
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    pField->setObjectName( GetChildName( objectName ) );
    QWidget* thirdWidget = 0;
    if( szUnit != 0 )
        thirdWidget = new QLabel( szUnit, pParent );
    else if( sideWidget != 0 )
        thirdWidget = sideWidget;

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = thirdWidget;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, thirdWidget );
    this->DoValidator( pField, nValidator );

    pGuiConnector = &pField->GetConnector();
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, const char* szUnit, E_Validator nValidator, QWidget* sideWidget )
{
    assert( szUnit == 0 || sideWidget == 0 );
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    pField->setObjectName( GetChildName( objectName ) );
    QWidget* thirdWidget = 0;
    if( szUnit != 0 )
        thirdWidget = new QLabel( szUnit, pParent );
    else if( sideWidget != 0 )
        thirdWidget = sideWidget;

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = thirdWidget;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, thirdWidget );
    this->DoValidator( pField, nValidator );

    ADN_Connector_ABC* pConnector = &pField->GetConnector();
    assert( pConnector );
    pConnector->Connect( &itemConnector );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddOptionalField
// Created: NPT 2013-04-15
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddOptionalField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector, T_optionalWidgets& optionalFields, const char* szUnit /*= 0*/, E_Validator nValidator /*= eNone*/, QWidget* sideWidget /*= 0*/ )
{
    assert( szUnit == 0 || sideWidget == 0 );
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    pField->setObjectName( GetChildName( objectName ) );
    QWidget* thirdWidget = 0;
    if( szUnit != 0 )
        thirdWidget = new QLabel( szUnit, pParent );
    else if( sideWidget != 0 )
        thirdWidget = sideWidget;

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = thirdWidget;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, thirdWidget );
    this->DoValidator( pField, nValidator );

    pGuiConnector = &pField->GetConnector();
    optionalFields.push_back( pNameLabel );
    optionalFields.push_back( pField );
    return pField;
}
// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddOptionalField
// Created: NPT 2013-04-15
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddOptionalField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector, T_optionalWidgets& optionalFields, const char* szUnit /*= 0*/, E_Validator nValidator /*= eNone*/, QWidget* sideWidget /*= 0*/ )
{
    assert( szUnit == 0 || sideWidget == 0 );
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    T* pField = new T( pParent );
    pField->setObjectName( GetChildName( objectName ) );
    QWidget* thirdWidget = 0;
    if( szUnit != 0 )
        thirdWidget = new QLabel( szUnit, pParent );
    else if( sideWidget != 0 )
        thirdWidget = sideWidget;

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pField;
    pCurrentFieldGfx2_ = pField;
    pCurrentFieldWidget3_ = thirdWidget;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pField, thirdWidget );
    this->DoValidator( pField, nValidator );

    ADN_Connector_ABC* pConnector = &pField->GetConnector();
    assert( pConnector );
    pConnector->Connect( &itemConnector );
    optionalFields.push_back( pNameLabel );
    optionalFields.push_back( pField );
    return pField;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddCheckableField
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
template< class T >
T* ADN_GuiBuilder::AddCheckableField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiOptionConnector, ADN_Connector_ABC*& pGuiConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    ADN_CheckBox* pCheckbox = new ADN_CheckBox( szName, pParent );
    pCheckbox->setObjectName( GetChildName( std::string( std::string( "has-" ) + objectName ).c_str() ) );
    T* pField = new T( pParent );
    pField->setObjectName( GetChildName( objectName ) );
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
// Name: ADN_GuiBuilder::AddCheckableField
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_GuiBuilder::AddCheckableField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemOptionConnector, ADN_Connector_ABC& itemConnector, const char* szUnit, E_Validator nValidator )
{
    // Create the field and labels.
    ADN_CheckBox* pCheckbox = new ADN_CheckBox( szName, pParent );
    pCheckbox->setObjectName( GetChildName( std::string( std::string( "has-" ) + objectName ).c_str() ) );
    T* pField = new T( pParent );
    pField->SetAutoEnabled( false );
    pField->setObjectName( GetChildName( objectName ) );
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
inline
ADN_ComboBox_Enum* ADN_GuiBuilder::AddEnumField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC*& pGuiConnector )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_ComboBox_Enum* pField = new ADN_ComboBox_Enum( pParent );
    pField->setObjectName( GetChildName( objectName ) );

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
inline
ADN_ComboBox_Enum* ADN_GuiBuilder::AddEnumField( QWidget* pParent, const char* objectName, const char* szName, ADN_Connector_ABC& itemConnector )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_ComboBox_Enum* pField = new ADN_ComboBox_Enum( pParent );
    pField->setObjectName( GetChildName( objectName ) );

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
// Name: ADN_GuiBuilder::AddValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template< class T >
void ADN_GuiBuilder::SetValidator( T& /*validator*/, E_Validator /*nValidator*/ )
{
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
    static int decimals = 10;

    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case eGreaterZero:
        validator.setRange( 0.001, INT_MAX, decimals );
        break;
    case eGreaterEqualZero:
        validator.setRange( 0, INT_MAX, decimals );
        break;
    case eLowerZero:
        validator.setRange( INT_MIN, -0.001, decimals );
        break;
    case eLowerEqualZero:
        validator.setRange( INT_MIN, 0, decimals );
        break;
    case ePercentage:
        validator.setRange( 0, 100, decimals );
        break;
    case eZeroOne:
        validator.setRange( 0, 1, decimals );
        break;
    case eDegrees:
        validator.setRange( 0, 360, decimals );
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
void ADN_GuiBuilder::SetValidator( ADN_DoublePercentageValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case ePercentage:
        validator.setRange( 0, 100, 10 );
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
// Name: ADN_GuiBuilder::AddValidator
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
template<>
inline
void ADN_GuiBuilder::SetValidator( ADN_IntPercentageValidator& validator, E_Validator nValidator )
{
    if( nValidator == eNone )
        return;

    switch( nValidator )
    {
    case ePercentage:
        validator.setRange( 0, 100 );
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
    assert( nValidator == eDIAType );
    validator.setRegExp( QRegExp( "([ -~])*" ) ); // $$$$ 2013-09-19: ASCII printable characters
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
void ADN_GuiBuilder::DoValidator< ADN_EditLine_LocalizedString >( ADN_EditLine_LocalizedString* pWidget, E_Validator nValidator )
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
void ADN_GuiBuilder::DoValidator< ADN_PH_EditLine_Double >( ADN_PH_EditLine_Double* pWidget, E_Validator nValidator )
{
    SetValidator( pWidget->GetValidator(), nValidator );
}

#endif // __ADN_GuiBuilder_h_
