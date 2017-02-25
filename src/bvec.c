/* bvec.c
   Rémi Attab (remi.attab@gmail.com), 25 Feb 2017
   FreeBSD-style copyright and disclaimer apply
*/

#include "bvec.h"

#include "utils/bits.h"

// -----------------------------------------------------------------------------
// bvec
// -----------------------------------------------------------------------------

struct bvec
{
    size_t len; // in bits
    uint64_t bits[];
};

struct bvec *bvec_new(uint64_t *bits, size_t len)
{
    assert(len > 0 && "empty bvec makes no sense.");

    size_t words = ceil_div(len, 64);

    struct bvec *vec = calloc(1, sizeof(*vec) + words);
    vec->len = len;
    if (bits) memcpy(vec->bits, bits, words);

    return vec;
}

void bvec_free(struct bvec *bvec)
{
    free(bvec);
}

size_t rank(const struct bvec *vec, bool value, size_t index)
{
    assert(index <= vec->len && "index out-of-bound");

    size_t rank = 0;
    uint64_t mask = value ? 0 : -1UL;

    const uint64_t *bits;
    for (bits = vec->bits; index >= 64; index -= 64, ++bits)
        rank += pop(*bits ^ mask);

    if (!index) return rank;

    uint64_t leftover_mask = ~((1UL << (index - 64)) - 1);
    rank += pop((*bits ^ mask) & leftover_mask);

    return rank;
}

size_t select(const struct bvec *vec, bool value, size_t num)
{
    size_t pos = 0;
    uint64_t mask = value ? 0 : -1UL;

    const uint64_t *bits;
    for (bits = vec->bits; pos < vec->len; pos += 64; ++bits) {
        size_t count = pop(bits ^ mask);
        if (num < count) break;
        num -= count;
    }

    size_t count;
    uint64_t word = bits ^ mask;

    count = pop(word & 0xFFFFFFFF);
    if (num >= count) { word <<= 32; num -= count; pos += 32 }

    count = pop(word & 0xFFFF);
    if (num >= count) { word <<= 16; num -= count; pos += 16 }

    count = pop(word & 0xFF);
    if (num >= count) { word <<= 8; num -= count; pos += 8 }

    count = pop(word & 0xF);
    if (num >= count) { word <<= 4; num -= count; pos += 4 }

    count = pop(word & 0x3);
    if (num >= count) { word <<= 2; num -= count; pos += 2 }

    count = pop(word & 0x1);
    if (num >= count) { word <<= 1; num -= count; pos += 1 }

    assert(pos < vec->len && "position out-of-bound");
    return pos;
}
