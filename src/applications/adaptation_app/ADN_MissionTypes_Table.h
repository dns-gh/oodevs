// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MissionTypes_Table_h_
#define __ADN_MissionTypes_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_MissionTypes_Table
    @brief  ADN_MissionTypes_Table
*/
// Created: LDC 2010-08-19
// =============================================================================
class ADN_MissionTypes_Table : public ADN_Table2
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionTypes_Table( QWidget* pParent, const char* szName );
    virtual ~ADN_MissionTypes_Table();
    //@}

public:
    //! @name Operations
    //@{
    virtual void FinalizeConnection();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_MissionTypes_Table( const ADN_MissionTypes_Table& );            //!< Copy constructor
    ADN_MissionTypes_Table& operator=( const ADN_MissionTypes_Table& ); //!< Assignment operator
    //@}
};

#endif // __ADN_MissionTypes_Table_h_
