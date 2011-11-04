// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitsFilter_h_
#define __UnitsFilter_h_

#include "ADN_ComboBox_Vector.h"
#include "ADN_Units_Data.h"
#include <set>

// =============================================================================
/** @class  UnitsFilter
    @brief  Units filter
*/
// Created: LGY 2011-11-03
// =============================================================================
class UnitsFilter : public ADN_ComboBox_Vector< ADN_Units_Data::UnitInfos >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitsFilter( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~UnitsFilter();
    //@}

    //! @name Operations
    //@{
    virtual void insertItem( ADN_ComboBoxItem* item, int index = -1 );
    virtual void SetCurrentData( void *data );

    void Add( const std::string& name );
    void Remove( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UnitsFilter( const UnitsFilter& );            //!< Copy constructor
    UnitsFilter& operator=( const UnitsFilter& ); //!< Assignment operator
    //@}

private:
    //! @name Member Data
    //@{
    std::set< std::string > units_;
    //@}
};

#endif // __UnitsFilter_h_
