># Caesar Cipher Plus
## Usage
### Encrypt
Command: caesar -e -p password -s string_to_encrypt <br>
e.g. <br>
<pre> ./caesar -e -p AABDCA -s  github.com
Output:
    string to process: github.com
    encrypted: )+6*7$N%1/
</pre>
### Decrypt
Command: caesar -d -p password -s string_to_encrypt <br>
Note: '\\' is used to escape special characters <br>
e.g. <br>
<pre> ./caesar -d -p AABDCA -s \)\+\6\*\7\$\N\%\1\/
Output:
    string to process: )+6*7$N%1/
    decrypted: github.com
</pre>

### Verbose Output
Please add "-v" to command

## How It Works
We all know that Caesar Cipher is a classic shift ciper. It rotates the whole plain text by a specific distance. Therefore, if we encrypt a string with 26 characters, there are only 26 possible output. <br>
In this project, we enhance this by, <br>
1. Using ASCII code from 33, character '!' to 126, character "~". The ASCII version used is 1967, please checkout at, <br>
   https://en.wikipedia.org/wiki/ASCII <br>
2. Using a password to make the shift more randomly. The duplicated characters in password are removed to get a key. The key is used to build mapping table.Following is an example for password "AABDCA"<br>
password: AABDCA <br>
key:      ABDC <br>
string to process: github.com <br>
encrypted: )+6*7$N%1/ <br>
mapping table: <br>

|ASCII | 33| 34| 35| 36| 37| 38| 39| 40| 41| 42| 43| 44| 45| 46| 47|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | ! | " | # | $ | % | & | ' | ( | ) | * | + | , | - | . | / |
|cipher| A | B | D | C | E | F | G | H | I | J | K | L | M | N | O |

|ASCII | 48| 49| 50| 51| 52| 53| 54| 55| 56| 57| 58| 59| 60| 61| 62|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | : | ; | < | = | > |
|cipher| P | Q | R | S | T | U | V | W | X | Y | Z | [ | \ | ] | ^ |

|ASCII | 63| 64| 65| 66| 67| 68| 69| 70| 71| 72| 73| 74| 75| 76| 77|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | ? | @ | A | B | C | D | E | F | G | H | I | J | K | L | M |
|cipher| _ | ` | a | b | c | d | e | f | g | h | i | j | k | l | m |

|ASCII | 78| 79| 80| 81| 82| 83| 84| 85| 86| 87| 88| 89| 90| 91| 92|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | [ | \ |
|cipher| n | o | p | q | r | s | t | u | v | w | x | y | z | { | \| |

|ASCII | 93| 94| 95| 96| 97| 98| 99|100|101|102|103|104|105|106|107|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | ] | ^ | _ | ` | a | b | c | d | e | f | g | h | i | j | k |
|cipher| } | ~ | ! | " | # | $ | % | & | ' | ( | ) | * | + | , | - |

|ASCII |108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|plain | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z |
|cipher| . | / | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | : | ; | < |

|ASCII |123|124|125|126|
|------|---|---|---|---|
|plain | { | \| | } | ~ |
|cipher| = | > | ? | @ |
   
   
