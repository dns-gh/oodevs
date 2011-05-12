// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MinePrototype_ABC_h_
#define __MinePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
    class LoadableLineEdit;

// =============================================================================
/** @class  MinePrototype_ABC
    @brief  MinePrototype_ABC
*/
// Created: SBO 2007-02-08
// =============================================================================
class MinePrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MinePrototype_ABC( QWidget* parent );
    virtual ~MinePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    void SetMineField( bool minefield ); // $$$$ SBO 2009-04-03: never called
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableLineEdit* density_;
    QLabel* densityLabel_;
    QHBox* densityBox_;
    //@}
};

}

#endif // __MinePrototype_ABC_h_
