// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-17 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Class.h $
// $Author: Age $
// $Modtime: 20/09/04 11:39 $
// $Revision: 3 $
// $Workfile: AGR_Class.h $
//
// *****************************************************************************

#ifndef __AGR_Class_h_
#define __AGR_Class_h_

#ifdef __GNUG__
#   pragma interface
#endif

class AGR_Member;

// =============================================================================
/** @class  AGR_Class
    @brief  AGR_Class
*/
// Created: AGE 2004-09-17
// =============================================================================
class AGR_Class
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Class( const std::string& strAsnName, const std::string& strModuleName );
    virtual ~AGR_Class();
    //@}

    //! @name Types
    //@{
    typedef std::vector< AGR_Member* >       T_MemberVector;
    typedef T_MemberVector::const_iterator CIT_MemberVector;
    //@}

    //! @name Operations
    //@{
    virtual std::string KnowledgeAccessor() const = 0;
    virtual std::string KnowledgeAccessorCheck() const = 0;
    virtual std::string ASNPrefixedName( const AGR_Member& mamber ) const;
    virtual std::string Mos2ASNPrefixedName( const AGR_Member& mamber ) const;

    virtual std::string MilFileName() const;
    virtual std::string MilFilePathName() const;
    virtual std::string HumanName() const;
    virtual std::string EnumName() const;
    virtual std::string ASNTypeName() const;
    virtual std::string BaseName() const;
    //@}

    //! @name Accessors
    //@{
    const T_MemberVector& MemberList() const;

    const std::string& Name() const;
          std::string  CompleteName() const;
    const std::string& LowName() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void SetLowName( const std::string& strName );
    //@}

protected:
    //! @name Modifiers
    //@{
    void RegisterMember( AGR_Member& member );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Class( const AGR_Class& );            //!< Copy constructor
    AGR_Class& operator=( const AGR_Class& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string strAsnName_;
    std::string strModuleName_;
    std::string strLowName_;

    T_MemberVector memberList_;
    //@}
};

#endif // __AGR_Class_h_
