#include "roma.h"
#include "assert.h"

#define __GET_ROMA_IMPL(HIRAGANA, ROMAS_LEN, ...)                              \
    if (string_eq(hiragana, HIRAGANA)) {                                       \
        const static char *romas[] = {__VA_ARGS__};                            \
        const static struct StringSlice data = {.length = ROMAS_LEN,           \
                                                .pointer = romas};             \
        return &data;                                                          \
    }

const struct StringSlice *get_roma(const char *hiragana) {
    assert(hiragana != NULL, "passed NULL to get_roma(hiragana)");

    __GET_ROMA_IMPL("‚ ", 1, "a")
    __GET_ROMA_IMPL("‚¢", 1, "i")
    __GET_ROMA_IMPL("‚¤", 1, "u")
    __GET_ROMA_IMPL("‚¦", 1, "e")
    __GET_ROMA_IMPL("‚¨", 1, "o")
    __GET_ROMA_IMPL("‚©", 1, "ka")
    __GET_ROMA_IMPL("‚«", 1, "ki")
    __GET_ROMA_IMPL("‚­", 1, "ku")
    __GET_ROMA_IMPL("‚¯", 1, "ke")
    __GET_ROMA_IMPL("‚±", 1, "ko")
    __GET_ROMA_IMPL("‚ª", 1, "ga")
    __GET_ROMA_IMPL("‚¬", 1, "gi")
    __GET_ROMA_IMPL("‚®", 1, "gu")
    __GET_ROMA_IMPL("‚°", 1, "ge")
    __GET_ROMA_IMPL("‚²", 1, "go")
    __GET_ROMA_IMPL("‚³", 1, "sa")
    __GET_ROMA_IMPL("‚µ", 1, "si")
    __GET_ROMA_IMPL("‚·", 1, "su")
    __GET_ROMA_IMPL("‚¹", 1, "se")
    __GET_ROMA_IMPL("‚»", 1, "so")
    __GET_ROMA_IMPL("‚´", 1, "za")
    __GET_ROMA_IMPL("‚¶", 2, "zi", "ji")
    __GET_ROMA_IMPL("‚¸", 1, "zu")
    __GET_ROMA_IMPL("‚º", 1, "ze")
    __GET_ROMA_IMPL("‚¼", 1, "zo")
    __GET_ROMA_IMPL("‚½", 1, "ta")
    __GET_ROMA_IMPL("‚¿", 2, "ti", "chi")
    __GET_ROMA_IMPL("‚Â", 2, "tu", "tsu")
    __GET_ROMA_IMPL("‚Ä", 1, "te")
    __GET_ROMA_IMPL("‚Æ", 1, "to")
    __GET_ROMA_IMPL("‚¾", 1, "da")
    __GET_ROMA_IMPL("‚À", 1, "di")
    __GET_ROMA_IMPL("‚Ã", 1, "du")
    __GET_ROMA_IMPL("‚Å", 1, "de")
    __GET_ROMA_IMPL("‚Ç", 1, "do")
    __GET_ROMA_IMPL("‚È", 1, "na")
    __GET_ROMA_IMPL("‚É", 1, "ni")
    __GET_ROMA_IMPL("‚Ê", 1, "nu")
    __GET_ROMA_IMPL("‚Ë", 1, "ne")
    __GET_ROMA_IMPL("‚Ì", 1, "no")
    __GET_ROMA_IMPL("‚Í", 1, "ha")
    __GET_ROMA_IMPL("‚Ð", 1, "hi")
    __GET_ROMA_IMPL("‚Ó", 2, "hu", "fu")
    __GET_ROMA_IMPL("‚Ö", 1, "he")
    __GET_ROMA_IMPL("‚Ù", 1, "ho")
    __GET_ROMA_IMPL("‚Î", 1, "ba")
    __GET_ROMA_IMPL("‚Ñ", 1, "bi")
    __GET_ROMA_IMPL("‚Ô", 1, "bu")
    __GET_ROMA_IMPL("‚×", 1, "be")
    __GET_ROMA_IMPL("‚Ú", 1, "bo")
    __GET_ROMA_IMPL("‚Ï", 1, "pa")
    __GET_ROMA_IMPL("‚Ò", 1, "pi")
    __GET_ROMA_IMPL("‚Õ", 1, "pu")
    __GET_ROMA_IMPL("‚Ø", 1, "pe")
    __GET_ROMA_IMPL("‚Û", 1, "po")
    __GET_ROMA_IMPL("‚Ü", 1, "ma")
    __GET_ROMA_IMPL("‚Ý", 1, "mi")
    __GET_ROMA_IMPL("‚Þ", 1, "mu")
    __GET_ROMA_IMPL("‚ß", 1, "me")
    __GET_ROMA_IMPL("‚à", 1, "mo")
    __GET_ROMA_IMPL("‚â", 1, "ya")
    __GET_ROMA_IMPL("‚ä", 1, "yu")
    __GET_ROMA_IMPL("‚æ", 1, "yo")
    __GET_ROMA_IMPL("‚ç", 1, "ra")
    __GET_ROMA_IMPL("‚è", 1, "ri")
    __GET_ROMA_IMPL("‚é", 1, "ru")
    __GET_ROMA_IMPL("‚ê", 1, "re")
    __GET_ROMA_IMPL("‚ë", 1, "ro")
    __GET_ROMA_IMPL("‚í", 1, "wa")
    __GET_ROMA_IMPL("‚ð", 1, "wo")
    __GET_ROMA_IMPL("‚ñ", 1, "nn")

    failure("couldn't find such character in get_roma", hiragana);
}
