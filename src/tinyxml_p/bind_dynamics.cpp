#include "bind_dynamics.h"

DEFINE_MEMBER_BIND_BEGIN( TTest )
	binding.AddMember( "test1", Member(&TM::val1) );	
	binding.AddMember( "test2", Member(&TM::val2) );	
	  } 
	  return &binding; 
};

DEFINE_STL_CONT_BIND( TVecTest, TTest, true, "test_obj", "size" );

DEFINE_MEMBER_BIND_BEGIN( TTests )
	binding.AddMember( "testing", Member(&TM::testing) );	
	  } 
	  return &binding; 
};


