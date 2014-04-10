// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ADN_Models_ModelInfos_h
#define ADN_Models_ModelInfos_h

#include "ADN_Models_MissionInfos.h"
#include "ADN_Models_OrderInfos.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"

class ADN_Missions_FragOrder;

// =============================================================================
/** @class  ADN_Models_ModelInfos
    @brief  ADN_Models_ModelInfos
*/
// Created: ABR 2014-04-10
// =============================================================================
class ADN_Models_ModelInfos : public ADN_RefWithLocalizedName
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Models_ModelInfos();
    explicit ADN_Models_ModelInfos( E_EntityType type );
    virtual ~ADN_Models_ModelInfos();
    //@}

    //! @name Operations
    //@{
    ADN_Models_ModelInfos* CreateCopy();
    void ReadArchive( xml::xistream& input );
    void ReadMission( xml::xistream& input );
    void ReadOrder( xml::xistream& input );
    void WriteArchive( const std::string& type, xml::xostream& output );
    void AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& order );
    void RemoveFragOder( const std::string& order );
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_String strDiaType_;
    ADN_Type_Path strFile_;
    ADN_Type_Bool isMasalife_;
    ADN_Type_Vector_ABC< ADN_Models_MissionInfos > vMissions_;
    ADN_Type_Vector_ABC< ADN_Models_OrderInfos > vFragOrders_;
    E_EntityType type_;
    //@}
};

#endif // ADN_Models_ModelInfos_h
