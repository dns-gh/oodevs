// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StockPrototype_ABC_h_
#define __StockPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Resolver_ABC.h"
#include "ValuedComboBox.h"
#include <qvgroupbox.h>

namespace kernel
{
    class DotationType;
}

namespace gui
{
    class RichLabel;
    class LoadableSpinBox;

// =============================================================================
/** @class  StockPrototype_ABC
    @brief  Base GUI class to create stock object
*/
// Created: BCI 2011-02-04
// =============================================================================
class StockPrototype_ABC : public ObjectAttributePrototype_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    StockPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver );
    virtual ~StockPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

private slots:
    void dotationCountChanged( int );

private:
    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:

    struct DotationWidget : public QVGroupBox
    {
        DotationWidget( QWidget* parent )
            : QVGroupBox( parent )
        {}
        RichLabel* dotationLabel_;
        ValuedComboBox< const kernel::DotationType* >* dotationType_;
        LoadableSpinBox* stock_;
        RichLabel* maxStockLabel_;
        LoadableSpinBox* maxStock_;
    };

    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver_;
    std::vector< DotationWidget* > dotationWidgets_;
    //@}
};

}

#endif // __StockPrototype_ABC_h_
