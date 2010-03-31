// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

namespace xml
{
    class xistream;
}

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

private:
    //! @name Member data
    //@{
    std::string login_;
    bool supervisor_;
    bool valid_;
    //@}
};

#endif // __Profile_h_
