// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __frontend_Profile_h_
#define __frontend_Profile_h_

namespace sword
{
    class ProfileDescription;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class ProfileVisitor_ABC;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: RDS 2008-09-10
// =============================================================================
class Profile
{
public:
    //! @name Statics
    //@{
    static Profile Invalid;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Profile();
    explicit Profile( xml::xistream& xis );
    virtual ~Profile();
    //@}

    //! @name Accessors
    //@{
    QString GetLogin() const;
    bool IsSupervision() const;
    bool IsValid() const;
    //@}

    //! @name Operations
    //@{
    static void VisitProfiles( const tools::GeneralConfig& config, const std::string& exercise, ProfileVisitor_ABC& visitor );
    void Send( sword::ProfileDescription& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string login_;
    std::string password_;
    bool supervisor_;
    bool valid_;
    //@}
};

}

#endif // __frontend_Profile_h_
