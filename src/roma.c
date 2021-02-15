#include "roma.h"
#include "assert.h"

#define __GET_ROMA_IMPL(HIRAGANA, ROMAS_LEN, ...)                              \
    if (string_eq(hiragana, HIRAGANA)) {                                       \
        const static char *romas[] = {__VA_ARGS__};                            \
        const static struct StringSlice data                                   \
            = {.length = ROMAS_LEN, .pointer = romas};                         \
        return &data;                                                          \
    }

const struct StringSlice *get_roma(const char *hiragana) {
    assert(hiragana != NULL, "passed NULL to get_roma(hiragana)");

    __GET_ROMA_IMPL("Ç†", 1, "a")
    __GET_ROMA_IMPL("Ç¢", 1, "i")
    __GET_ROMA_IMPL("Ç§", 1, "u")
    __GET_ROMA_IMPL("Ç¶", 1, "e")
    __GET_ROMA_IMPL("Ç®", 1, "o")
    __GET_ROMA_IMPL("Ç©", 1, "ka")
    __GET_ROMA_IMPL("Ç´", 1, "ki")
    __GET_ROMA_IMPL("Ç≠", 1, "ku")
    __GET_ROMA_IMPL("ÇØ", 1, "ke")
    __GET_ROMA_IMPL("Ç±", 1, "ko")
    __GET_ROMA_IMPL("Ç™", 1, "ga")
    __GET_ROMA_IMPL("Ç¨", 1, "gi")
    __GET_ROMA_IMPL("ÇÆ", 1, "gu")
    __GET_ROMA_IMPL("Ç∞", 1, "ge")
    __GET_ROMA_IMPL("Ç≤", 1, "go")
    __GET_ROMA_IMPL("Ç≥", 1, "sa")
    __GET_ROMA_IMPL("Çµ", 1, "si")
    __GET_ROMA_IMPL("Ç∑", 1, "su")
    __GET_ROMA_IMPL("Çπ", 1, "se")
    __GET_ROMA_IMPL("Çª", 1, "so")
    __GET_ROMA_IMPL("Ç¥", 1, "za")
    __GET_ROMA_IMPL("Ç∂", 2, "zi", "ji")
    __GET_ROMA_IMPL("Ç∏", 1, "zu")
    __GET_ROMA_IMPL("Ç∫", 1, "ze")
    __GET_ROMA_IMPL("Çº", 1, "zo")
    __GET_ROMA_IMPL("ÇΩ", 1, "ta")
    __GET_ROMA_IMPL("Çø", 2, "ti", "chi")
    __GET_ROMA_IMPL("Ç¬", 2, "tu", "tsu")
    __GET_ROMA_IMPL("Çƒ", 1, "te")
    __GET_ROMA_IMPL("Ç∆", 1, "to")
    __GET_ROMA_IMPL("Çæ", 1, "da")
    __GET_ROMA_IMPL("Ç¿", 1, "di")
    __GET_ROMA_IMPL("Ç√", 1, "du")
    __GET_ROMA_IMPL("Ç≈", 1, "de")
    __GET_ROMA_IMPL("Ç«", 1, "do")
    __GET_ROMA_IMPL("Ç»", 1, "na")
    __GET_ROMA_IMPL("Ç…", 1, "ni")
    __GET_ROMA_IMPL("Ç ", 1, "nu")
    __GET_ROMA_IMPL("ÇÀ", 1, "ne")
    __GET_ROMA_IMPL("ÇÃ", 1, "no")
    __GET_ROMA_IMPL("ÇÕ", 1, "ha")
    __GET_ROMA_IMPL("Ç–", 1, "hi")
    __GET_ROMA_IMPL("Ç”", 2, "hu", "fu")
    __GET_ROMA_IMPL("Ç÷", 1, "he")
    __GET_ROMA_IMPL("ÇŸ", 1, "ho")
    __GET_ROMA_IMPL("ÇŒ", 1, "ba")
    __GET_ROMA_IMPL("Ç—", 1, "bi")
    __GET_ROMA_IMPL("Ç‘", 1, "bu")
    __GET_ROMA_IMPL("Ç◊", 1, "be")
    __GET_ROMA_IMPL("Ç⁄", 1, "bo")
    __GET_ROMA_IMPL("Çœ", 1, "pa")
    __GET_ROMA_IMPL("Ç“", 1, "pi")
    __GET_ROMA_IMPL("Ç’", 1, "pu")
    __GET_ROMA_IMPL("Çÿ", 1, "pe")
    __GET_ROMA_IMPL("Ç€", 1, "po")
    __GET_ROMA_IMPL("Ç‹", 1, "ma")
    __GET_ROMA_IMPL("Ç›", 1, "mi")
    __GET_ROMA_IMPL("Çﬁ", 1, "mu")
    __GET_ROMA_IMPL("Çﬂ", 1, "me")
    __GET_ROMA_IMPL("Ç‡", 1, "mo")
    __GET_ROMA_IMPL("Ç‚", 1, "ya")
    __GET_ROMA_IMPL("Ç‰", 1, "yu")
    __GET_ROMA_IMPL("ÇÊ", 1, "yo")
    __GET_ROMA_IMPL("ÇÁ", 1, "ra")
    __GET_ROMA_IMPL("ÇË", 1, "ri")
    __GET_ROMA_IMPL("ÇÈ", 1, "ru")
    __GET_ROMA_IMPL("ÇÍ", 1, "re")
    __GET_ROMA_IMPL("ÇÎ", 1, "ro")
    __GET_ROMA_IMPL("ÇÌ", 1, "wa")
    __GET_ROMA_IMPL("Ç", 1, "wo")
    __GET_ROMA_IMPL("ÇÒ", 1, "nn")
    __GET_ROMA_IMPL("Å[", 1, "-")

    failure("couldn't find such character in get_roma: %s", hiragana);
}
