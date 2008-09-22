// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __WeaponSystemType_h_
#define __WeaponSystemType_h_

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  WeaponSystemType
    @brief  WeaponSystemType
*/
// Created: SBO 2008-08-06
// =============================================================================
class WeaponSystemType
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponSystemType( xml::xistream& xis );
    virtual ~WeaponSystemType();
    //@}

    //! @name Operations
    //@{
    std::string GetId() const;
    unsigned int GetMaxRange() const;
    unsigned int GetMinRange() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeaponSystemType( const WeaponSystemType& );            //!< Copy constructor
    WeaponSystemType& operator=( const WeaponSystemType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadDirectFire( xml::xistream& xis );
    void ReadDirectFireHitProbabilities( xml::xistream& xis );
    void ReadDirectFireHitProbability( xml::xistream& xis );
    void ReadIndirectFire( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string launcher_;
    std::string ammunition_;
    unsigned int maxIndirectRange_;
    unsigned int minIndirectRange_;
    unsigned int maxDirectRange_;
    unsigned int minDirectRange_;
    //@}
};

}

#endif // __WeaponSystemType_h_
