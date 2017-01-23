
/*
** Xtank
**
** Copyright 1988 by Terry Donahue
**
** sysdep.h
*/


/*
$Author: lidl $
$Id: sysdep.h,v 1.1.1.2 1995/02/01 00:28:53 lidl Exp $
*/

#ifndef SYSDEP_H
#define SYSDEP_H

#ifdef i860
#define STRUCT_ASSIGN(a,b,c) { memcpy(&(a), &(b), sizeof(c)); }
#else
#define STRUCT_ASSIGN(a,b,c) { a = b; }
#endif


#if defined(sun) || defined(hpux) || defined(apollo) || defined(mips) || defined(MOTOROLA) || defined(i860) || defined(mmax) || defined(__alpha) || defined(__bsdi__)
/* Avoid domain errors when both x and y are 0 */
#define ATAN2(_Y,_X) ((_X)==0 && (_Y)==0 ? 0.0 : atan2((double)_Y, (double)_X))
#else
#define ATAN2(Y,X) atan2((double)Y, (double)X)
#endif


#if defined(AMIGA) || defined(MOTOROLA) || defined(i860) || defined(mmax)
/* drem() not available so replace with wrapper around fmod() */
static double temp_drem;

#define drem(a,b) ((temp_drem = fmod(a,b)) > (b)/2 ? temp_drem-(b) : temp_drem)
#endif


/* If you have no drem or fmod try this... */
#if defined(sequent)
#define drem(a,b) (((double)(a))-(int)((((double)(a))/((double)(b)))+((double) 0.5))*(b))
#endif

/* If you have no cbrt in your library, try this */
#if defined(sequent) || defined(__hpux) || defined(linux)
#define cbrt(n) pow(n, 1.0/3.0)
#endif

#if defined(mips) && defined(ultrix)
/* DEC bites again - broken math.h */
extern double fmod(), drem();

/* There's a rumor that DEC's drem has a bug.  Consider using the macro above. */
#endif

/*
** Ultrix doesn't have a prototype for this, so we must.  Typical.
*/
#if defined(ultrix) || defined(sequent)
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
double rint P_((double x));
#undef P_
#endif

/*
** Some ANSI compilers bitch and moan about float != double, and then
** also have different arguement promotion rules.
*/

#if defined(i860) || defined(sparc)
#define FLOAT double
#else
#define FLOAT float
#endif

#if defined(SYSV) || defined(SVR4)
#ifndef sgi
#define random lrand48
#endif
#define srandom srand48
#if defined(__hpux) || defined(__STDC__) || defined(STDC_LIBRARIES)
#define bcopy(s,d,n) memmove(d,s,n)
#else
#define bcopy(s,d,n) memcpy(d,s,n)
#endif
#define bzero(d,n) memset(d,0,n)
#define bcmp(a,b,n) memcmp(a,b,n)
#define index(s,c) strchr(s,c)
#ifndef sgi
#define rindex(s,c) strrchr(s,c)
#endif
#endif


#endif /* SYSDEP_H */
