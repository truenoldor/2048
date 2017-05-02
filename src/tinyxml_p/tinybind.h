/*
www.sourceforge.net/projects/tinybind
Original code copyright (c) 2004 IMVU, Inc (author: eric@imvu.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

// WARNING: current modification of TinyBind needs STL anyway

// WARNING: // MY ADDON // don't save default optional members // be Emille

#ifndef TINYBIND_INCLUDED
#define TINYBIND_INCLUDED

#include "tinyxml.h"

#include <vector>
#include <list>

void ConvertFromString( char const* strIn, std::string* dataOut );
void ConvertFromString( char const* strIn, int* dataOut );
void ConvertFromString( char const* s, float* f);
void ConvertFromString( char const* s, double* f);
void ConvertFromString( char const*, void** );
void ConvertFromString( char const * strIn, unsigned int * dataOut );
void ConvertFromString( char const * strIn, short * dataOut );
void ConvertFromString( char const * strIn, unsigned short * dataOut );
void ConvertFromString( char const* strIn, const char * * dataOut );
void ConvertFromString( char const * strIn, bool * dataOut );

class IdentityBase
{
 public:
};

template< class T >
class Identity : public IdentityBase
{
 public:
  typedef T type;

};

template< class T >
inline bool IsEqual(const T & a, const T & b)
{	
	bool bIs = ( !memcmp( (void *)&a, (void *)&b, sizeof( T ) ) );
	return bIs;
}

inline bool IsEqual (const std::string & a, const std::string & b)
{
	return    ( a.length() == b.length() )				// optimization on some platforms
		&& ( strcmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}

inline bool IsEqual (const std::string & a, const char* b)
{
	return strcmp(a.c_str(), b) == 0;
}

inline bool IsEqual (const char* a, const std::string & b)
{
	return b == a;
}

enum MemberSerializeFlags {
  MemberSerializeFlagsNone = 0,
  MemberOptional = 1,
};

//class Tag
//{
// public:
//  // support up to 3 tags
//  char const * tag_;
//
//
//  Tag( char const * tagOne = NULL ) {
//    tag_ = tagOne;
//  }
//};

//struct SerializeParams {
//  Tag tag_;
//};

template<class T>
class TiXmlBinding
{
 public:
//    virtual bool fromXml( TiXmlElement const & elem, T * data, SerializeParams const & ) const = 0;
    virtual bool fromXml( TiXmlElement const & elem, T * data ) const = 0;
  virtual bool intoXml( TiXmlElement * elem, T const & data/*, SerializeParams const &*/ ) const = 0;
};

template<class T>
bool
BindToXml( TiXmlElement * elemOut, T const & dataIn )
{
  TiXmlBinding<T> const * binding = GetTiXmlBinding( dataIn, Identity<T>() );
  //SerializeParams params;
  return binding->intoXml( elemOut, dataIn/*, params*/ );
}

template<class T>
bool
BindFromXml( TiXmlElement const & elemIn, T * dataOut )
{
  TiXmlBinding<T> const * binding = GetTiXmlBinding( *dataOut, Identity<T>() );
  //SerializeParams params;
    return binding->fromXml( elemIn, dataOut);//, params );
}


template<class T>
class IMemberHolder
{
 public:
  MemberSerializeFlags flags_;
  char const * tag_;
//  SerializeParams params_;

  void setFlags( MemberSerializeFlags f ) {
    flags_ = f;
  }

//  SerializeParams const & params() {
//    params_.tag_ = tag_;
//    return params_;
//  }
  
  virtual char const * tag() { return tag_; }
  
  virtual void fromXml( T * ) = 0;							// set default value // MY ADDON
  virtual bool fromXml( TiXmlElement const &, T * ) = 0;	// set value from XML
  virtual bool intoXml( TiXmlElement *, T const * ) = 0;	// put value to XML
  virtual bool isAttributeMember() = 0;
  
};



template<class T, class MT>
class IMemberValuePolicy
{
 public:
  virtual MT const & getMemberValue( T const * thisPtr ) = 0;
  virtual void setMemberValue( T * thisPtr, MT const & mv ) = 0;
};


