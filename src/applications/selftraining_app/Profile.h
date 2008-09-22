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

#include <vector>

namespace xml
{
    class xistream  ; 
}

class Side ; 

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: RDS 2008-09-10
// =============================================================================
class Profile
{

public:

    enum Rights 
    {
        NONE = 0x0, 
        READ = 0x1 , 
        WRITE = 0x2
    };

    //! @name Constructors/Destructor
    //@{
    explicit Profile( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides );
    virtual ~Profile( );
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    virtual QString GetLogin() const ;
    virtual bool IsSupervision() const ;    
    const int GetRights( Side& side ) const ; 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadSides( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides ); 
    void ReadSide ( xml::xistream& xis, const kernel::Resolver_ABC<Side>& sides, int rights ) ; 
    //@}

private:

    typedef std::map< Side*, int >              T_SideRightsList; 
    typedef T_SideRightsList::const_iterator  CIT_SideRightsList; 

    //! @name Member data
    //@{
    std::string name_ ; 
    bool supervisor_ ; 
    T_SideRightsList sides_ ; 
    //@}
};

#endif // __Profile_h_
