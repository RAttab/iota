/* bvec.h
   Rémi Attab (remi.attab@gmail.com), 25 Feb 2017
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once


// -----------------------------------------------------------------------------
// bvec
// -----------------------------------------------------------------------------

struct bvec;

struct bvec *bvec_new(uint64_t *bits, size_t len);
void bvec_free(struct bvec *bvec);

size_t rank(const struct bvec *vec, bool value, size_t index);
size_t select(const struct bvec *vec, bool value, size_t num);