template<class T, class MT>
  class MemberFuncHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT (T::*getter_)();
  void (T::*setter_)(MT);

  virtual MT const & getMemberValue( T const * thisPtr ) {
    static MT mv;
    mv = (const_cast<T*>(thisPtr)->*getter_)();
    return mv;
  }

  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*setter_)(mv);
  }

};

template<class T, class MT>
  class MemberFuncHolderConstRef  : public IMemberValuePolicy<T, MT>
{
 public:
  MT const & (T::*getter_)();
  void (T::*setter_)(MT const &);

  virtual MT const & getMemberValue( T const * thisPtr ) {
    return (thisPtr->*getter_)();
  }

  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*setter_)(mv);
  }
};

template<class T, class MT>
  class MemberPtrHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT T::*memberPtr_;
  virtual MT const & getMemberValue( T const * thisPtr ) { return thisPtr->*memberPtr_; }
  virtual void setMemberValue( T * thisPtr, MT const & mv ) { 
    // by casting away const here, we can support member pointers to arrays
    thisPtr->*memberPtr_ = const_cast<MT &>(mv); 
  }
};


template<class T, class MT>
  class MemberRefFuncHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT & (T::*memberRefFunc_)();
  virtual MT const & getMemberValue( T const * thisPtr ) { return (const_cast<T*>(thisPtr)->*memberRefFunc_)(); }
  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*memberRefFunc_)() = mv;
  }
};


template<class T, class MT>
  class FromXmlElement : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlElement( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }
  
  virtual void fromXml( T * thisPtr )
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
      IMemberHolder<T>::mv = MT();
      mvPolicy_->setMemberValue(thisPtr, IMemberHolder<T>::mv);
  }

  virtual bool fromXml( TiXmlElement const & elem, T * thisPtr )
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
    MT & mv = const_cast<MT &>(mvPolicy_->getMemberValue(thisPtr));
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>()  );
    if( binding->fromXml(elem, &mv, IMemberHolder<T>::params()) ) {
      mvPolicy_->setMemberValue(thisPtr, mv);
      return true;
    } else {
//	  if ( flags_ & MemberOptional )	// MY ADDON
//	  {									// MY ADDON
//	      fromXml(thisPtr );			// MY ADDON
//	  }									// MY ADDON
      return false;
    }
  }

  virtual bool intoXml( TiXmlElement * elem, T const * thisPtr)
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
	if ( IMemberHolder<T>::flags_ & MemberOptional )	// MY ADDON
	{								// MY ADDON
		if ( IsEqual( mv , MT() ) )			// MY ADDON
			return true;			// MY ADDON
	}								// MY ADDON
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>() );
    std::string oldValue = elem->Value();
    elem->SetValue( IMemberHolder<T>::tag() );
    bool ret = binding->intoXml( elem, mv/*, IMemberHolder<T>::params()*/ );
    elem->SetValue( oldValue );
    return ret;
  }

  virtual bool isAttributeMember() { return true; }
};


template<class T, class MT, class FromXmlPolicy, class MemberValuePolicy>
  class MemberHolder
{
 public:
  FromXmlPolicy xmlPolicy_;
  MemberValuePolicy mvPolicy_;

  MemberHolder()
    : xmlPolicy_((IMemberValuePolicy<T, MT> *)&mvPolicy_)
  {
  }
};



template<class T, class MT>
  class FromXmlChildElement : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlChildElement( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }

  virtual void fromXml( T * thisPtr )
  {
	  MT mv = MT();
      mvPolicy_->setMemberValue(thisPtr, mv);	
  }

  virtual bool fromXml( TiXmlElement const & elem, T * thisPtr)
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
	if( strcmp(elem.Value(), IMemberHolder<T>::tag()) == 0 ) {
	  MT mv;
      TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>()  );
        //if( binding->fromXml(elem, &mv, IMemberHolder<T>::params()) ) {
        if( binding->fromXml(elem, &mv) ) {
        mvPolicy_->setMemberValue(thisPtr, mv);
        return true;
      } else {
		return false;
      }
    } else {
		if ( IMemberHolder<T>::flags_ & MemberOptional )	// MY ADDON
		{								// MY ADDON
			//fromXml(thisPtr);			// MY ADDON
		}								// MY ADDON
		return false;
    }
  }

  virtual bool intoXml( TiXmlElement * elem, T const * thisPtr)
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
	if ( IMemberHolder<T>::flags_ & MemberOptional )		// MY ADDON
	{									// MY ADDON
		if ( IsEqual( mv, MT() ) )				// MY ADDON
			return true;				// MY ADDON
	}									// MY ADDON
	TiXmlElement child(IMemberHolder<T>::tag());
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv, Identity<MT>()  );
    if( binding->intoXml( &child, mv/*, IMemberHolder<T>::params()*/ ) ) {
      elem->InsertEndChild(child);
      return true;
    } else {
      return false;
    }
  }

  virtual bool isAttributeMember() { return false; }
};


