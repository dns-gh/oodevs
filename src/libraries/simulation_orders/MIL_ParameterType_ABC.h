// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_ABC_h_
#define __MIL_ParameterType_ABC_h_

class MIL_OrderType_ABC;
class DEC_KnowledgeResolver_ABC;
class MIL_MissionParameter_ABC;

// =============================================================================
/** @class  MIL_ParameterType_ABC
    @brief  MIL_ParameterType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_ABC
{
public:
    //! @name Factory
    //@{
    static void                         Initialize();
    static const MIL_ParameterType_ABC* Find      ( const std::string& strName );

    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}
    
    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const = 0;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const = 0;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_ABC( const std::string& strName );
    virtual ~MIL_ParameterType_ABC();
    //@}

private:
    //! @name Initialization
    //@{
    template< typename T > static void RegisterParameterType();
    template< typename T > static void RegisterParameterType( const std::string& name );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_ParameterType_ABC*, sCaseInsensitiveLess > T_ParameterMap;
    typedef T_ParameterMap::const_iterator                                              CIT_ParameterMap;
    //@}

private:
    const std::string strName_;

private:
    static T_ParameterMap parameters_;
};

#endif // __MIL_ParameterType_ABC_h_
