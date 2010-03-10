// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ZurbType_h_
#define __ZurbType_h_

namespace xml
{
    class xistream;
}

namespace urban
{
    class StaticModel;
}

// =============================================================================
/** @class  ZurbType
    @brief  ZurbType
*/
// Created: SLG 2010-03-05
// =============================================================================
class ZurbType : private boost::noncopyable
{

public:

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static ZurbType& GetZurbType();
    urban::StaticModel& GetStaticModel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ZurbType( const ZurbType& );            //!< Copy constructor
    ZurbType& operator=( const ZurbType& ); //!< Assignment operator
    //@}

    //! @name Constructors/Destructor
    //@{
    ZurbType( xml::xistream& xis );
    virtual ~ZurbType();
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< urban::StaticModel > staticModel_;
    static ZurbType* zurbType_;
    //@}
};

#endif // __ZurbType_h_
