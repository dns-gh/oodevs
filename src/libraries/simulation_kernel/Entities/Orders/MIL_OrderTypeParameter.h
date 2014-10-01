// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_OrderTypeParameter_h_
#define __MIL_OrderTypeParameter_h_

namespace sword
{
    class MissionParameter;
}

namespace xml
{
    class xistream;
}

class MIL_ParameterType_ABC;
class MIL_MissionParameter_ABC;

// =============================================================================
/** @class  MIL_OrderTypeParameter
    @brief  MIL_OrderTypeParameter
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_OrderTypeParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_OrderTypeParameter( xml::xistream& xis );
    virtual ~MIL_OrderTypeParameter();
    //@}

    //! @name Operations
    //@{
    bool Copy( const MIL_MissionParameter_ABC& from, sword::MissionParameter& to ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const std::string& GetDIAName() const;
    const MIL_ParameterType_ABC& GetType() const;
    bool IsList() const;
    bool IsOptional() const { return bIsOptional_; }
    bool CheckSize( unsigned int size ) const;
    //@}

private:
    //! @name Member data
    //@{
          std::string            strName_;
          std::string            strDiaName_;
          bool                   bIsOptional_;
    const MIL_ParameterType_ABC* pParameter_;
    unsigned int           minOccurs_;
    unsigned int           maxOccurs_;
    //@}
};

#endif // __MIL_OrderTypeParameter_h_