template<class T>
char const *
ConvertToString( T const & t );

template<class T>
void
ConvertFromString( char const * strIn, T * dataOut );

template<class T, class MT>
  class FromXmlAttribute  : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlAttribute( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }

  virtual void fromXml( T * thisPtr )
  {
    MT mv;
	memset( &mv, 0, sizeof(MT) );
	mvPolicy_->setMemberValue(thisPtr, mv);
  }

  virtual bool fromXml( TiXmlElement const & elem, T * thisPtr)
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
    const char * attributeValue = elem.Attribute( IMemberHolder<T>::tag() );
    if( attributeValue && *attributeValue ) {
	  MT mv;
      ConvertFromString( attributeValue, &mv );
      mvPolicy_->setMemberValue(thisPtr, mv);
      return true;
    } else {
	  if ( IMemberHolder<T>::flags_ & MemberOptional )	// MY ADDON		test it
	  {									// MY ADDON
		//fromXml(thisPtr);				// MY ADDON
	  }									// MY ADDON
      return false;
    }
  }

  virtual bool intoXml( TiXmlElement * elem, T const * thisPtr)
  {
      //OX_ASSERT(false);// код ниже не компилируется
      //return false;
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
	if ( IMemberHolder<T>::flags_ & MemberOptional )										// MY ADDON		test it
	{																	// MY ADDON
		MT mvZero;														// MY ADDON
		memset( &mvZero, 0, sizeof(MT) );								// MY ADDON
		if ( !memcmp( (void *)&mv, (void *)&mvZero, sizeof( MT ) ) )	// MY ADDON
			return true;												// MY ADDON
	}																	// MY ADDON
    char const * attributeValue = ConvertToString( mv );
    elem->SetAttribute( IMemberHolder<T>::tag(), attributeValue );
    return true;
  }

  virtual bool isAttributeMember() { return true; }
};

template<class T, class MT>
  IMemberHolder<T> * Member(  MT T::*mp )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(MT & (T::*mp)() )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberRefFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(  MT (T::*getter)()const, void (T::*setter)(MT) )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

// BEGIN ATTRIBUTE MAKERS
  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT T::*mp )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT (T::*getter)() const, void (T::*setter)(MT) )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }
// END ATTRIBUTE MAKERS

// BEGIN PEER MAKERS
  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(  MT T::*mp )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(MT & (T::*mp)() )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberRefFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
  }

// END PEER MAKERS

template<class T>
class MemberTiXmlBinding : public TiXmlBinding<T>
{
 private:
  std::vector<IMemberHolder<T> *> members_;
  
 public:
  bool empty() const
  {
    return members_.empty();
  }

  IMemberHolder<T> * AddMember( const char *tag, IMemberHolder<T> * mph )
  {
    mph->flags_ = MemberSerializeFlagsNone;
    mph->tag_ = tag;
    members_.push_back( mph );
    return mph;
  }

//    virtual bool fromXml( TiXmlElement const & elem, T * data, SerializeParams const & params ) const
    virtual bool fromXml( TiXmlElement const & elem, T * data ) const
  {
      TiXmlElement const * child = elem.FirstChildElement();
      for( int i = 0; i < (int)members_.size(); i++ ) {
          IMemberHolder<T> * mph = members_[i];
          bool error = false;
          
          bool ret;
          if( mph->isAttributeMember() ) {
              ret = mph->fromXml( elem, data );
          } else {
              if( !child ) {
                  if ( mph->flags_ & MemberOptional )
                  {
                      // no problem
                      continue;
                  }
                  // error
                  return false;
              }
              ret = mph->fromXml( *child, data );
          }
          error = !ret;
          if( mph->isAttributeMember() ) {
          } else if( !error ) {
              child = child->NextSiblingElement();
          }
          
          if( error ) {
              if( mph->isAttributeMember() ) {
                  // no problem
                  continue;
              } else {
                  if ( mph->flags_ & MemberOptional )
                      // no problem
                      continue;
                  // error
                  return false;
              }
          }
      }
      return true;
  }
  
