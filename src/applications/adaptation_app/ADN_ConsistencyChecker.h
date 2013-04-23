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
#include "ADN_NavigationInfos.h"

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
    void AddError( ConsistencyError error );
    void AddError( E_ConsistencyCheck type, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    static ADN_NavigationInfos::GoTo* CreateGotoInfo( const std::string& name, int tab, int subTab = -1 );
    static void AddLoadingError( ConsistencyError error );
    static void AddLoadingError( E_ConsistencyCheck type, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    static void ClearLoadingErrors();
    static const T_ConsistencyErrors& GetLoadingErrors();
    //@}

private:
    //! @name Helpers
    //@{
    void CheckValidDatabase();
    bool IsAlreadyRegistered( const std::string& code, E_ConsistencyCheck type ) const;
    //@}

private:
    //! @name Member data
    //@{
    static T_ConsistencyErrors loadingErrors_;
    //@}
};

#endif // __ADN_ConsistencyChecker_h_
