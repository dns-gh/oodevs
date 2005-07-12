// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_FragOrder.h $
// $Author: Nld $
// $Modtime: 9/11/04 10:01 $
// $Revision: 7 $
// $Workfile: AGR_FragOrder.h $
//
// *****************************************************************************

#ifndef __AGR_FragOrder_h_
#define __AGR_FragOrder_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Types.h"
#include "AGR_Class.h"

class AGR_Workspace;
class AGR_Member;

// =============================================================================
/** @class  AGR_FragOrder
    @brief  AGR_FragOrder
*/
// Created: AGE 2004-09-15
// =============================================================================
class AGR_FragOrder : public AGR_Class
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_FragOrder( const std::string& strAsnName, const std::string& strModuleName );
    virtual ~AGR_FragOrder();
    //@}

    //! @name Operations
    //@{
    void Read( MT_XXmlInputArchive& input, const AGR_Workspace& workspace );

    void GenerateMilClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMilClassCpp   ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;

    std::string GenerateDiaDefinition() const;
    std::string MosLight2CreationCode() const;
    std::string MosLight2DeletionCode() const;
    std::string MosLight2ImplementationCode() const;

    bool CanBeDemanded() const;
    std::string ASNDemandName() const;
    std::string DIATypeName() const;

    virtual std::string MilFilePathName() const;
    virtual std::string HumanName() const;
    virtual std::string ASNTypeName() const;
    virtual std::string BaseName() const;
    virtual std::string EnumName() const;

    virtual std::string KnowledgeAccessor() const;
    virtual std::string ASNPrefixedName( const AGR_Member& member ) const;
    virtual std::string Mos2ASNPrefixedName( const AGR_Member& mamber ) const;

    bool IsAvailableForAllMissions () const;
    bool IsAvailableWithoutMissions() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void SetLowName( const std::string& str );
            void SetDemandLowName( const std::string& str );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_FragOrder( const AGR_FragOrder& );            //!< Copy constructor
    AGR_FragOrder& operator=( const AGR_FragOrder& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    enum E_OrderType
    {
        eComplexContent,
        eRestriction,
        eSequence
    };
    //@}

    //! @name Helpers
    //@{
    bool NeedsDeleting() const;
    //@}

private:
    //! @name Member data
    //@{
    bool bAvailableForAllMissions_;
    bool bAvailableWithoutMissions_;
    bool bOrderForAutomata_;
    std::string strDemandLowName_;
    E_OrderType nFragOrderType_;
    //@}
};

#endif // __AGR_FragOrder_h_
