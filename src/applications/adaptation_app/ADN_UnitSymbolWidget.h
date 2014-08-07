// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_UnitSymbolWidget_h_
#define __ADN_UnitSymbolWidget_h_

#include "ADN_SymbolWidget_ABC.h"
#include "ADN_Symbols_Data.h"
#include <geometry/Types.h>

// =============================================================================
/** @class  ADN_UnitSymbolWidget
    @brief  ADN_UnitSymbolWidget
*/
// Created: MMC 2013-04-09
// =============================================================================
class ADN_UnitSymbolWidget : public ADN_SymbolWidget_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
   typedef std::function< const QPixmap& ( ADN_Symbols_Data::SymbolsUnit* ) > T_PixmapExtractor;
   typedef std::function< const std::string& ( ADN_Symbols_Data::SymbolsUnit* ) > T_StringExtractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_UnitSymbolWidget( QWidget* parent, T_PixmapExtractor pixmapAccessor, T_StringExtractor symbolAccessor = &ADN_Symbols_Data::SymbolsUnit::GetSymbol );
    virtual ~ADN_UnitSymbolWidget();
    //@}

signals:
    //! @name Signals
    //@{
    void SymbolChanged( const QString& );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnNatureChanged( const QString& );
    //@}

private :
    //! @name Member data
    //@{
    T_PixmapExtractor pixmapAccessor_;
    T_StringExtractor symbolAccessor_;
    //@}
};

#endif // __ADN_UnitSymbolWidget_h_
