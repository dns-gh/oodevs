// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-13 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Type_ABC.h $
// $Author: Age $
// $Modtime: 24/09/04 15:29 $
// $Revision: 3 $
// $Workfile: AGR_Type_ABC.h $
//
// *****************************************************************************

#ifndef __AGR_Type_ABC_h_
#define __AGR_Type_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

class AGR_Member;

// =============================================================================
/** @class  AGR_Type_ABC
    @brief  AGR_Type_ABC
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    AGR_Type_ABC( const std::string& strAsnType, const std::string& strFunctionSuffix, const std::string& strCPPType, const std::string& strDIAType, const std::string& strTesterType, bool bRequiresCleaning );
    virtual ~AGR_Type_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOfType( const std::string& strASNType ) const;

    std::string Declaration                    ( const AGR_Member& member ) const;
    std::string DIAType                        ( const AGR_Member& member ) const;
    std::string DIAIndexDeclaration            ( const AGR_Member& member ) const;
    std::string DIAInitialisationCode          ( const AGR_Member& member ) const;
    std::string DIAParametersInitialisationCode( const AGR_Member& member ) const;
    std::string MemberInitialisationCode       ( const AGR_Member& member ) const;
    std::string MissionInitialisationCode      ( const AGR_Member& member ) const;
    std::string ResetCode                      ( const AGR_Member& member ) const;
    std::string SerializationCleaningCode      ( const AGR_Member& member ) const;

    virtual std::string MOS2RCCode                     ( const std::string& strAsnParam ) const;
    virtual std::string MOSRCCode                      ( const std::string& strAsnParam ) const;

    virtual std::string ASNInitialisationCode          ( const AGR_Member& member ) const;
    virtual std::string Mos2InitialisationCode         ( const AGR_Member& member ) const;
    virtual std::string SerializationCode              ( const AGR_Member& member ) const;

    virtual std::string TesterParamAllocationCode      ( const AGR_Member& member ) const;
    virtual std::string TesterParamCleaningCode        ( const AGR_Member& member ) const;
    virtual std::string TesterSerializationCode        ( const AGR_Member& member ) const;
            std::string TesterSerializationCleaningCode( const AGR_Member& member ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& ASNType() const;
    const std::string& GetDIAType() const;
    const std::string& GetHumanName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Type_ABC( const AGR_Type_ABC& );            //!< Copy constructor
    AGR_Type_ABC& operator=( const AGR_Type_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    std::string RemoveNamespace( const std::string& strName ) const;
    //@}    
private:
    //! @name Member data
    //@{
    std::string strHumanName_;
    std::string strAsnType_;
    std::string strFunctionSuffix_;
    std::string strCPPType_;
    std::string strDIAType_;
    std::string strTesterType_;
    bool bExplicitMember_;
    bool bRequiresCleaning_;
    //@}
};

#endif // __AGR_Type_ABC_h_
