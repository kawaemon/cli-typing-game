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

    // 入力されたひらがなに対して対応するローマ字の一覧を返却する。
    // 同じようなコードが並ぶため、マクロを使って省略する。
    __GET_ROMA_IMPL("あ", 1, "a")
    __GET_ROMA_IMPL("い", 1, "i")
    __GET_ROMA_IMPL("う", 1, "u")
    __GET_ROMA_IMPL("え", 1, "e")
    __GET_ROMA_IMPL("お", 1, "o")
    __GET_ROMA_IMPL("か", 1, "ka")
    __GET_ROMA_IMPL("き", 1, "ki")
    __GET_ROMA_IMPL("く", 1, "ku")
    __GET_ROMA_IMPL("け", 1, "ke")
    __GET_ROMA_IMPL("こ", 1, "ko")
    __GET_ROMA_IMPL("が", 1, "ga")
    __GET_ROMA_IMPL("ぎ", 1, "gi")
    __GET_ROMA_IMPL("ぐ", 1, "gu")
    __GET_ROMA_IMPL("げ", 1, "ge")
    __GET_ROMA_IMPL("ご", 1, "go")
    __GET_ROMA_IMPL("さ", 1, "sa")
    __GET_ROMA_IMPL("し", 1, "si")
    __GET_ROMA_IMPL("す", 1, "su")
    __GET_ROMA_IMPL("せ", 1, "se")
    __GET_ROMA_IMPL("そ", 1, "so")
    __GET_ROMA_IMPL("ざ", 1, "za")
    __GET_ROMA_IMPL("じ", 2, "zi", "ji")
    __GET_ROMA_IMPL("ず", 1, "zu")
    __GET_ROMA_IMPL("ぜ", 1, "ze")
    __GET_ROMA_IMPL("ぞ", 1, "zo")
    __GET_ROMA_IMPL("た", 1, "ta")
    __GET_ROMA_IMPL("ち", 2, "ti", "chi")
    __GET_ROMA_IMPL("つ", 2, "tu", "tsu")
    __GET_ROMA_IMPL("て", 1, "te")
    __GET_ROMA_IMPL("と", 1, "to")
    __GET_ROMA_IMPL("だ", 1, "da")
    __GET_ROMA_IMPL("ぢ", 1, "di")
    __GET_ROMA_IMPL("づ", 1, "du")
    __GET_ROMA_IMPL("で", 1, "de")
    __GET_ROMA_IMPL("ど", 1, "do")
    __GET_ROMA_IMPL("な", 1, "na")
    __GET_ROMA_IMPL("に", 1, "ni")
    __GET_ROMA_IMPL("ぬ", 1, "nu")
    __GET_ROMA_IMPL("ね", 1, "ne")
    __GET_ROMA_IMPL("の", 1, "no")
    __GET_ROMA_IMPL("は", 1, "ha")
    __GET_ROMA_IMPL("ひ", 1, "hi")
    __GET_ROMA_IMPL("ふ", 2, "hu", "fu")
    __GET_ROMA_IMPL("へ", 1, "he")
    __GET_ROMA_IMPL("ほ", 1, "ho")
    __GET_ROMA_IMPL("ば", 1, "ba")
    __GET_ROMA_IMPL("び", 1, "bi")
    __GET_ROMA_IMPL("ぶ", 1, "bu")
    __GET_ROMA_IMPL("べ", 1, "be")
    __GET_ROMA_IMPL("ぼ", 1, "bo")
    __GET_ROMA_IMPL("ぱ", 1, "pa")
    __GET_ROMA_IMPL("ぴ", 1, "pi")
    __GET_ROMA_IMPL("ぷ", 1, "pu")
    __GET_ROMA_IMPL("ぺ", 1, "pe")
    __GET_ROMA_IMPL("ぽ", 1, "po")
    __GET_ROMA_IMPL("ま", 1, "ma")
    __GET_ROMA_IMPL("み", 1, "mi")
    __GET_ROMA_IMPL("む", 1, "mu")
    __GET_ROMA_IMPL("め", 1, "me")
    __GET_ROMA_IMPL("も", 1, "mo")
    __GET_ROMA_IMPL("や", 1, "ya")
    __GET_ROMA_IMPL("ゆ", 1, "yu")
    __GET_ROMA_IMPL("よ", 1, "yo")
    __GET_ROMA_IMPL("ら", 1, "ra")
    __GET_ROMA_IMPL("り", 1, "ri")
    __GET_ROMA_IMPL("る", 1, "ru")
    __GET_ROMA_IMPL("れ", 1, "re")
    __GET_ROMA_IMPL("ろ", 1, "ro")
    __GET_ROMA_IMPL("わ", 1, "wa")
    __GET_ROMA_IMPL("を", 1, "wo")
    __GET_ROMA_IMPL("ん", 1, "nn")
    __GET_ROMA_IMPL("ー", 1, "-")

    failure("couldn't find such character in get_roma: %s", hiragana);
}
