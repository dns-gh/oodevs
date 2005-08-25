// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationType.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:04 $
// $Revision: 3 $
// $Workfile: CommunicationType.h $
//
// *****************************************************************************

#ifndef __CommunicationType_h_
#define __CommunicationType_h_

#include "Types.h"

namespace TIC
{

// =============================================================================
/** @class  CommunicationType
    @brief  CommunicationType
    @par    Using example
    @code
    CommunicationType;
    @endcode
*/
// Created: SBO 2005-07-12
// =============================================================================
class CommunicationType
{
public:
    //! @name Static member
    //@{
    static const CommunicationType notDefined;
    //@}

public:
    //! @name Types
    //@{
    enum EType
    {
        eNoType,
        eOrder,
        ePerception,
        eReport
    };

    enum ESubType
    {
        eNoSubType,
        // order subtypes
        eMission,
        eConduite,
          
        // perception subtypes
        eUnitFriend,
        eUnitEnemy,
        eUnitNeutral,
        eUnitUnknown,
        eUnitLost,
        eObject,
        eObjectLost,
          
        // rc subtypes
        eReportEnemy,
        eReportObstacle,
        eReportNbc,
        eReportLogisticSituation,
        eReportArtillery,
        eReportMission,
        eReportScipio
    };
    //@}

public:
    //! @name Static Operations
    //@{
    static       void               Initialize();
    static       void               Terminate ();
    static const CommunicationType& Find      ( EType eType, ESubType eSubType );
    static const CommunicationType& Find      ( EType eType, uint     nAsnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetType   () const;
    const std::string& GetSubType() const;
          bool         IsReport  () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const CommunicationType& type ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             CommunicationType( EType eType, ESubType eSubType );
    virtual ~CommunicationType();
    //@}

private:
    //! @name Comparator
    //@{
    struct sCommunicationTypeLess
    {
        bool operator()( EType lhs, EType rhs ) const
        {
            return lhs < rhs;
        }

        bool operator()( ESubType lhs, ESubType rhs ) const
        {
            return lhs < rhs;
        }

        bool operator()(const CommunicationType* lhs, const CommunicationType* rhs) const
        {
            return ( operator() ( lhs->eType_   , rhs->eType_    ) || 
                    !operator() ( rhs->eType_   , lhs->eType_    ) && 
                     operator() ( lhs->eSubType_, rhs->eSubType_ ) );
        }
    };
    //@}


private:
    //! @name Types
    //@{
    typedef std::map< EType, const std::string >    T_TypeMap;
    typedef T_TypeMap::const_iterator               CIT_TypeMap;

    typedef std::map< ESubType, const std::string > T_SubTypeMap;
    typedef T_SubTypeMap::const_iterator            CIT_SubTypeMap;

    typedef std::set< const CommunicationType*, sCommunicationTypeLess > T_CommunicationTypeSet;
    typedef T_CommunicationTypeSet::const_iterator                       CIT_CommunicationTypeSet;
    //@}

private:
    //! @name Member data
    //@{
    EType    eType_;
    ESubType eSubType_;
    //@}

    //! @name Static member
    //@{
    static T_TypeMap              types_;
    static T_SubTypeMap           subTypes_;
    static T_CommunicationTypeSet comTypes_;
    //@}
};

} // end namespace TIC

#include "CommunicationType.inl"

#endif // __CommunicationType_h_