  virtual bool intoXml( TiXmlElement * elem, T const & data/*, SerializeParams const &*/ ) const
  {
    for( int i = 0; i < (int)members_.size(); i++ ) {
      IMemberHolder<T> * mph = members_[i];
      mph->intoXml( elem, &data );		
    }
    return true;
  }
};


template<class T>
class GenericTiXmlBinding : public TiXmlBinding<T>
{
 public:
  virtual bool fromXml( TiXmlElement const & elem, T * data/*, SerializeParams const &*/ ) const
  {
    const TiXmlNode * node = elem.FirstChild();
    const TiXmlText * nodedata = node->ToText();
    ConvertFromString( nodedata->Value(), data );
    return true;
  }
  
  virtual bool intoXml( TiXmlElement * elem, T const & data/*, SerializeParams const &*/ ) const
  {
    TiXmlText textData( ConvertToString( data ) );
    elem->InsertEndChild( textData );
    return true;
  }
};

template<class T, class VecT>
class StlContainerTiXmlBinding : public TiXmlBinding<VecT>
{
 public:

  char const * subTag_;
  bool useSubTag_;
  char const * sizeAttributeName_;
  StlContainerTiXmlBinding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
    :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }
    
//    virtual bool fromXml( TiXmlElement const & elem, VecT * data, SerializeParams const & params ) const
    virtual bool fromXml( TiXmlElement const & elem, VecT * data ) const
    {
      data->clear();
      TiXmlElement const * child;
      child = elem.FirstChildElement();
      if( sizeAttributeName_ ) {
        int sz = 0;
        ConvertFromString( elem.Attribute(sizeAttributeName_), &sz );
        if( sz ) {
          //data->reserve(sz);
        }
      }
      while(child) {
        T * value = new T();
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
          bool ret = binding->fromXml( *child, value);//, params );
        data->push_back(*value);
        if( ! ret ) {
          return false;
        }
        child = child->NextSiblingElement();
      }
      return true;
    }
    
    virtual bool intoXml( TiXmlElement * elem, VecT const & data/*, SerializeParams const & params*/ ) const
    {
      if( sizeAttributeName_ ) {
          //elem->SetAttribute( sizeAttributeName_, ConvertToString( data.size() ) );
          elem->SetAttribute( sizeAttributeName_, data.size() );
      }
      for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
        T const & value = *i;
        TiXmlBinding<T> const * binding = GetTiXmlBinding( value,  Identity<T>()  );
        char const * tag;
        if( useSubTag_ ) {
          tag = subTag_;
        } else {
          tag = elem->Value();
        }
        TiXmlElement child(tag);
        if( ! binding->intoXml( &child, value/*, params*/ ) ) {
          return false;
        }
        elem->InsertEndChild(child);
      }
      return true;
    }
  };

template<class T, class VecT>
class StlContainerPtrBinding : public TiXmlBinding<VecT>
{
 public:
    
  char const * subTag_;
  bool useSubTag_;
  char const * sizeAttributeName_;
  StlContainerPtrBinding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
    :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool fromXml( TiXmlElement const & elem, VecT * data/*, SerializeParams const & params*/ ) const
    {
      data->clear();
      TiXmlElement const * child;
      child = elem.FirstChildElement();
      if( sizeAttributeName_ ) {
        int sz = 0;
        ConvertFromString( elem.Attribute(sizeAttributeName_), &sz );
        if( sz ) {
          //data->reserve(sz);
        }
      }
      while(child) {
        T * value = new T();
        if( !value->create() ) {
          return false;
        }
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
        bool ret = binding->fromXml( *child, value/*, params*/ );
        data->push_back(value);
        if( ! ret ) {
          return false;
        }
        child = child->NextSiblingElement();
      }
      return true;
    }
    
