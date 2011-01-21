// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_InhabitantType_h_
#define __MIL_InhabitantType_h_

#include "MT_Tools/MT_Stl.h"
#include <memory>

namespace xml
{
    class xistream;
    class xibufferstream;
}

class MIL_Army_ABC;
class MIL_Inhabitant;
class MIL_PopulationType;
class MIL_Schedule_ABC;

// =============================================================================
// @class  MIL_InhabitantType
// Created: SLG 2010-11-29
// =============================================================================
class MIL_InhabitantType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static const MIL_InhabitantType* Find( const std::string& strName );
    static const MIL_InhabitantType* Find( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    const MIL_PopulationType& GetAssociatedCrowdType() const;
    void InitializeSchedule( MIL_Schedule_ABC& schedule ) const;
    //@}

    //! @name Operations
    //@{
    MIL_Inhabitant& InstanciateInhabitant( xml::xistream& xis, MIL_Army_ABC& army ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_InhabitantType( const MIL_PopulationType& model );
             MIL_InhabitantType( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_InhabitantType();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_InhabitantType*, sCaseInsensitiveLess > T_InhabitantMap;
    typedef T_InhabitantMap::const_iterator                                        CIT_InhabitantMap;
    //@}

private:

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadInhabitant( xml::xistream& xis );
    //@}

private:
    //! @name Member Data
    //@{
    const std::string strName_;
    unsigned int nID_;
    const MIL_PopulationType* pModel_;
    float securityGainPerHour_;
    float securityLossOnFire_;
    std::auto_ptr< xml::xibufferstream > pXisSchedule_;
    static T_InhabitantMap inhabitants_;
    //@}
};

#endif // __MIL_InhabitantType_h_
