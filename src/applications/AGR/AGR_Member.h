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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Member.h $
// $Author: Age $
// $Modtime: 17/09/04 14:28 $
// $Revision: 2 $
// $Workfile: AGR_Member.h $
//
// *****************************************************************************

#ifndef __AGR_Member_h_
#define __AGR_Member_h_

#ifdef __GNUG__
#   pragma interface
#endif

class AGR_Type_ABC;
class AGR_Class;

// =============================================================================
/** @class  AGR_Member
    @brief  AGR_Member
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_Member
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Member( const std::string& strName, const AGR_Type_ABC& type, const AGR_Class& ownerClass, bool bOptional = false );
    virtual ~AGR_Member();
    //@}

    //! @name Operations
    //@{
    std::string ASNName() const;
    std::string ASNPrefixedName() const;
    std::string Mos2ASNPrefixedName() const;
    std::string CPPName() const;
    std::string HumanName() const;
    std::string DIAIndexName() const;

    std::string Declaration              () const;
    std::string DIAIndexDeclaration      () const;
    std::string DIAInitialisationCode    () const;
    std::string ASNInitialisationCode    () const;
    std::string DIAParametersInitialisationCode() const;
    std::string MemberInitialisationCode () const;
    std::string MissionInitialisationCode() const;
    std::string Mos2InitialisationCode   () const;

    std::string ResetCode                () const;
    std::string SerializationCode        () const;
    std::string SerializationCleaningCode() const;

    std::string TesterSerializationCode        () const;
    std::string TesterSerializationCleaningCode() const;
    std::string TesterParamAllocationCode      () const;
    std::string TesterParamCleaningCode        () const;

    std::string DIAType                  () const;
    //@}

    //! @name Accessors
    //@{
    const AGR_Type_ABC& Type() const;
    const AGR_Class&    OwnerClass() const;
    bool IsOptional() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeName( const std::string& strNewName );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Member( const AGR_Member& );            //!< Copy constructor
    AGR_Member& operator=( const AGR_Member& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    std::string Normalize( const std::string& strASN ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string strASNName_;
    const AGR_Type_ABC* pType_;
    const AGR_Class*  pOwnerClass_;

    bool bOptional_;
    //@}
};

#endif // __AGR_Member_h_
