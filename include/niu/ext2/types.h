#if(UINT_MAX == 0xFFFFFFFFUL)
	typedef unsigned int atleast4;
#elif(ULONG_MAX == 0xFFFFFFFFUL)
	#warning "atleas4 typedefed as 'long' "
	"(are you using a 16-bit compiler?)"
	typedef ulong atleast4;
#else
	#error "atleast4 not typedefed"
#endif

