// Scintilla source code edit control
/**
 * @file LexFSharp.cxx
 * Lexer for F#
 * Copyright 2021 by Robert Di Pardo <dipardo.r@gmail.com>
 * Parts of LexerFSharp::Lex were adapted from LexCaml.cxx by Robert Roessler ("RR").
 * Parts of LexerFSharp::Fold were adapted from LexCPP.cxx by Neil Hodgson and Udo Lechner.
 * The License.txt file describes the conditions under which this software may be distributed.
 */
// clang-format off
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <string>
#include <map>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"
#include "OptionSet.h"
#include "DefaultLexer.h"
// clang-format on

using namespace Scintilla;

static const char *lexerName = "fsharp";
static constexpr int WORDLIST_SIZE = 5;
static const char *const fsharpWordLists[] = {
	"standard language keywords",
	"core functions, including most functions in the FSharp.Collections namespace",
	"built-in types, core namespaces, modules",
	"optional",
	"optional",
	nullptr,
};

namespace {

struct OptionsFSharp {
	bool fold;
	bool foldCompact;
	bool foldComment;
	bool foldCommentOCamlStyle;
	bool foldCommentMultiLine;
	bool foldPreprocessor;
	bool foldImports;
	OptionsFSharp() {
		fold = true;
		foldCompact = true;
		foldComment = true;
		foldCommentOCamlStyle = true;
		foldCommentMultiLine = true;
		foldPreprocessor = false;
		foldImports = true;
	}
};

struct OptionSetFSharp : public OptionSet<OptionsFSharp> {
	OptionSetFSharp() {
		DefineProperty("fold", &OptionsFSharp::fold);
		DefineProperty("fold.compact", &OptionsFSharp::foldCompact);
		DefineProperty("fold.comment", &OptionsFSharp::foldComment,
				   "Setting this option to 0 disables comment folding in F# files.");

		DefineProperty("fold.fsharp.comment.ocaml", &OptionsFSharp::foldCommentOCamlStyle,
				   "Setting this option to 0 disables folding of OCaml-style comments in F# files when "
				   "fold.comment=1.");

		DefineProperty("fold.fsharp.comment.multiline", &OptionsFSharp::foldCommentMultiLine,
				   "Setting this option to 0 disables folding of grouped line comments in F# files when "
				   "fold.comment=1.");

		DefineProperty("fold.fsharp.preprocessor", &OptionsFSharp::foldPreprocessor,
				   "Setting this option to 1 enables folding of F# compiler directives.");

		DefineProperty("fold.fsharp.imports", &OptionsFSharp::foldImports,
				   "Setting this option to 0 disables folding of F# import declarations.");

		DefineWordListSets(fsharpWordLists);
	}
};

const CharacterSet setOperators = CharacterSet(CharacterSet::setNone, "~^'-+*/%=@|&<>()[]{};,:!?");
const CharacterSet setClosingTokens = CharacterSet(CharacterSet::setNone, ")]}'\"");
const char *numericMetaChars1 = "_Ibflmnosuxy";
const char *numericMetaChars2 = "lnsy";
constexpr Sci_Position ZERO_LENGTH = -1;

struct FSharpString {
	Sci_Position startPos;
	int startChar;
	FSharpString() {
		startPos = ZERO_LENGTH;
		startChar = '"';
	}
	constexpr bool HasLength() const {
		return startPos > ZERO_LENGTH;
	}
};

class UnicodeChar {
	enum class Notation { none, asciiDec, asciiHex, utf16, utf32 };

	int8_t parseDigit(int ch, const int base = 10) {
		char buf[2] = { 0 };
		char *end = { 0 };
		*buf = static_cast<char>(ch);
		buf[1] = '\0';
		errno = 0;
		int8_t result = static_cast<int8_t>(strtol(buf, &end, base));
		result = errno != 0 ? -1 : result;
		errno = 0;
		return result;
	}

