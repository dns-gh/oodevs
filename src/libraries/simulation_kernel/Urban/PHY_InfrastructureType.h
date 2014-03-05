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

// =============================================================================
/** @class  PHY_InfrastructureType
    @brief  PHY_InfrastructureType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_InfrastructureType : private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    ~PHY_InfrastructureType();
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
    //! @name Constructors
    //@{
    PHY_InfrastructureType( const std::string& name, xml::xistream& xis );
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadInfrastructure( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const std::string symbol_;
    const bool medical_;
    //@}
};

#endif // __PHY_InfrastructureType_h_
