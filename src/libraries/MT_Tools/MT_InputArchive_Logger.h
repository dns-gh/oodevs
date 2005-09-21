// *****************************************************************************
//
// $Created: JVT 2004-08-23 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_InputArchive_Logger.h $
// $Author: Ape $
// $Modtime: 22/04/05 16:34 $
// $Revision: 5 $
// $Workfile: MT_InputArchive_Logger.h $
//
// *****************************************************************************

#ifndef __MT_InputArchive_Logger_h_
#define __MT_InputArchive_Logger_h_

#include <stdexcept>

// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
class MT_ArchiveLogger_Exception : public std::runtime_error
{
public:
    MT_ArchiveLogger_Exception( const std::string& str ) : std::runtime_error( str.c_str() ) {}
};


// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename Archive>
class MT_InputArchive_Logger
{
public:
    enum ErrorPolicyType
    {
        eNothing,
        eWarning,
        eThrow
    };

public:
    //! @name Constructors/Destructor
    //@{
     MT_InputArchive_Logger();
    ~MT_InputArchive_Logger();
    //@}

    //! @name Warning Management
    //@{
    void AddWarningStream   ( std::ostream& );
    void RemoveWarningStream( std::ostream& );

    void        GetContext( std::string& ) const;
    std::string GetContext()               const;
    //@}

    //! @name archive parsing
    //@{
    bool Open ( const std::string&, ErrorPolicyType = eThrow );
    void SetData( const std::string& );
    bool Close( ErrorPolicyType = eThrow );

    bool Section   ( const std::string&, ErrorPolicyType = eThrow );
    bool EndSection( ErrorPolicyType = eThrow );
    
    bool BeginList      ( const std::string&, ErrorPolicyType = eThrow );
    bool NextListElement();
    bool EndList        ( ErrorPolicyType = eThrow );

