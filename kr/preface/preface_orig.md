**THE**

PROGRAMMING
LANGUAGE

![](RackMultipart20210527-4-11w799e_html_5daaee307c435c79.png)

THE

# PROGRAMMING

#
 LANGUAGE

Brian W. Kernighan

Dennis M. Ritchie

_Bell Laboratories_

_Murray Hill, New Jersey_

PRENTICE-HALL, INC., Englewood Cliffs, New Jersey 07632

_Library of Congress Cataloging in Publication Data_

KERNIGHAN, BRIAN W.

The C programming language.

Includes index.

1. C (Computer program language) I. RITCHIE,

DENNIS M., joint author. 11. Title.

QA76.73.C15K47 001.6 424 77-28983

ISBN 0-13-110163-3

**The Publisher offers discounts on this book when ordered in bulk**
**quantities. For more information write:**

**Special Sales/College Marketing**

**Prentice-Hall, Inc.**

**College Technical and Reference Division**

**Englewood Cliffs, N.J. 07632**

Copyright @ 1978 by Bell Telephone Laboratories, Incorporated.

All rights reserved. No part of this publication may be reproduced, stored in a retrieval
system, or transmitted, in any form or by any means, electronic, mechanical, photocopy­
ing, recording, or otherwise, without the prior written permission of the publisher. Print­
ed in the United States of America. Published simultaneously in Canada.

This book was set in Times Roman and Courier 12 by the authors, using a Graphic Sys­
tems phototypesetter driven by a PDP-11/70 running under the UNIX operating system.

UNIX is a Trademark of Bell Laboratories.

25 24

PRENTICE-HALL INTERNATIONAL, INC., London

PRENTICE-HALL OF AUSTRALIA PTY. LIMITED, Sydney

PRENTICE-HALL OF CANADA, LTD., Toronto

PRENTICE-HALL OF INDIA PRIVATE LIMITED, New Delhi

PRENTICE-HALL OF JAPAN, INC., Tokyo

PRENTICE-HALL OF SOUTHEAST ASIA PTE. LTD., Singapore

WHITEHALL BOOKS LIMITED, Wellington, New Zealand

**CONTENTS**

**Preface ix**

**Chapter 0 Introduction 1**

**Chapter 1 A Tutorial Introduction 5**

1.1 Getting Started 5

1.2 Variables and Arithmetic 8

1.3 The For Statement 11

1.4 Symbolic Constants 12

1.5 A Collection of Useful Programs 13

1.6 Arrays 20

1.7 Functions 22

1.8 Arguments — Call by Value 24

1.9 Character Arrays 25

1.10 Scope; External Variables 28

1.11 Summary 31

**Chapter 2 Types, Operators and Expressions 33**

2.1 Variable Names 33

2.2 Data Types and Sizes 33

2.3 Constants 34

2.4 Declarations 36

2.5 Arithmetic Operators 37

2.6 Relational and Logical Operators 38

2.7 Type Conversions 39

2.8 Increment and Decrement Operators 42

2.9 Bitwise Logical Operators **44**

2.10 Assignment Operators and Expressions 46

2.11 Conditional Expressions 47

2.12 Precedence and Order of Evaluation 48

| Vi THE C PROGRAMMING LANGUAGE | |
| --- | --- |
| **Chapter 3** | **Control Flow** | **51** |
| 3.1 | Statements and Blocks | 51 |
| 3.2 | If-Else | 51 |
| 3.3 | Else-If | 53 |
| 3.4 | Switch | 54 |
| 3.5 | Loops - While and For | 56 |
| 3.6 | Loops - Do-while | 59 |
| 3.7 | Break | 61 |
| 3.8 | Continue | 62 |
| 3.9 | Goto&#39;s and Labels | 62 |
| **Chapter 4** | **Functions and Program Structure** | **65** |
| 4.1 | Basics | 65 |
| 4.2 | Functions Returning Non-Integers | 68 |
| 4.3 | More on Function Arguments | 71 |
| 4.4 | External Variables | 72 |
| 4.5 | Scope Rules | 76 |
| 4.6 | Static Variables | 80 |
| 4.7 | Register Variables | 81 |
| 4.8 | Block Structure | 81 |
| 4.9 | Initialization | 82 |
| 4.10 | Recursion | 84 |
| 4.11 | The C Preprocessor | 86 |
| **Chapter 5** | **Pointers and Arrays** | **89** |
| 5.1 | Pointers and Addresses | 89 |
| 5.2 | Pointers and Function Arguments | 91 |
| 5.3 | Pointers and Arrays | 93 |
| 5.4 | Address Arithmetic | 96 |
| 5.5 | Character Pointers and Functions | 99 |
| 5.6 | Pointers are not Integers | 102 |
| 5.7 | Multi-Dimensional Arrays | 103 |
| 5.8 | Pointer Arrays; Pointers to Pointers | 105 |
| 5.9 | Initialization of Pointer Arrays | 109 |
| 5.10 | Pointers vs. Multi-dimensional Arrays | 110 |
| 5.11 | Command-line Arguments | 110 |
| 5.12 | Pointers to Functions | 114 |
| **Chapter 6** | **Structures** | **119** |
| 6.1 | Basics | 119 |
| 6.2 | Structures and Functions | 121 |
| 6.3 | Arrays of Structures | 123 |

