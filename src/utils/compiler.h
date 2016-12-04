/* compiler.h
   Rémi Attab (remi.attab@gmail.com), 03 May 2014
   FreeBSD-style copyright and disclaimer apply

   Compiler related utilities and attributes.
*/

#pragma once

// -----------------------------------------------------------------------------
// attributes
// -----------------------------------------------------------------------------

#define iota_unused       __attribute__((unused))
#define iota_noreturn     __attribute__((noreturn))
#define iota_align(x)     __attribute__((aligned(x)))
#define iota_packed       __attribute__((__packed__))
#define iota_pure         __attribute__((pure))
#define iota_printf(x,y)  __attribute__((format(printf, x, y)))
#define iota_malloc       __attribute__((malloc))
#define iota_noinline     __attribute__((noinline))
#define iota_likely(x)    __builtin_expect(x, 1)
#define iota_unlikely(x)  __builtin_expect(x, 0)


// -----------------------------------------------------------------------------
// builtin
// -----------------------------------------------------------------------------

#define iota_unreachable() __builtin_unreachable()


// -----------------------------------------------------------------------------
// asm
// -----------------------------------------------------------------------------

#define iota_asm __asm__

#define iota_no_opt()         iota_asm volatile ("")
#define iota_no_opt_val(x)    iota_asm volatile ("" : "+r" (x))
#define iota_no_opt_clobber() iota_asm volatile ("" : : : "memory")
