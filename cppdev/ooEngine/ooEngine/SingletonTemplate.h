#ifndef _SINGLETONTEMPLATE_H
#define _SINGLETONTEMPLATE_H

template< typename T > class SingletonTemplate
{
public:
    static T* GetInstance()
    {
        if( instance_ == 0 )
            instance_ = new T();
        return instance_;
    }
    static void Kill()
    {
        if( instance_ != 0 )
            delete _Instance;
        instance_ = 0;
    }

protected:
    SingletonTemplate< T >(){};
    SingletonTemplate< T >( const T& rhs ){};
    T& operator=( const T& rhs ){};
	virtual ~SingletonTemplate< T >(){};
	static T* instance_;
};

template< typename T > T* SingletonTemplate< T >::instance_ = 0;

#endif // SingletonTemplate.h