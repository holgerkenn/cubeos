#ifndef _TESTMAK_H
#define _TESTMAK_H

#define failif(e,m) if (e) { printf(" %s in %s:%d\n   %s\n\n",#e,__FILE__,__LINE__,m);return(-1);}
#define checkif(e,m) if (!(e)) { printf(" check (%s) in %s:%d failed\n   %s\n\n",#e,__FILE__,__LINE__,m);return(-1);}

#endif
