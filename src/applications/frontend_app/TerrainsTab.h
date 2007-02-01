// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainsTab_h_
#define __TerrainsTab_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  TerrainsTab
    @brief  TerrainsTab
*/
// Created: SBO 2007-02-01
// =============================================================================
class TerrainsTab : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainsTab( QWidget* parent, kernel::Controllers& controllers );
    virtual ~TerrainsTab();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainsTab( const TerrainsTab& );            //!< Copy constructor
    TerrainsTab& operator=( const TerrainsTab& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __TerrainsTab_h_