	Notation type = Notation::none;
	// single-byte Unicode char (000 - 255)
	int8_t asciiDigits[3] = { 0 };
	int8_t maxDigit = 9;
	int8_t toEnd = 0;
	bool invalid = false;

public:
	UnicodeChar() noexcept = default;
	UnicodeChar(const int prefix) {
		if (IsADigit(prefix)) {
			*asciiDigits = parseDigit(prefix);
			if (*asciiDigits >= 0 && *asciiDigits <= 2) {
				type = Notation::asciiDec;
				// count first digit as "prefix"
				toEnd = 2;
			}
		} else if (strchr("xuU", prefix)) {
			switch (prefix) {
				case 'x':
					type = Notation::asciiHex;
					toEnd = 2;
					break;
				case 'u':
					type = Notation::utf16;
					toEnd = 4;
					break;
				case 'U':
					type = Notation::utf32;
					toEnd = 8;
					break;
			}
		}
	}
	UnicodeChar &operator=(const UnicodeChar *other) {
		if (this != other) {
			type = other->type;
			delete other;
		}
		return *this;
	}
	void Parse(const int ch) noexcept {
		invalid = false;
		switch (type) {
			case Notation::asciiDec: {
				int8_t nextDigit = parseDigit(ch);
				maxDigit = (*asciiDigits < 2) ? 9 : (asciiDigits[1] <= 4) ? 9 : 5;
				if (IsADigit(ch) && asciiDigits[1] <= maxDigit && nextDigit <= maxDigit) {
					asciiDigits[1] = nextDigit;
					toEnd--;
				} else {
					invalid = true;
				}
				break;
			}
			case Notation::asciiHex:
			case Notation::utf16:
				if (IsADigit(ch, 16)) {
					toEnd--;
				} else {
					invalid = true;
				}
				break;
			case Notation::utf32:
				if ((toEnd > 6 && ch == '0') || (toEnd <= 6 && IsADigit(ch, 16))) {
					toEnd--;
				} else {
					invalid = true;
				}
				break;
			case Notation::none:
				break;
		}
	}
	constexpr bool AtEnd() noexcept {
		return invalid || type == Notation::none || (type != Notation::none && toEnd < 0);
	}
};

inline bool MatchStreamCommentStart(StyleContext &cxt) {
	// match (* ... *), but allow point-free usage of the `*` operator,
	// e.g.  List.fold (*) 1 [ 1; 2; 3 ]
	return (cxt.Match('(', '*') && cxt.GetRelative(2) != ')');
}

inline bool MatchStreamCommentEnd(StyleContext &cxt) {
	return cxt.Match(')') && cxt.chPrev == '*';
}

inline bool MatchLineComment(StyleContext &cxt) {
	// style shebang lines as comments in F# scripts:
	// https://fsharp.org/specs/language-spec/4.1/FSharpSpec-4.1-latest.pdf#page=30&zoom=auto,-98,537
	return cxt.Match('/', '/') || cxt.Match('#', '!');
}

inline bool MatchLineNumberStart(StyleContext &cxt) {
	return cxt.atLineStart && (cxt.MatchIgnoreCase("#line") ||
		(cxt.Match('#') && (IsADigit(cxt.chNext) || IsADigit(cxt.GetRelative(2)))));
}

inline bool MatchPPDirectiveStart(StyleContext &cxt) {
	return cxt.atLineStart && cxt.Match('#') && iswordstart(cxt.chNext);
}

inline bool MatchTypeAttributeStart(StyleContext &cxt) {
	return cxt.Match('[', '<');
}

inline bool MatchTypeAttributeEnd(StyleContext &cxt) {
	return cxt.Match(']') && cxt.chPrev == '>';
}

inline bool MatchQuotedExpressionStart(StyleContext &cxt) {
	return cxt.Match('<', '@');
}

inline bool MatchQuotedExpressionEnd(StyleContext &cxt) {
	return cxt.Match('>') && cxt.chPrev == '@';
}

inline bool MatchStringStart(StyleContext &cxt) {
	return cxt.Match('"') || cxt.Match('@', '"') || cxt.Match('$', '"') || cxt.Match('`', '`');
}

inline bool MatchStringEnd(StyleContext &cxt, const FSharpString &fsStr) {
	return (cxt.Match('`') && cxt.chPrev == '`') ||
		(fsStr.HasLength() && fsStr.startChar == '"' && cxt.MatchIgnoreCase("\"\"\"")) ||
		(!fsStr.HasLength() && cxt.Match('"') &&
		(cxt.chPrev != '\\' ||
		// treat backslashes as char literals in verbatim strings
		(fsStr.startChar == '@' && cxt.chPrev == '\\')));
}

inline bool MatchCharStart(StyleContext &cxt) {
	// don't style generic type parameters: 'a, 'b, 'T, etc.
	return cxt.Match('\'') && !(cxt.chPrev == ':' || cxt.GetRelative(-2) == ':');
}

inline bool CanEmbedQuotes(StyleContext &cxt) {
	// allow unescaped double quotes inside triple-quoted strings and white-spaced identifiers:
	// - https://docs.microsoft.com/en-us/dotnet/fsharp/language-reference/strings#triple-quoted-strings
	// - https://fsharp.org/specs/language-spec/4.1/FSharpSpec-4.1-latest.pdf#page=25&zoom=auto,-98,600
	return cxt.MatchIgnoreCase("\"\"\"") || cxt.Match('`', '`');
}

inline bool IsNumber(StyleContext &cxt, const int base = 10) {
	return IsADigit(cxt.ch, base) || (IsADigit(cxt.chPrev, base) && strchr(numericMetaChars1, cxt.ch)) ||
		(IsADigit(cxt.GetRelative(-2), base) && strchr(numericMetaChars2, cxt.ch));
}

inline bool IsExponent(StyleContext &cxt) {
	return strchr("eE", cxt.chPrev) && (strchr("+-", cxt.ch) || IsADigit(cxt.ch));
}

inline bool IsFloat(StyleContext &cxt) {
	return IsExponent(cxt) || (cxt.ch == '.' && IsADigit(cxt.chPrev));
}

class LexerFSharp : public DefaultLexer {
	WordList keywords[WORDLIST_SIZE];
	OptionsFSharp options;
	OptionSetFSharp optionSet;

public:
	explicit LexerFSharp() : DefaultLexer(lexerName, SCLEX_FSHARP) {
	}
	static ILexer5 *LexerFactoryFSharp() {
		return new LexerFSharp();
	}
	virtual ~LexerFSharp() {
	}
	void SCI_METHOD Release() noexcept override {
		delete this;
	}
	int SCI_METHOD Version() const noexcept override {
		return lvRelease5;
	}
	const char *SCI_METHOD GetName() noexcept override {
		return lexerName;
	}
	int SCI_METHOD GetIdentifier() noexcept override {
		return SCLEX_FSHARP;
	}
	int SCI_METHOD LineEndTypesSupported() noexcept override {
		return SC_LINE_END_TYPE_DEFAULT;
	}
	void *SCI_METHOD PrivateCall(int, void *) noexcept override {
		return nullptr;
	}
	const char *SCI_METHOD DescribeWordListSets() noexcept override {
		return optionSet.DescribeWordListSets();
	}
	const char *SCI_METHOD PropertyNames() noexcept override {
		return optionSet.PropertyNames();
	}
	int SCI_METHOD PropertyType(const char *name) noexcept override {
		return optionSet.PropertyType(name);
	}
	const char *SCI_METHOD DescribeProperty(const char *name) noexcept override {
		return optionSet.DescribeProperty(name);
	}
	const char *SCI_METHOD PropertyGet(const char *key) noexcept override {
		return optionSet.PropertyGet(key);
	}
	Sci_Position SCI_METHOD PropertySet(const char *key, const char *val) noexcept override;
	Sci_Position SCI_METHOD WordListSet(int n, const char *wl) noexcept override;
	void SCI_METHOD Lex(Sci_PositionU start, Sci_Position length, int initStyle,
						IDocument *pAccess) noexcept override;
	void SCI_METHOD Fold(Sci_PositionU start, Sci_Position length, int initStyle,
						IDocument *pAccess) noexcept override;
};

Sci_Position SCI_METHOD LexerFSharp::PropertySet(const char *key, const char *val) noexcept {
	if (optionSet.PropertySet(&options, key, val)) {
		return 0;
	}

	return -1;
}

Sci_Position SCI_METHOD LexerFSharp::WordListSet(int n, const char *wl) noexcept {
	WordList *wordListN = nullptr;
	Sci_Position firstModification = -1;

	if (n < WORDLIST_SIZE) {
		wordListN = &keywords[n];
	}
	if (wordListN) {
		WordList wlNew;
		wlNew.Set(wl);

		if (*wordListN != wlNew) {
			wordListN->Set(wl);
			firstModification = 0;
		}
	}

	return firstModification;
}

void SCI_METHOD LexerFSharp::Lex(Sci_PositionU start, Sci_Position length, int initStyle,
								IDocument *pAccess) noexcept {
	LexAccessor styler(pAccess);
	StyleContext sc(start, length, initStyle, styler);
	Sci_PositionU cursor = 0;
	UnicodeChar uniCh = UnicodeChar();
	FSharpString fsStr = FSharpString();
	std::map<int, int> bases = { { 'b', 2 }, { 'o', 8 }, { 'x', 16 } };
	constexpr Sci_Position MAX_TOKEN_LEN = 64;
	constexpr int SPACE = ' ';
	int currentBase = 10;

	while (sc.More()) {
		Sci_PositionU colorSpan = sc.currentPos - 1;
		int state = -1;
		bool advance = true;

		switch (sc.state & 0xff) {
			case SCE_FSHARP_DEFAULT:
				cursor = sc.currentPos;

				if (MatchLineNumberStart(sc)) {
					state = SCE_FSHARP_LINENUM;
				} else if (MatchPPDirectiveStart(sc)) {
					state = SCE_FSHARP_PREPROCESSOR;
				} else if (MatchLineComment(sc)) {
					state = SCE_FSHARP_COMMENTLINE;
					sc.Forward();
					sc.ch = SPACE;
				} else if (MatchStreamCommentStart(sc)) {
					state = SCE_FSHARP_COMMENT;
					sc.Forward();
					sc.ch = SPACE;
				} else if (MatchTypeAttributeStart(sc)) {
					state = SCE_FSHARP_ATTRIBUTE;
					sc.Forward();
				} else if (MatchQuotedExpressionStart(sc)) {
					state = SCE_FSHARP_EXPR;
					sc.Forward();
				} else if (MatchCharStart(sc)) {
					state = SCE_FSHARP_CHAR;
				} else if (MatchStringStart(sc)) {
					fsStr.startChar = sc.ch;
					fsStr.startPos = ZERO_LENGTH;
					if (CanEmbedQuotes(sc)) {
						// double quotes after this position should be non-terminating
						fsStr.startPos = static_cast<Sci_Position>(sc.currentPos - cursor);
					}
					state = SCE_FSHARP_STRING;
				} else if (IsADigit(sc.ch, currentBase) || (strchr("+-", sc.ch) && IsADigit(sc.chNext))) {
					state = SCE_FSHARP_NUMBER;
					if (sc.Match('0')) {
						const int prefix = MakeLowerCase(sc.chNext);
						if (bases.find(prefix) != bases.end()) {
							currentBase = bases[prefix];
						}
					}
				} else if (setOperators.Contains(sc.ch) &&
					   // don't use operator style in async keywords (e.g. `return!`)
					   !(sc.Match('!') && iswordchar(sc.chPrev)) &&
					   // don't use operator style in member access, array/string indexing
					   !(sc.Match('.') && (sc.chPrev == '\"' || iswordchar(sc.chPrev)) &&
						 (iswordchar(sc.chNext) || sc.chNext == '['))) {
					state = SCE_FSHARP_OPERATOR;
				} else if (iswordstart(sc.ch)) {
					state = SCE_FSHARP_IDENTIFIER;
				} else {
					state = SCE_FSHARP_DEFAULT;
				}
				break;
			case SCE_FSHARP_LINENUM:
			case SCE_FSHARP_PREPROCESSOR:
			case SCE_FSHARP_COMMENTLINE:
				// TestLexers.cxx will warn about splitting styles across CRLF line endings
				// without the second condition
				if (sc.atLineEnd || sc.Match('\r')) {
					state = SCE_FSHARP_DEFAULT;
					advance = false;
				}
				break;
			case SCE_FSHARP_COMMENT:
			case SCE_FSHARP_ATTRIBUTE:
			case SCE_FSHARP_EXPR:
				if (MatchStreamCommentEnd(sc) || MatchTypeAttributeEnd(sc) || MatchQuotedExpressionEnd(sc)) {
					state = SCE_FSHARP_DEFAULT;
					colorSpan++;
				}
				break;
			case SCE_FSHARP_CHAR:
				if (sc.chPrev == '\\' && sc.GetRelative(-2) != '\\') {
					uniCh = UnicodeChar(sc.ch);
				} else if (sc.Match('\'') && sc.chPrev != '\\') {
					// byte literal?
					if (sc.Match('\'', 'B')) {
						sc.Forward();
						colorSpan++;
					}
					if (!sc.atLineEnd) {
						colorSpan++;
					} else {
						sc.ChangeState(SCE_FSHARP_IDENTIFIER);
					}
					state = SCE_FSHARP_DEFAULT;
				} else {
					uniCh.Parse(sc.ch);
					if (uniCh.AtEnd() && (sc.currentPos - cursor) >= 2) {
						// terminate now, since we left the char behind
						sc.ChangeState(SCE_FSHARP_IDENTIFIER);
						advance = false;
					}
				}
				break;
			case SCE_FSHARP_STRING:
				// continue string, but highlight embedded whitespace
				if (sc.Match('\\') && sc.chPrev != '\\' && IsASpaceOrTab(sc.chNext)) {
					state = SCE_FSHARP_WHITE;
				} else if (sc.Match('\\', '\\')) {
					sc.ch = SPACE;
				} else if (MatchStringEnd(sc, fsStr)) {
					const Sci_Position strLen = static_cast<Sci_Position>(sc.currentPos - cursor);
					// if it looks like the end of a string, find the beginning
					for (Sci_Position i = -strLen; i < 0; i++) {
						const int startQuote = sc.GetRelative(i);
						if (startQuote == '\"' ||
							(startQuote == '`' && sc.GetRelative(i - 1) == '`')) {
							// byte array?
							if (sc.Match('\"', 'B')) {
								sc.Forward();
								colorSpan++;
							}
							if (!sc.atLineEnd) {
								colorSpan++;
							} else {
								sc.ChangeState(SCE_FSHARP_IDENTIFIER);
							}
							state = SCE_FSHARP_DEFAULT;
							break;
						}
					}
				}
				break;
			case SCE_FSHARP_WHITE:
				if (sc.Match('\\')) {
					state = SCE_FSHARP_STRING;
					sc.ch = SPACE;
					styler.ColourTo((colorSpan + 1), sc.state);
					styler.Flush();
					Sci_Position p = static_cast<Sci_Position>(colorSpan - 1);
					for (; p >= 0 && styler.StyleAt(p) == sc.state; p--)
						;
					if (p >= 0) {
						state = static_cast<int>(styler.StyleAt(p));
					}
					sc.ChangeState(state);
					state = -1;
				}
				break;
			case SCE_FSHARP_IDENTIFIER:
				if (!(IsAlphaNumeric(sc.ch) || sc.Match('\''))) {
					const Sci_Position wordLen = static_cast<Sci_Position>(sc.currentPos - cursor);
					if (wordLen < MAX_TOKEN_LEN) {
						// wordLength is believable as keyword, [re-]construct token - RR
						char token[MAX_TOKEN_LEN] = { 0 };
						for (Sci_Position i = -wordLen; i < 0; i++) {
							token[wordLen + i] = static_cast<char>(sc.GetRelative(i));
						}
						token[wordLen] = '\0';
						// don't style snake_case_identifiers
						if (!(sc.Match('_') || sc.GetRelative(-wordLen - 1) == '_')) {
							for (int i = 0; i < WORDLIST_SIZE; i++) {
								if (keywords[i].InList(token)) {
									switch (i) {
										case 0:
											sc.ChangeState(SCE_FSHARP_KEYWORD);
											break;
										case 1:
											sc.ChangeState(SCE_FSHARP_KEYWORD2);
											break;
										case 2:
											sc.ChangeState(SCE_FSHARP_KEYWORD3);
											break;
										case 3:
											sc.ChangeState(SCE_FSHARP_KEYWORD4);
											break;
										case 4:
											sc.ChangeState(SCE_FSHARP_KEYWORD5);
											break;
										default:
											sc.ChangeState(SCE_FSHARP_KEYWORD);
											break;
									}
									break;
								}
							}
						}
					}
					state = SCE_FSHARP_DEFAULT;
					advance = false;
				}
				break;
			case SCE_FSHARP_OPERATOR:
				// special-case "()" and "[]" tokens as KEYWORDS - RR
				if (setClosingTokens.Contains(sc.ch) &&
					((sc.Match(')') && sc.chPrev == '(') || (sc.Match(']') && sc.chPrev == '['))) {
					sc.ChangeState(SCE_FSHARP_KEYWORD);
					colorSpan++;
				} else {
					advance = false;
				}

				state = SCE_FSHARP_DEFAULT;
				break;
			case SCE_FSHARP_NUMBER:
				state = (IsNumber(sc, currentBase) || IsExponent(sc) || IsFloat(sc))
						? SCE_FSHARP_NUMBER
						// change style even when operators aren't spaced
						: setOperators.Contains(sc.ch) ? SCE_FSHARP_OPERATOR : SCE_FSHARP_DEFAULT;
				currentBase = (state == SCE_FSHARP_NUMBER) ? currentBase : 10;
				break;
		}

		if (state >= SCE_FSHARP_DEFAULT) {
			styler.ColourTo(colorSpan, sc.state);
			sc.ChangeState(state);
		}

		if (advance) {
			sc.Forward();
		}
	}

	sc.Complete();
}

void SCI_METHOD LexerFSharp::Fold(Sci_PositionU start, Sci_Position length, int initStyle,
								IDocument *pAccess) noexcept {
	if (!options.fold) {
		return;
	}

	LexAccessor styler(pAccess);
	Sci_Position lineCurrent = styler.GetLine(start);
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent + 1);
	const Sci_PositionU endPos = start + length;
	int style = initStyle;
	int styleNext = styler.StyleAt(start);
	char chNext = styler[start];
	int levelNext;
	int levelCurrent = SC_FOLDLEVELBASE;
	int visibleChars = 0;
	int8_t commentLinesInFold = -1;
	int8_t importsInFold = -1;

	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 0x10;
	}

	levelNext = levelCurrent;

	for (Sci_PositionU i = start; i < endPos; i++) {
		const bool atEOL = i == (lineStartNext - 1);
		const int stylePrev = style;
		const char ch = chNext;
		bool inLineComment = (style == SCE_FSHARP_COMMENTLINE);
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		chNext = styler.SafeGetCharAt(i + 1);

		if (options.foldPreprocessor && style == SCE_FSHARP_PREPROCESSOR) {
			if (ch == '#') {
				Sci_PositionU j = i + 1;
				while ((j < endPos) && IsASpaceOrTab(styler.SafeGetCharAt(j))) {
					j++;
				}
				if (styler.Match(j, "if")) {
					levelNext++;
				} else if (styler.Match(j, "endif")) {
					levelNext--;
				}
			}
		}

		if (options.foldImports && style == SCE_FSHARP_KEYWORD) {
			bool haveImport = false;

			// find import declarations at any position
			for (Sci_Position i = lineCurrent; i < styler.LineEnd(lineCurrent); i++) {
				if (styler.Match(i, "open")) {
					haveImport = true;
					break;
				}
			}

			if (haveImport) {
				bool haveImportList = false;

				for (Sci_Position i = lineStartNext; i < styler.LineEnd(lineCurrent + 1); i++) {
					if (styler.Match(i, "open")) {
						haveImportList = true;
						break;
					}
				}

				if (haveImportList) {
					importsInFold++;
					if (importsInFold < 1) {
						levelNext++;
					}
				} else {
					const int8_t offset = importsInFold > 0 ? importsInFold : 1;
					levelNext -= offset;
					importsInFold = -1;
				}
			}
		}

		if (options.foldComment && options.foldCommentMultiLine && style == SCE_FSHARP_COMMENTLINE) {
			if (stylePrev != SCE_FSHARP_COMMENTLINE) {
				bool haveCommentList = false;

				// find line comments at any position
				for (Sci_Position i = lineStartNext; i < styler.LineEnd(lineCurrent + 1); i++) {
					if (styler.Match(i, "//") || styler.Match(i, "///")) {
						haveCommentList = true;
						break;
					}
				}

				if (haveCommentList) {
					commentLinesInFold++;
					if (commentLinesInFold < 1) {
						levelNext++;
					}
				} else {
					const int8_t offset = commentLinesInFold > 0 ? commentLinesInFold : 1;
					levelNext -= offset;
					commentLinesInFold = -1;
				}
			}
		}

		if (options.foldComment && options.foldCommentOCamlStyle && style == SCE_FSHARP_COMMENT &&
		    !inLineComment) {
			if (stylePrev != SCE_FSHARP_COMMENT) {
				levelNext++;
			} else if (styleNext != SCE_FSHARP_COMMENT && !atEOL) {
				levelNext--;
			}
		}

		if (!IsASpace(ch)) {
			visibleChars++;
		}

		if (atEOL || (i == endPos - 1)) {
			int levelUse = levelCurrent;
			int lev = levelUse | levelNext << 16;
			if (visibleChars == 0 && options.foldCompact) {
				lev |= SC_FOLDLEVELWHITEFLAG;
			}
			if (levelUse < levelNext) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			lineStartNext = styler.LineStart(lineCurrent + 1);
			levelCurrent = levelNext;
			if (atEOL && (i == static_cast<Sci_PositionU>(styler.Length() - 1))) {
				styler.SetLevel(lineCurrent,
						(levelCurrent | levelCurrent << 16) | SC_FOLDLEVELWHITEFLAG);
			}
			visibleChars = 0;
			inLineComment = false;
		}
	}
}
} // namespace

LexerModule lmFSharp(SCLEX_FSHARP, LexerFSharp::LexerFactoryFSharp, lexerName, fsharpWordLists);