| | CONTENTS | Vii |
| --- | --- | --- |
| 6.4 | Pointers to Structures | 128 |
| 6.5 | Self-referential Structures | 130 |
| 6.6 | Table Lookup | 134 |
| 6.7 | Fields | 136 |
| 6.8 | Unions | 138 |
| 6.9 | Typedef | 140 |
| **Chapter 7** | **Input and Output** | **143** |
| 7.1 | Access to the Standard Library | 143 |
| 7.2 | Standard Input and Output - Getchar and Putchar | 144 |
| 7.3 | Formatted Output - Printf | 145 |
| 7.4 | Formatted Input - Scanf | 147 |
| 7.5 | In-memory Format Conversion | 150 |
| 7.6 | File Access | 151 |
| 7.7 | Error Handling - Stderr and Exit | 154 |
| 7.8 | Line Input and Output | 155 |
| 7.9 | Some Miscellaneous Functions | 156 |
| **Chapter 8** | **The UNIX System Interface** | **159** |
| 8.1 | File Descriptors | 159 |
| 8.2 | Low Level I/O - Read and Write | 160 |
| 8.3 | Open, Creat, Close, Unlink | 162 |
| 8.4 | Random Access - Seek and Lseek | 164 |
| 8.5 | Example - An Implementation of Fopen and Getc | 165 |
| 8.6 | Example - Listing Directories | 169 |
| 8.7 | Example - A Storage Allocator | 173 |
| **Appendix A** | **C Reference Manual** | **179** |
| 1. | Introduction | 179 |
| 2. | Lexical conventions | 179 |
| 3. | Syntax notation | 182 |
| 4. | What&#39;s in a name? | 182 |
| 5. | Objects and lvalues | 183 |
| 6. | Conversions | 183 |
| 7. | Expressions | 185 |
| 8. | Declarations | 192 |
| 9. | Statements | 201 |
| 10. | External definitions | 204 |
| 11. | Scope rules | 205 |
| 12. | Compiler control lines | 207 |
| 13. | Implicit declarations | 208 |
| 14. | Types revisited | 209 |
| 15. | Constant expressions | 211 |

viii THE C PROGRAMMING LANGUAGE

1. Portability considerations 211
2. Anachronisms 212
3. Syntax Summary 214

**PREFACE**

C is a general-purpose programming language which features economy
of expression, modern control flow and data structures, and a rich set of
operators. C is not a &quot;very high level&quot; language, nor a &quot;big&quot; one, and is
not specialized to any particular area of application. But its absence of res­
trictions and its generality make it more convenient and effective for many
tasks than supposedly more powerful languages.

C was originally designed for and implemented on the UNIXt operating
system on the DEC PDP-11, by Dennis Ritchie. The operating system, the
C compiler, and essentially all UNIX applications programs (including all of
the software used to prepare this book) are written in C. Production com­
pilers also exist for several other machines, including the IBM System/370,
the Honeywell 6000, and the Interdata 8/32. C is not tied to any particular
hardware or system, however, and it is easy to write programs that will run
without change on any machine that supports C.

This book is meant to help the reader learn how to program in C. It
contains a tutorial introduction to get new users started as soon as possible,
separate chapters on each major feature, and a reference manual. Most of
the treatment is based on reading, writing and revising examples, rather
than on mere statements of rules. For the most part, the examples are com­
plete, real programs, rather than isolated fragments. All examples have
been tested directly from the text, which is in machine-readable form.
Besides showing how to make effective use of the language, we have also
tried where possible to illustrate useful algorithms and principles of good
style and sound design.

The book is not an introductory programming manual; it assumes some
familiarity with basic programming concepts like variables, assignment state­
ments, loops, and functions. Nonetheless, a novice programmer should be
able to read along and pick up the language, although access to a more

t UNIX is a Trademark of Bell Laboratories. The UNIX operating system is
available under license from Western Electric, Greensboro, N. C.

ix

X THE C PROGRAMMING LANGUAGE

knowledgeable colleague will help.

In our experience, C has proven to be a pleasant, expressive, and versa­
tile language for a wide variety of programs. It is easy to learn, and it wears
well as one&#39;s experience with it grows. We hope that this book will help you
to use it well.

The thoughtful criticisms and suggestions of many friends and col­
leagues have added greatly to this book and to our pleasure in writing it. In
particular, Mike Bianchi, Jim Blue, Stu Feldman, Doug McIlroy, Bill
Roome, Bob Rosin, and Larry Rosler all read multiple versions with care.
We are also indebted to Al Aho, Steve Bourne, Dan Dvorak, Chuck Haley,
Debbie Haley, Marion Harris, Rick Holt, Steve Johnson, John Mashey, Bob
Mitze, Ralph Muha, Peter Nelson, Elliot Pinson, Bill Plauger, Jerry Spivack,
Ken Thompson, and Peter Weinberger for helpful comments at various
stages, and to Mike Lesk and Joe Ossanna for invaluable assistance with
typesetting.

Brian W. Kernighan
Dennis M. Ritchie

