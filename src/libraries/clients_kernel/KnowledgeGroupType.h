// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupType_h_
#define __KnowledgeGroupType_h_

namespace xml
{
    class xistream;
}

namespace kernel
{

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
    explicit KnowledgeGroupType( xml::xistream& xis );
    virtual ~KnowledgeGroupType();
    //@}

    //! @name Operations
    //@{
    const std::string&    GetName() const;
    double  GetCommunicationDelay() const;
    const std::string ShowCommunicationDelay() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroupType( const KnowledgeGroupType& );            //!< Copy constructor
    KnowledgeGroupType& operator=( const KnowledgeGroupType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    double      rCommunicationDelay_;
    //@}
};

}

#endif // __KnowledgeGroupType_h_
