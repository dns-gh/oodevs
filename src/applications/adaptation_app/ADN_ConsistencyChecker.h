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

    //! @name Static operations
    //@{
    static ADN_NavigationInfos::GoTo* CreateGotoInfo( const std::string& name, int tab, int subTab = -1 );
    static void AddLoadingError( ConsistencyError error );
    static void AddLoadingError( E_ConsistencyCheck type, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    static void ClearLoadingErrors();
    static const T_ConsistencyErrors& GetLoadingErrors();
    //@}

private: // NNO Consistency

    //! @name Types
    //@{
    struct NNOElement
    {
        NNOElement( const std::string& name, const std::string& codeNNO, const std::string& codeEMAT8, int tab, int subTab = -1, bool bRepairTypeValid = true )
            : name_( name ), codeNNO_( codeNNO ), codeEMAT8_( codeEMAT8 ), tab_( tab ), subTab_( subTab ), bRepairTypeValid_( bRepairTypeValid ) {}
        std::string name_;
        std::string codeNNO_;
        std::string codeEMAT8_;
        int         tab_;
        int         subTab_;
        bool        bRepairTypeValid_;
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
    void CheckMissionTypes( const ADN_Missions_Data::T_Mission_ABC_Vector& missions, int subTab );
    void CheckParameters( const ADN_Missions_Data::T_MissionParameter_Vector& parameters, const std::string& missionName, int subTab );
    void CheckMissionParameters();
    void CheckComposantes();
    bool IsAlreadyRegistered( const std::string& code, E_ConsistencyCheck type ) const;
    void AddError( E_ConsistencyCheck type, const NNOElement& element );
    void AddError( E_ConsistencyCheck type, const T_NNOElements& elements );
    void AddError( E_ConsistencyCheck type, const ADN_Missions_Data::ADN_Missions_ABC& rhs, const ADN_Missions_Data::ADN_Missions_ABC& lhs, int subTab );
    //@}

private:
    //! @name Member data
    //@{
    static T_ConsistencyErrors loadingErrors_;
    //@}
};

#endif // __ADN_ConsistencyChecker_h_
