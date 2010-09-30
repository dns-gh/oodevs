// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_OrderType_ABC_h_
#define __MIL_OrderType_ABC_h_

namespace Common
{
    class MsgMissionParameters;
}

namespace xml
{
    class xistream;
}

class DEC_KnowledgeResolver_ABC;
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
    //! @name Constructors/Destructor
    //@{
             MIL_OrderType_ABC( unsigned int nID, xml::xistream& xis );
    virtual ~MIL_OrderType_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName   () const;
    const std::string& GetDIAType() const;
          unsigned int GetID     () const;
    const MIL_ParameterType_ABC& GetParameterType( unsigned int ) const;
    const std::string&           GetParameterName( unsigned int ) const;
    unsigned int                 GetParameterIndex( const std::string& name ) const;
    //@}

    //! @name Copy operations
    //@{
    bool Copy( const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& from, Common::MsgMissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const;
    void InitializeDefault( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters ) const;
    //@}

protected:
    MIL_OrderType_ABC();

private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_OrderTypeParameter* > T_MissionParameterVector;
    typedef T_MissionParameterVector::const_iterator     CIT_MissionParameterVector;
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    const unsigned int             nID_;
          std::string              strName_;
          std::string              diaType_;
          T_MissionParameterVector parameters_;
};

#endif // __MIL_OrderType_ABC_h_
