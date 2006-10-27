// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupType_h_
#define __KnowledgeGroupType_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  KnowledgeGroupType
    @brief  KnowledgeGroupType
*/
// Created: SBO 2006-10-27
// =============================================================================
class KnowledgeGroupType
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupType( const QString& name, const QString& value );
    virtual ~KnowledgeGroupType();
    //@}

    //! @name Operations
    //@{
    const QString& GetName() const;
    const QString& GetValue() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupType( const KnowledgeGroupType& );            //!< Copy constructor
    KnowledgeGroupType& operator=( const KnowledgeGroupType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString value_;
    //@}
};

#endif // __KnowledgeGroupType_h_
