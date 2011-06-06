// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_OrderType_ABC_h_
#define __MIL_OrderType_ABC_h_

namespace sword
{
    class MissionParameters;
}

namespace xml
{
    class xistream;
}

class MIL_MissionParameter_ABC;
class MIL_OrderContext;
class MIL_OrderTypeParameter;
class MIL_ParameterType_ABC;

// =============================================================================
/** @class  MIL_OrderType_ABC
    @brief  MIL_OrderType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_OrderType_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< const MIL_OrderTypeParameter* > T_MissionParameterVector;
    typedef T_MissionParameterVector::const_iterator     CIT_MissionParameterVector;
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderType_ABC( unsigned int nID, unsigned int contextLength, xml::xistream& xis );
    virtual ~MIL_OrderType_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string&              GetName   () const;
    const std::string&              GetDIAType() const;
          unsigned int              GetID     () const;
    const MIL_OrderTypeParameter&   GetParameterType( unsigned int index ) const;
    const std::string&              GetParameterName( unsigned int ) const;
          unsigned int              GetParameterIndex( const std::string& name ) const;
    const T_MissionParameterVector& GetParameters() const { return parameters_; }
    //@}

    //! @name Copy operations
    //@{
    void InitializeDefault( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters ) const;

    bool Copy( const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& from, sword::MissionParameters& to, const MIL_OrderContext& context ) const;
    //@}

protected:
    MIL_OrderType_ABC();

private:
    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, unsigned int& index, unsigned int contextLength );
    //@}

private:
    const unsigned int             nID_;
          std::string              strName_;
          std::string              diaType_;
          T_MissionParameterVector parameters_;
};

#endif // __MIL_OrderType_ABC_h_
