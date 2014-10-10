// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ButtonGroup_h_
#define __ButtonGroup_h_

#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  ButtonGroup
    @brief  ButtonGroup
*/
// Created: RPD 2008-08-21
// =============================================================================
class ButtonGroup : public RichWidget< QGroupBox >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ButtonGroup( const QString& objectName,
                          QWidget* parent = 0 );
    virtual ~ButtonGroup();
    //@}

    //! @name Operations
    //@{
    void AddButton( int id, QAbstractButton* button );
    void SetChecked( int id );

    int CheckedId() const;
    QAbstractButton* CheckedButton() const;
    //@}

signals:
    //! @name Signals
    //@{
    void ButtonClicked( int );
    void ButtonClicked( QAbstractButton* );
    //@}

private:
    //! @name Member data
    //@{
    QButtonGroup* buttonGroup_;
    QHBoxLayout* layout_;
    //@}
};

}

#endif // __ButtonGroup_h_