    virtual bool intoXml( TiXmlElement * elem, VecT const & data/*, SerializeParams const & params */) const
    {
      if( sizeAttributeName_ ) {
          elem->SetAttribute( sizeAttributeName_, data.size() );
          //elem->SetAttribute( sizeAttributeName_, ConvertToString( data.size() ) );
      }
      for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
        T const * value = *i;
        if( ! value ) {
          continue;
        }
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
        char const * tag;
        if( useSubTag_ ) {
          tag = subTag_;
        } else {
          tag = elem->Value();
        }
        TiXmlElement child(tag);
        if( ! binding->intoXml( &child, *value/*, params*/ ) ) {
          return false;
        }
        elem->InsertEndChild(child);
      }
      return true;
    }
};

// generic abstract declaration

template<class T>
TiXmlBinding<T> const *
GetTiXmlBinding( T const &, IdentityBase  );

// stl abstract declaration // here VecT may be std::vector<T> or std::list<T>

template<class T, class VecT>
TiXmlBinding<VecT> const *
GetTiXmlBinding( VecT const &, Identity<VecT>  );
/*
template<class T, class VecT>
TiXmlBinding<VecT> const *
GetTiXmlBinding( std::vector<T> const &, Identity<VecT>  );

template<class T, class VecT>
TiXmlBinding<VecT> const *
GetTiXmlBinding( std::list<T> const &, Identity<VecT>  );
*/

// defines for particular declarations and definitions

#define DECLARE_GENERIC_BIND( xType ) \
	TiXmlBinding< xType > const * \
	GetTiXmlBinding( xType const &, IdentityBase );
#define DEFINE_GENERIC_BIND( gType ) \
	TiXmlBinding<gType> const * \
	GetTiXmlBinding( gType const &, IdentityBase  ) \
{ \
	static GenericTiXmlBinding<gType> binding; \
	return &binding; \
}

#define DECLARE_MEMBER_BIND( xType ) \
	TiXmlBinding< xType > const * \
	GetTiXmlBinding( xType const &, Identity<xType> );

#define DEFINE_MEMBER_BIND_BEGIN( xType ) \
	TiXmlBinding<xType> const * \
	GetTiXmlBinding( xType const &, Identity<xType> ) \
	{ \
	  static MemberTiXmlBinding<xType> binding; \
	  typedef xType TM; \
	  if( binding.empty() ) {

#define BIND_ADD_MEMBER binding.AddMember
#define _OPTIONAL ->setFlags( MemberOptional ) // <- uncomment for testing of _OPTIONAL modifier
#define DEFINE_MEMBER_BIND_END \
	  } \
	  return &binding; \
}

#define DECLARE_STL_CONT_BIND DECLARE_MEMBER_BIND
#define DEFINE_STL_CONT_BIND( vType, eType, use_subname, elem_subname, size_name ) \
	TiXmlBinding< vType > const * \
	GetTiXmlBinding( vType const &, Identity< vType > ) \
	{ \
		static StlContainerTiXmlBinding< eType, vType > binding( (use_subname), (elem_subname), (size_name) ); \
		return &binding; \
	}

// generic

TiXmlBinding< float > const * GetTiXmlBinding( float const &, IdentityBase );
TiXmlBinding< double > const * GetTiXmlBinding( double const &, IdentityBase );
TiXmlBinding< int > const * GetTiXmlBinding( int const &, IdentityBase );
TiXmlBinding< unsigned int > const * GetTiXmlBinding( unsigned int const &, IdentityBase );
TiXmlBinding< short > const * GetTiXmlBinding( short const &, IdentityBase );
TiXmlBinding< unsigned short > const * GetTiXmlBinding( unsigned short const &, IdentityBase );
TiXmlBinding< bool > const * GetTiXmlBinding( bool const &, IdentityBase );
TiXmlBinding< void *  > const * GetTiXmlBinding( void *  const &, IdentityBase );
TiXmlBinding< char const * > const * GetTiXmlBinding( char const * const &, IdentityBase );
TiXmlBinding< std::string > const * GetTiXmlBinding( std::string const &, IdentityBase );

#endif
