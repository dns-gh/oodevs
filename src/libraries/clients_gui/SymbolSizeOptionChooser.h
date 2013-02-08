// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SymbolSizeOptionChooser_h_
#define __SymbolSizeOptionChooser_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class StaticModel;

// =============================================================================
/** @class  SymbolSizeOptionChooser
    @brief  SymbolSizeOptionChooser
*/
// Created: MMC 2013-02-01
// =============================================================================
class SymbolSizeOptionChooser : public QObject
                              , public tools::Observer_ABC
                              , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SymbolSizeOptionChooser( QWidget* parent, QToolBar* toolBar, QPixmap increaseIcon, QPixmap decreaseIcon, kernel::Controllers& controllers );
    virtual ~SymbolSizeOptionChooser();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnIncreaseSymbolUnit();
    void OnDecreaseSymbolUnit();
    //@}

private:
    //! @name Helpers
    //@{
    void CheckButtonsEnable( float value );
    //@}

private:
    //! @name Member data
    //@{
    QToolButton* increaseButton_;
    QToolButton* decreaseButton_;
    kernel::Controllers& controllers_;
    const float min_;
    const float max_;
    const float default_;
    const float increment_;
    const std::string name_;
    //@}
};

#endif // __SymbolSizeOptionChooser_h_
