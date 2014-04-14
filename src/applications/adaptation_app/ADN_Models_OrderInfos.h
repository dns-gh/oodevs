// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ADN_Models_OrderInfos_h
#define ADN_Models_OrderInfos_h

#include "ADN_CrossedRef.h"
#include "ADN_Missions_ABC.h"

class ADN_Missions_FragOrder;

// =============================================================================
/** @class  ADN_Models_OrderInfos
    @brief  ADN_Models_OrderInfos
*/
// Created: ABR 2014-04-10
// =============================================================================
class ADN_Models_OrderInfos : public ADN_CrossedRef< ADN_Missions_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Models_OrderInfos( const ADN_Type_Vector_ABC< ADN_Missions_ABC >& missions, ADN_Missions_ABC* mission = 0 );
             ADN_Models_OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name );
    virtual ~ADN_Models_OrderInfos();
    //@}

    //! @name Operations
    //@{
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    ADN_Models_OrderInfos* CreateCopy();
    //@}
};

#endif // ADN_Models_OrderInfos_h
