// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

class MIL_OrderType_ABC;
class DEC_KnowledgeResolver_ABC;
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
             MIL_OrderTypeParameter( const MIL_OrderType_ABC& orderType, xml::xistream& xis );
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
    //@}

private:
    //! @name Member data
    //@{
          std::string            strName_;
          std::string            strDiaName_;
          bool                   bIsOptional_;
          bool                   bIsList_;
    const MIL_ParameterType_ABC* pParameter_;
    //@}
};

#endif // __MIL_OrderTypeParameter_h_
