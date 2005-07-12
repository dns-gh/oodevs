// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_LogWorkTime.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:20 $
// $Revision: 4 $
// $Workfile: PHY_LogWorkTime.h $
//
// *****************************************************************************

#ifndef __PHY_LogWorkTime_h_
#define __PHY_LogWorkTime_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_LogWorkTime
// Created: JVT 2004-08-03
// =============================================================================
class PHY_LogWorkTime
{
    MT_COPYNOTALLOWED( PHY_LogWorkTime )

public:
    //! @name Types
    //@{
    static PHY_LogWorkTime time8Hours_;
    static PHY_LogWorkTime time12Hours_;
    static PHY_LogWorkTime time16Hours_;
    static PHY_LogWorkTime time20Hours_;
    static PHY_LogWorkTime time24Hours_;
    //@}

public:
    //! @name Manager
    //@{
    static       void             Initialize( MIL_InputArchive& archive );
    static       void             Terminate ();
    static const PHY_LogWorkTime* Find      ( ASN1T_EnumTempsBordee nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&           GetName             () const;
          ASN1T_EnumTempsBordee  GetAsnID            () const;
          MT_Float               GetRepairTimeFactor () const;
          MT_Float               GetSortingTimeFactor() const;
          MT_Float               GetHealingTimeFactor() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_LogWorkTime*, sCaseInsensitiveLess > T_WorkTimeMap;
    typedef T_WorkTimeMap::const_iterator                                         CIT_WorkTimeMap;
    //@}

private:
     PHY_LogWorkTime( const std::string& strName, ASN1T_EnumTempsBordee asn );
    ~PHY_LogWorkTime();

    //! @name Init
    //@{
    void ReadData( MIL_InputArchive& archive );
    //@}

private:
    const std::string           strName_;
    const ASN1T_EnumTempsBordee asn_;
          MT_Float              rRepairTimeFactor_;
          MT_Float              rSortingTimeFactor_;
          MT_Float              rHealingTimeFactor_;

private:
    static T_WorkTimeMap workTimes_;
};

#include "PHY_LogWorkTime.inl"

#endif // __PHY_LogWorkTime_h_
