// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

struct ASN1T_MasterProfile;
struct ASN1T_MsgsInMaster;

namespace xml{ class xistream; };

namespace master
{
class Master;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: NLD 2006-09-19
// =============================================================================
class Profile
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile( const Master& master, const std::string& strLogin, xml::xistream& xis );
    virtual ~Profile();
    //@}

    //! @name Main
    //@{
    bool CheckPassword( const std::string& strPassword ) const;
    bool CheckRights  ( const ASN1T_MsgsInMaster& msg ) const;
    //@}

    //! @name Network
    //@{
    void Send( ASN1T_MasterProfile& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignement operator
    //@}

private:
    const Master&         master_;
          std::string     strLogin_;
          std::string     strPassword_;

    // Roles
    bool bRoleAdaptation_;
    bool bRolePreparation_;
    bool bRoleGame_;
    bool bRoleSupervision_;
};

}

#endif // __Profile_h_
