// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ADN_Models_MissionInfos_h
#define ADN_Models_MissionInfos_h

#include "ADN_CrossedRef.h"
#include "ADN_Missions_ABC.h"
#include "ADN_Models_OrderInfos.h"

// =============================================================================
/** @class  ADN_Models_MissionInfos
    @brief  ADN_Models_MissionInfos
*/
// Created: ABR 2014-04-10
// =============================================================================
class ADN_Models_MissionInfos : public ADN_CrossedRef< ADN_Missions_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Models_MissionInfos( const ADN_Type_Vector_ABC< ADN_Missions_ABC >& missions, ADN_Missions_ABC* mission = 0 );;
    virtual ~ADN_Models_MissionInfos();
    //@}

    //! @name Operations
    //@{
    ADN_Models_MissionInfos* CreateCopy();
    void ReadArchive( xml::xistream& input );
    void ReadFragOrder( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ADN_Models_OrderInfos > vOrders_;
    //@}
};

#endif // ADN_Models_MissionInfos_h
