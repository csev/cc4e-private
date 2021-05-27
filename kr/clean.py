import re
# https://pypi.org/project/fuzzy-match/
from fuzzy_match import match
from fuzzy_match import algorithims

reserved = [
"#define",
"auto", "else", "long", "switch", "break", "enum", "register",
"typedef", "case", "extern", "return", "union", "char",
"float", "short", "unsigned", "const", "for", "signed",
"void", "continue", "goto", "sizeof", "volatile", "default",
"if", "static", "while", "do", "int", "struct", "double"
]

codefix = {
    "/\\*": "/*",
    "\\*/": "*/",
    "\\*": "*",
    "\\&lt;": "<",
    "\\&gt;": ">",
    "&lt;": "<",
    "&gt;": ">",
}

special = "{}()%";

patches = {
        "priirtf" : "printf",
        "*define" : "#define",
        "\\*define" : "#define",
        '&#39;': "'",
        '&quot;': '"',
        'charactei': 'character',
        'beteitien': 'between',
}

# decide if a line is code (to be indented)
def iscode(line) : 
    global reserved, special, codefix, punctuation

    surebet = line.startswith('/\\*') or line.endswith('\\*/')
    keywords = 0
    nonkeywords = 0
    words = [item for item in re.split("(\W+)", line) if len(item) > 0]
    words = re.split("(\W+)", line);

    for word in words :
        # print('     ', word)
        key = False
        for res in reserved:
            if res == word : key = True
        if key: 
            keywords = keywords + 1
        else:
            nonkeywords = nonkeywords + 1
    keywordstart = False
    for res in reserved:
        if line.startswith(res) : keywordstart = True

    semi = line.endswith(';')
    specials = 0
    for ch in line:
        for spec in special:
            if ch == spec : specials = specials + 1

    if not surebet and nonkeywords > 19 : return line
    if not surebet and not keywordstart and not semi and specials < 2 : return line
    # print(keywordstart, keywords, nonkeywords, semi, specials, line)
    for old, new in codefix.items():
        line = line.replace(old, new)
    return '    '+line

# Note tables are vertical bars
hand = open("chap01/chap01_orig.md")
# wordsplit = u'\u00AD'.encode('utf-8')
wordsplit = u'\u00AD'
chaptext = ""
sectext = ""
booktext = "THE C PROGRAMMING LANGUAGE"
lines = hand.read().split("\n");
join = ""
pageno = 0
for i in range(len(lines)):
    line = lines[i]
    line = line.rstrip()
    if len(join) > 0 : line = join + line
    for old, new in patches.items() :
        line = line.replace(old, new)

    if line.endswith(wordsplit) :
        # join =  line[:-1*len(wordsplit)]
        join =  line[:-1]
        continue

    # 26 THE C PROGRAMMING LANGUAGE CHAPTER I
    if algorithims.levenshtein(chaptext, line) > 0.5 or algorithims.levenshtein(booktext, line) > 0.5 :
        words = line.split()
        pageno = pageno + 1
        if len(words) > 1 and words[0].isnumeric() :
            pageno = int(words[0])
        elif len(words) > 2 and words[-1].isnumeric() : 
            pageno = int(words[-1])
        print("[comment]: <> (page",pageno,",",line,")");
        continue

    chapter = re.search("^CHAPTER [0-9]+:",line);
    if chapter : chaptext = line.replace(':', '')
    # 1.1 Getting Started
    section = re.search("^[0-9]+.[0-9]+ ",line);

    print(iscode(line))
    if ( chapter ) : print('='*len(line))
    if ( section ) : print('-'*len(line))
    prevline = line
    join = ""


