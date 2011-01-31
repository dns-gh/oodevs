// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Consumptions_Table_h_
#define __ADN_Consumptions_Table_h_

#include "ADN_CommonGfx.h"
#include "ADN_ResourceNetworks_Data.h"

// =============================================================================
/** @class  ADN_Consumptions_Table
    @brief  ADN_Consumptions_Table
*/
// Created: JSR 2011-01-31
// =============================================================================
class ADN_Consumptions_Table
    : public ADN_Table2
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Consumptions_Table( QWidget* parent = 0 );
    virtual ~ADN_Consumptions_Table();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int row, int col, const QPoint& pt );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_Consumptions_Table( const ADN_Consumptions_Table& );            //!< Copy constructor
    ADN_Consumptions_Table& operator=( const ADN_Consumptions_Table& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateNewConsumption( int resource );
    void RemoveCurrentConsumption();
    bool Contains( const ADN_ResourceNetworks_Data::ResourceNetworkInfos& infos ) const;
    //@}
};

#endif // __ADN_Consumptions_Table_h_
