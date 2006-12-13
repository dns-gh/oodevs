// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MissionParameterValues_Table_h_
#define __ADN_MissionParameterValues_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_MissionParameterValues_Table
    @brief  ADN_MissionParameterValues_Table
*/
// Created: SBO 2006-12-05
// =============================================================================
class ADN_MissionParameterValues_Table : public ADN_Table2
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameterValues_Table( QWidget* pParent, const char* szName );
    virtual ~ADN_MissionParameterValues_Table();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_MissionParameterValues_Table( const ADN_MissionParameterValues_Table& );            //!< Copy constructor
    ADN_MissionParameterValues_Table& operator=( const ADN_MissionParameterValues_Table& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddNewElement();
    void RemoveCurrentElement();
    virtual void OnContextMenu( int row, int col, const QPoint& pt );
    //@}
};

#endif // __ADN_MissionParameterValues_Table_h_
