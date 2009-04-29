// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorTypeResolver_h_
#define __IndicatorTypeResolver_h_

class IndicatorType;

// =============================================================================
/** @class  IndicatorTypeResolver
    @brief  IndicatorTypeResolver
*/
// Created: SBO 2009-04-15
// =============================================================================
class IndicatorTypeResolver
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorTypeResolver();
    virtual ~IndicatorTypeResolver();
    //@}

    //! @name Operations
    //@{
    std::string Resolve( const std::string& type ) const;
    void AddElement( const IndicatorType& instance, const IndicatorType& definition );
    static std::string ToSimpleType( const std::string& type );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorTypeResolver( const IndicatorTypeResolver& );            //!< Copy constructor
    IndicatorTypeResolver& operator=( const IndicatorTypeResolver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update();
    void ConvertTypes( std::vector< std::string >& type1, std::vector< std::string >& type2 ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > dictionary_;
    std::map< const IndicatorType*, const IndicatorType* > instances_;
    //@}
};

#endif // __IndicatorTypeResolver_h_
