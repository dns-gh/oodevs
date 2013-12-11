// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_SymbolWidget_ABC_h_
#define __ADN_SymbolWidget_ABC_h_

#include "ADN_Gfx_ABC.h"
#undef max

namespace gui
{
    class SvglRenderer;
}

namespace kernel
{
    class SymbolFactory;
}

// =============================================================================
/** @class  ADN_SymbolWidget_ABC
    @brief  ADN_SymbolWidget_ABC
*/
// Created: NLD 2010-12-01
// =============================================================================
class ADN_SymbolWidget_ABC : public QLabel
                       //, public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_SymbolWidget_ABC( QWidget* parent );
    virtual ~ADN_SymbolWidget_ABC();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnNatureChanged( const QString& ) = 0;
    //@}
};

#endif // __ADN_SymbolWidget_ABC_h_