    template <typename T, typename CheckPolicy>
    bool ReadField( const std::string& strField, T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool ReadField( const std::string& strField, T& value, ErrorPolicyType nErrorPolicy = eThrow );

    template <typename T, typename CheckPolicy>
    bool ReadAttribute( const std::string& strAttribute, T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool ReadAttribute( const std::string& strAttribute, T& value, ErrorPolicyType nErrorPolicy = eThrow );

    template <typename T, typename CheckPolicy>
    bool Read( T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool Read( T& value, ErrorPolicyType nErrorPolicy = eThrow );

    template <typename T, typename CheckPolicy>
    bool ReadTimeField( const std::string& strField, T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool ReadTimeField( const std::string& strField, T& value, ErrorPolicyType nErrorPolicy = eThrow );

    template <typename T, typename CheckPolicy>
    bool ReadTimeAttribute( const std::string& strAttribute, T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool ReadTimeAttribute( const std::string& strAttribute, T& value, ErrorPolicyType nErrorPolicy = eThrow );

    template <typename T, typename CheckPolicy>
    bool ReadTime( T& value, const CheckPolicy& checker, ErrorPolicyType nCheckErrorPolicy = eThrow, ErrorPolicyType nReadErrorPolicy = eThrow );
    template <typename T>
    bool ReadTime( T& value, ErrorPolicyType nErrorPolicy = eThrow );    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MT_InputArchive_Logger( const MT_InputArchive_Logger& );            //!< Copy constructor
    MT_InputArchive_Logger& operator=( const MT_InputArchive_Logger& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template<typename T>
    void ProcessCheckError ( const T&, const std::string& currentAction, ErrorPolicyType );
    void ProcessError      ( const std::string& currentAction, ErrorPolicyType );
    void DoWarning         ( const std::string& currentAction) const;
    void FormatContext     ( std::ostream& )                   const;

    template<typename T>
    bool TransformTimeValue( const std::string& strValue, T& rTimeValue ) const;
    //@}

private:
    //! @name types
    //@{
    struct sContext
    {
        typedef std::list< std::string >         T_AttributesList;
        typedef T_AttributesList::const_iterator CIT_AttributesList;

        std::string      sName_;
        T_AttributesList lAttributes_;
    };

    typedef          std::list< sContext >         T_ContextList;
    typedef typename T_ContextList::const_iterator CIT_ContextList;

    typedef std::set< std::ostream* >           T_WarningStreamSet;
    typedef T_WarningStreamSet::const_iterator  CIT_WarningStreamSet;
    //@}

private:
    //! @name Member data
    //@{
    Archive            archive_;
    T_ContextList      contexts_;
    T_WarningStreamSet warningStreams_;
    //@}
};


// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename T>
struct sCheckValueGreaterOrEqual
{
    sCheckValueGreaterOrEqual( const T& minValue )
        : minValue_ ( minValue )
    {
    }

    sCheckValueGreaterOrEqual( const sCheckValueGreaterOrEqual& rhs )
        : minValue_ ( rhs.minValue_ )
    {
    }

    virtual ~sCheckValueGreaterOrEqual()
    {
    }

    bool operator () ( const T& value ) const
    {
        return value >= minValue_;
    }

    const std::string Message() const
    {
        std::stringstream str;
        str << " is not greater than " << minValue_;
        return str.str();
    }

private:
    sCheckValueGreaterOrEqual& operator = ( const sCheckValueGreaterOrEqual& );

private:
    const T minValue_;
};

template<typename T>
inline
sCheckValueGreaterOrEqual<T> CheckValueGreaterOrEqual( const T& minValue )
{
    return sCheckValueGreaterOrEqual<T>( minValue );
}

// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename T>
struct sCheckValueGreater
{
    sCheckValueGreater( const T& minValue )
        : minValue_ ( minValue )
    {
    }

    sCheckValueGreater( const sCheckValueGreater& rhs )
        : minValue_ ( rhs.minValue_ )
    {
    }

    virtual ~sCheckValueGreater()
    {
    }

    bool operator () ( const T& value ) const
    {
        return value > minValue_;
    }

    const std::string Message() const
    {
        std::stringstream str;
        str << " is not greater than " << minValue_;
        return str.str();
    }

private:
    sCheckValueGreater& operator = ( const sCheckValueGreater& );

private:
    const T minValue_;
};

template<typename T>
inline
sCheckValueGreater<T> CheckValueGreater( const T& minValue )
{
    return sCheckValueGreater<T>( minValue );
}

// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename T>
struct sCheckValueLessOrEqual
{
    sCheckValueLessOrEqual( const T& maxValue )
        : maxValue_( maxValue )
    {
    }

    sCheckValueLessOrEqual( const sCheckValueLessOrEqual& rhs )
        : maxValue_( rhs.maxValue_ )
    {
    }
    
    virtual ~sCheckValueLessOrEqual()
    {
    }
    
    bool operator () ( const T& value ) const
    {
        return value <= maxValue_;
    }
    
    const std::string Message() const
    {
        std::stringstream str;
        str << " is not less than " << maxValue_;
        return str.str();
    }
    
private:
    sCheckValueLessOrEqual& operator = ( const sCheckValueLessOrEqual& );

private:
    const T maxValue_;
};


template<typename T>
inline
sCheckValueLessOrEqual<T> CheckValueLessOrEqual( const T& maxValue )
{
    return sCheckValueLessOrEqual<T>( maxValue );
}


// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename T>
struct sCheckValueLess
{
    sCheckValueLess( const T& maxValue )
        : maxValue_( maxValue )
    {
    }

    sCheckValueLess( const sCheckValueLess& rhs )
        : maxValue_( rhs.maxValue_ )
    {
    }
    
    virtual ~sCheckValueLess()
    {
    }
    
    bool operator () ( const T& value ) const
    {
        return value < maxValue_;
    }
    
    const std::string Message() const
    {
        std::stringstream str;
        str << " is not less than " << maxValue_;
        return str.str();
    }
    
private:
    sCheckValueLess& operator = ( const sCheckValueLess& );

private:
    const T maxValue_;
};


template<typename T>
inline
sCheckValueLess<T> CheckValueLess( const T& maxValue )
{
    return sCheckValueLess<T>( maxValue );
}


// =============================================================================
// Created: JVT 2004-08-23
// =============================================================================
template <typename T>
struct sCheckValueBound : private sCheckValueGreaterOrEqual<T>
                        , private sCheckValueLessOrEqual   <T>
{
    sCheckValueBound( const T& minValue, const T& maxValue )
        : sCheckValueGreaterOrEqual<T>( minValue )
        , sCheckValueLessOrEqual<T>   ( maxValue )
    {
    }
   
    sCheckValueBound( const sCheckValueBound& rhs )
        : sCheckValueGreaterOrEqual<T>( rhs )
        , sCheckValueLessOrEqual<T>   ( rhs )
    {
    }
    
    virtual ~sCheckValueBound()
    {
    }
    
    bool operator () ( const T& value ) const
    {
        return  this->sCheckValueGreaterOrEqual<T>::operator()( value ) && this->sCheckValueLessOrEqual<T>::operator()( value );
    }
    
    const std::string Message() const
    {
        return this->sCheckValueLessOrEqual<T>::Message() + std::string( " or" ) + this->sCheckValueGreaterOrEqual<T>::Message();
    }
    
private:
    sCheckValueBound& operator = ( const sCheckValueBound& );
};

template<typename T>
inline
sCheckValueBound<T> CheckValueBound( const T& minValue, const T& maxValue )
{
    return sCheckValueBound<T>( minValue, maxValue );
}


#include "MT_InputArchive_Logger.inl"

#endif // __MT_InputArchive_Logger_h_
