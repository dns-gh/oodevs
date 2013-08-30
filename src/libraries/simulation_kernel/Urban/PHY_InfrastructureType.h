// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_InfrastructureType_h_
#define __PHY_InfrastructureType_h_

#include "MT_Tools/MT_String.h"

// =============================================================================
/** @class  PHY_InfrastructureType
    @brief  PHY_InfrastructureType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_InfrastructureType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_InfrastructureType* > T_InfrastructureMap;
    typedef T_InfrastructureMap::const_iterator CIT_InfrastructureMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static const PHY_InfrastructureType* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    bool IsMedical() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PHY_InfrastructureType( const std::string& name, xml::xistream& xis );
    virtual ~PHY_InfrastructureType();
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadInfrastructure( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    static T_InfrastructureMap infrastructures_;
    std::string name_;
    std::string symbol_;
    bool medical_;
    //@}
};

#endif // __PHY_InfrastructureType_h_
