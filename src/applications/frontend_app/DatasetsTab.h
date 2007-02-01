// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatasetsTab_h_
#define __DatasetsTab_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  DatasetsTab
    @brief  DatasetsTab
*/
// Created: SBO 2007-02-01
// =============================================================================
class DatasetsTab : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             DatasetsTab( QWidget* parent, kernel::Controllers& controllers );
    virtual ~DatasetsTab();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatasetsTab( const DatasetsTab& );            //!< Copy constructor
    DatasetsTab& operator=( const DatasetsTab& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __DatasetsTab_h_
