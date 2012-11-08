// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ConsistencyChecker_h_
#define __ADN_ConsistencyChecker_h_

#include "clients_kernel/ConsistencyChecker_ABC.h"
#include "ADN_ConsistencyErrorTypes.h"
#include "ADN_Missions_Data.h"

// =============================================================================
/** @class  ADN_ConsistencyChecker
    @brief  ADN_ConsistencyChecker
*/
// Created: ABR 2012-06-06
// =============================================================================
class ADN_ConsistencyChecker : public kernel::ConsistencyChecker< E_ConsistencyCheck, ADN_NavigationInfos::GoTo >
{
    typedef kernel::ConsistencyChecker< E_ConsistencyCheck, ADN_NavigationInfos::GoTo > T_Parent;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ConsistencyChecker();
    virtual ~ADN_ConsistencyChecker();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckConsistency();
    //@}

private: // NNO Consistency

    //! @name Types
    //@{
    struct NNOElement
    {
        NNOElement( const std::string& name, const std::string& codeNNO, const std::string& codeEMAT8, int tab, int subTab = -1 )
            : name_( name ) , codeNNO_( codeNNO ) , codeEMAT8_( codeEMAT8 ) , tab_( tab ) , subTab_( subTab ) {}
        std::string name_;
        std::string codeNNO_;
        std::string codeEMAT8_;
        int         tab_;
        int         subTab_;
    };
    typedef std::vector< NNOElement >        T_NNOElements;
    typedef T_NNOElements::iterator         IT_NNOElements;
    typedef T_NNOElements::const_iterator  CIT_NNOElements;
    //@}

    //! @name Helpers
    //@{
    void CheckNNOConsistency();
    void CheckMissionsTypes();
    void CheckBreakdownsBackup();
    void CheckMissionTypes( const ADN_Missions_Data::T_Mission_Vector& missions, int subTab );
    ADN_NavigationInfos::GoTo* CreateGotoInfo( const std::string& name, int tab, int subTab = -1 );
    bool IsAlreadyRegistered( const std::string& code, E_ConsistencyCheck type ) const;
    void AddError( E_ConsistencyCheck type, const NNOElement& element );
    void AddError( E_ConsistencyCheck type, const T_NNOElements& elements );
    void AddError( E_ConsistencyCheck type, const ADN_Missions_Data::Mission& rhs, const ADN_Missions_Data::Mission& lhs, int subTab );
    //@}
};

#endif // __ADN_ConsistencyChecker_h_
