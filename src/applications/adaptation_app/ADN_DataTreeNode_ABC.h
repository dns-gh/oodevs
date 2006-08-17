// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-12 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_DataTreeNode_ABC.h $
// $Author: Ape $
// $Modtime: 11/01/05 10:13 $
// $Revision: 2 $
// $Workfile: ADN_DataTreeNode_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_DataTreeNode_ABC_h_
#define __ADN_DataTreeNode_ABC_h_


// =============================================================================
/** @class  ADN_DataTreeNode_ABC
    @brief  ADN_DataTreeNode_ABC
    @par    Using example
    @code
    ADN_DataTreeNode_ABC;
    @endcode
*/
// Created: AGN 2004-05-12
// =============================================================================
class ADN_DataTreeNode_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_DataTreeNode_ABC( ADN_DataTreeNode_ABC* pParent = 0 );
    virtual ~ADN_DataTreeNode_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    void SetParentNode( ADN_DataTreeNode_ABC& parentNode );
    //@}

    //! @name Accessors
    //@{
    ADN_DataTreeNode_ABC* GetParentNode() const;
    //@}


private:
    //! @name Copy/Assignement
    //@{
    ADN_DataTreeNode_ABC( const ADN_DataTreeNode_ABC& );            //!< Copy constructor
    ADN_DataTreeNode_ABC& operator=( const ADN_DataTreeNode_ABC& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ADN_DataTreeNode_ABC* pParent_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_DataTreeNode_ABC::SetParentNode
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
inline
void ADN_DataTreeNode_ABC::SetParentNode( ADN_DataTreeNode_ABC& parentNode )
{
    pParent_ = & parentNode;
}

#endif // __ADN_DataTreeNode_ABC_h_
