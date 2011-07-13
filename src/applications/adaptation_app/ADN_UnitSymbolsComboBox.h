// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_UnitSymbolsComboBox_h_
#define __ADN_UnitSymbolsComboBox_h_

#include "ADN_ComboBox_Drawings.h"
#include "ADN_UnitSymbols_Data.h"

// =============================================================================
/** @class  ADN_UnitSymbolsComboBox
@brief  ADN_UnitSymbolsComboBox
*/
// Created: MMC 2011-07-07
// =============================================================================
class ADN_UnitSymbolsComboBox
    : public ADN_ComboBox_Drawings< ADN_UnitSymbols_Data::UnitSymbolInfo >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    ADN_UnitSymbolsComboBox( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_UnitSymbolsComboBox();
    //@}

signals:
    //! @name Signals
    //@{
    void UnitSymbolChanged( const QString& symbol );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnNatureChanged( const QString& nature );
    void OnSelectionChanged( int index );
    //@}
};

#endif // __ADN_UnitSymbolsComboBox_h_